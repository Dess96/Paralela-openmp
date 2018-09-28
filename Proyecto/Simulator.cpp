#include"Simulator.h"
#include"Person.h"
#include<iostream>
#include<random>
#include<time.h>
#include<omp.h>
#include <fstream>
#include<stdlib.h>

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
	number_people = number_peopleM;
	infectiousness = infectiousnessM;
	chance_recover = chance_recoverM;
	death_duration = death_durationM;
	infected = infectedM;
	world_size = world_sizeM;
	tic = ticM;
	thread_count = omp_get_num_threads();
	Person p;
#pragma omp single
	{
		v.resize(world_size); //Vector de vectores de tamaño world_size*world_size
		world.resize(world_size, v);
	}
	int perc;
	int healthy;
	int pos1, pos2;
	perc = number_people * infected / 100; //Cantidad correspondiente al porcentaje dado
	healthy = number_people - perc; //Gente sana
	srand(time(NULL));
#pragma omp parallel for num_threads (thread_count)
	for (int i = 0; i < perc; i++) { //Cambiamos a los infectados
		p.create(); //Se crean sanos y con su x y y
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
#pragma omp critical
		{
			p.setX(pos1);
			p.setY(pos2);
			p.change_state(1);
		}
#pragma omp atomic
		world[pos1][pos2]++; //Metemos a la persona en la lista de la posicion correspondiente	
		lists.push_back(p);
	}
#pragma omp parallel for num_threads (thread_count)
	for (int i = 0; i < healthy; i++) {
		p.create();
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
#pragma omp critical
		{
		p.setX(pos1);
		p.setY(pos2);
		p.change_state(0);
		}
#pragma omp atomic
		world[pos1][pos2]++;
		lists.push_back(p);
	}
	return healthy;
}

void Simulator::update(string name, int healthy_people) {
	int sick_people = number_people - healthy_people;
	int inmune_people = 0;
	int dead_people = 0;
	for (int actual_tic = 1; actual_tic <= tic; actual_tic++) {
		default_random_engine generator;
		uniform_real_distribution<double> distribution(0.0, 1.0);
		double prob_rec, prob_infect;
		double prob;
		int sick_time;
		int state, state2, i, j, i2, j2, pos1, pos2;
		list<Person>::iterator it;
		list<Person>::iterator it2;
		for (list<Person>::iterator it = lists.begin(); it != lists.end(); ++it) {
			prob = 0;
			i = (*it).getX();
			j = (*it).getY();
			//Cambiar estado si se le debe cambiar el estado
			state = (*it).getState();
			if (state == 0) {
				prob_infect = distribution(generator);
				if (prob_infect < prob) {
					(*it).change_state(1);
					(*it).setSick(1);
					sick_people++;
					healthy_people--;
				}
			}
			else if (state == 1) {
				prob += infectiousness;
				prob_rec = distribution(generator);
				sick_time = (*it).getSick();
				if (prob_rec < chance_recover) {
					(*it).change_state(2);
					inmune_people++;
					sick_people--;
				}
				else if (sick_time > death_duration) {
					(*it).change_state(3);
					dead_people++;
					sick_people--;
				}
				else {
					sick_time++;
					(*it).setSick(sick_time);
				}
			}
			pos1 = movePos(i, world_size);
			pos2 = movePos(j, world_size);
			(*it).setX(pos1);
			(*it).setY(pos2);
			it2 = it;
			it2++;
			if (world[i][j] > 0) {
				world[i][j]--;
				while ((world[i][j] > 0) && (it2 != lists.end())) {
					i2 = (*it2).getX();
					j2 = (*it2).getY();
					if (i == i2 && j == j2) {
						world[i][j]--;
						state2 = (*it2).getState();
						if (state2 == 0) {
							prob_infect = distribution(generator);
							if (prob_infect < prob) {
								(*it2).change_state(1);
								(*it2).setSick(1);
								sick_people++;
								healthy_people--;
							}
						}
						else if (state2 == 1) {
							prob += infectiousness;
							prob_rec = distribution(generator);
							sick_time = (*it2).getSick();
							if (prob_rec < chance_recover) {
								(*it2).change_state(2);
								inmune_people++;
								sick_people--;
							}
							else if (sick_time > death_duration) {
								(*it2).change_state(3);
								dead_people++;
								sick_people--;
							}
							else {
								sick_time++;
								(*it2).setSick(sick_time);
							}
						}
						pos1 = movePos(i, world_size);
						pos2 = movePos(j, world_size);
						(*it2).setX(pos1);
						(*it2).setY(pos2);
					}
					it2++;
				}
			}
		}
		clear(actual_tic, name, healthy_people, dead_people, sick_people, inmune_people);
	}
#pragma omp single
	{
	cout << "Archivo generado" << endl;
	lists.clear();
	world.clear();
	}
}

int Simulator::movePos(int pos, int world_size) {
	int movX;
	movX = rand() % 2;
	movX -= 1;
	pos += movX;
	if (pos < 0) {
		pos = world_size - 1;
	}
	else if (pos >= world_size) {
		pos = 0;
	}
	return pos;
}

void Simulator::clear(int actual_tic, string name, int healthy_people, int dead_people, int sick_people, int inmune_people) {
	int i, j, state;
	list<Person>::iterator it = lists.begin();
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

	while (it != lists.end()) {
		i = (*it).getX();
		j = (*it).getY();
		state = (*it).getState();
		if (state == 3) {
			lists.erase(it++);
		}
		else{
			++it;
		}
		world[i][j]++;
	}
}
