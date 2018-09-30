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
		lists.resize(number_people);
	}
	perc = number_people * infected / 100; //Cantidad correspondiente al porcentaje dado
	healthy = number_people - perc; //Gente sana
	srand(time(NULL));
#pragma omp parallel for num_threads(thread_count)
	for (int i = 0; i < perc; i++) { //Cambiamos a los infectados
		p.create(); //Se crean sanos y con su x y y
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
#pragma omp critical
		{
			p.setX(pos1);
			p.setY(pos2);
			p.change_state(1);
			world[pos1][pos2]++; //Metemos a la persona en la lista de la posicion correspondiente	
			lists[i] = p;
		}
	}

#pragma omp parallel for num_threads(thread_count)
	for (int j = perc; j < lists.size(); j++) {
		p.create();
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
#pragma omp critical
		{
			p.setX(pos1);
			p.setY(pos2);
			p.change_state(0);
			world[pos1][pos2]++;
			lists[j] = p;
		}
	}
#pragma omp single
	{
		for (vector<Person>::iterator it = lists.begin(); it != lists.end(); ++it) {
			cout << (*it).getX() << (*it).getY() << (*it).getState() << endl;
		}
	}
	return healthy;
}

void Simulator::update(string name, int healthy_people) {
	default_random_engine generator;
	uniform_real_distribution<double> distribution(0.0, 1.0);
	int sick_people = number_people - healthy_people;
	int inmune_people = 0;
	int dead_people = 0;
	int state, i, j, pos1, pos2, sick_time;
	double prob_rec, prob_infect;
	double prob;
	for (int actual_tic = 1; actual_tic <= tic; actual_tic++) {
#pragma omp parallel for num_threads(thread_count)
		for (int k = 0; k < lists.size(); k++) {
			prob = 0;
			i = lists[k].getX();
			j = lists[k].getY();
			pos1 = movePos(i, world_size);
			pos2 = movePos(j, world_size);
#pragma omp critical
			{
				lists[k].setX(pos1);
				lists[k].setY(pos2);
				world[i][j]--;
			}
		}
#pragma omp single
		{
			cout << "Actualizadas" << endl;
			for (vector<Person>::iterator it = lists.begin(); it != lists.end(); ++it) {
				cout << (*it).getX() << (*it).getY() << (*it).getState() << endl;
			}
		}
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
