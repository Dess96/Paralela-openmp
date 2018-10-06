#include"Simulator.h"
#include"Person.h"
#include<iostream>
#include<random>
#include<time.h>
#include <fstream>
#include<stdlib.h>
#include<omp.h>

using namespace std;

/* world_size: Tamaño del espacio bidimensional
* number_people: Cantidad total de personas
* death_duration: Cuanto pasa antes de que se mueran las personas
* tic: Cantidad de tics que dura la simulacion
* infectiousness: Potencia infecciosan
* chance_recover: Probabilidad de recuperacion
* infected: Personas infectadas inicialmente*/
int world_size, death_duration, tic, thread_count;
int healthy_people, dead_people, sick_people, inmune_people;
double infected, infectiousness, chance_recover, number_people;

int Simulator::initialize(int number_peopleM, double infectiousnessM, double chance_recoverM, int death_durationM, double infectedM, int world_sizeM, int ticM) {
	int perc;
	int healthy;
	int pos1, pos2;
	number_people = number_peopleM;
	infectiousness = infectiousnessM;
	chance_recover = chance_recoverM;
	death_duration = death_durationM;
	infected = infectedM;
	world_size = world_sizeM;
	tic = ticM;
	thread_count = omp_get_thread_num();
	Person p;
#pragma omp single
	{
		v.resize(world_size); //Vector de vectores de tamaño world_size*world_size
		world.resize(world_size, v);
		peopleVec.resize(number_people);
	}
	perc = number_people * infected / 100; //Cantidad correspondiente al porcentaje dado
	healthy = number_people - perc; //Gente sana
	srand(time(NULL));
#pragma omp parallel for num_threads(thread_count)
	for (int i = 0; i < perc; i++) { //Cambiamos a los infectados
		Person p; //Se crean sanos y con su x y y
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
		p.setX(pos1);
		p.setY(pos2);
		p.change_state(1);
		p.setSick(1);
#pragma omp atomic
		world[pos1][pos2]++; //Metemos a la persona en la lista de la posicion correspondiente	
		peopleVec[i] = p;
	}

#pragma omp parallel for num_threads(thread_count)
	for (int j = perc; j < peopleVec.size(); j++) {
		Person p;
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
		p.setX(pos1);
		p.setY(pos2);
#pragma omp atomic
		world[pos1][pos2]++;
		peopleVec[j] = p;
	}
	return healthy;
}

void Simulator::update(string name, int healthy) {
	healthy_people = healthy;
	sick_people = number_people - healthy_people;
	int state, i, j, pos1, pos2;
	for (int actual_tic = 1; actual_tic <= tic; actual_tic++) {
#pragma omp parallel for num_threads(thread_count)
		for (int k = 0; k < peopleVec.size(); k++) {
			i = peopleVec[k].getX();
			j = peopleVec[k].getY();
			changeState(k);
			pos1 = movePos(i, world_size);
			pos2 = movePos(j, world_size);
			peopleVec[k].setX(pos1);
			peopleVec[k].setY(pos2);
#pragma omp atomic
			world[i][j]--;
			if (world[i][j] > 0) {
				check_vec(k, i, j);
			}
		}
#pragma omp single
		cout << "Enfermos " << sick_people << " inmunes " << inmune_people << " sanos " << healthy_people << " muertos " << dead_people << " iteracion " << actual_tic << endl;
//		clear(actual_tic, name);
	}
#pragma omp single
	{
		cout << "Archivo generado" << endl;
		peopleVec.clear();
		world.clear();
	}
}

int Simulator::movePos(int pos, int world_size) {
	int movX;
	movX = rand() % 2;
#pragma omp atomic
	movX -= 1;
#pragma omp atomic
	pos += movX;
	if (pos < 0) {
		pos = world_size - 1;
	}
	else if (pos >= world_size) {
		pos = 0;
	}
	return pos;
}

void Simulator::check_vec(int k, int i, int j) {
	int i2, j2, pos1, pos2;
#pragma omp parallel for num_threads(thread_count)
	for (int l = k; l < peopleVec.size(); l++) {
		if (world[i][j] > 0) {
			i2 = peopleVec[l].getX();
			j2 = peopleVec[l].getY();
			if ((i2 == i) && (j2 == j)) {
				changeState(l);
				pos1 = movePos(i, world_size);
				pos2 = movePos(j, world_size);
				peopleVec[l].setX(pos1);
				peopleVec[l].setY(pos2);
#pragma omp atomic
				world[i][j]--;
			}
		}
	}
}

void Simulator::changeState(int i) {
	random_device generator;
	uniform_real_distribution<double> distribution(0.0, 1.0);
	double prob_infect, prob_rec;
	int sick_time;
	int state = peopleVec[i].getState();
	if (state == 1) {
		prob += infectiousness;
		sick_time = peopleVec[i].getSick();
		if (sick_time >= death_duration) {
			prob_rec = distribution(generator);
			if (prob_rec < chance_recover) {
				peopleVec[i].change_state(2);
				sick_people--;
				inmune_people++;
			}
			else {
				peopleVec[i].change_state(3);
				sick_people--;
				dead_people++;
			}
		}
		else {
			sick_time++;
			peopleVec[i].setSick(sick_time);
		}
	}
	else if (state == 0) {
		prob_infect = distribution(generator);
		if (prob_infect < prob) {
			peopleVec[i].change_state(1);
			peopleVec[i].setSick(1);
			healthy_people--;
			sick_people++;
		}
		prob = 0;
	}
}

void Simulator::clear(int actual_tic, string name) {
#pragma omp single
	{
		ofstream file;
		file.open(name, ios_base::app);
		file << "Reporte del tic " << actual_tic << endl
			<< " Personas muertas total " << dead_people << ", promedio " << dead_people / actual_tic << ", porcentaje " << number_people * dead_people / 100 << endl
			<< " Personas sanas total " << healthy_people << ", promedio " << healthy_people / actual_tic << ", porcentaje " << number_people * healthy_people / 100 << endl
			<< " Personas enfermas total " << sick_people << ", promedio " << sick_people / actual_tic << ", porcentaje " << number_people * sick_people / 100 << endl
			<< " Personas inmunes total " << inmune_people << ", promedio " << inmune_people / actual_tic << ", porcentaje " << number_people * inmune_people / 100 << endl;

		file.close();//Hacer archivo
	}
}
