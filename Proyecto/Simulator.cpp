#include"Simulator.h"
#include"Person.h"
#include<iostream>
#include<random>
#include<time.h>
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
void Simulator::initialize(int world_size, int number_people, double infected) {
	Person p;
	v.resize(world_size); //Vector de vectores de tamaño world_size*world_size
	world.resize(world_size, v);
	int perc;
	int pos1, pos2;
	int healthy;
	perc = number_people * infected / 100; //Cantidad correspondiente al porcentaje dado
	healthy = number_people - perc; //Gente sana
	srand(time(NULL));
	for (int i = 0; i < perc; i++) { //Cambiamos a los infectados
		p.create(); //Se crean sanos y con su x y y
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
		p.setX(pos1);
		p.setY(pos2);
		p.change_state(1);
		world[pos1][pos2]++; //Metemos a la persona en la lista de la posicion correspondiente	
		lists.push_back(p);
	}
	for (int i = 0; i < healthy; i++) {
		p.create();
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
		p.setX(pos1);
		p.setY(pos2);
		p.change_state(0);
		world[pos1][pos2]++;
		lists.push_back(p);
	}
}

void Simulator::update(int tics, int world_size, int death_duration, double infectiousness, double chance_recover) {
	for (int i = 0; i < tics - 1; i++) {
		change_world(world_size, death_duration, infectiousness, chance_recover, i);
	}
}

void Simulator::change_world(int world_size, int death_duration, double infectiousness, double chance_recover, int tic_actual) {
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
		pos1 = movePos(i, world_size);
		pos2 = movePos(j, world_size);
		(*it).setX(pos1);
		(*it).setY(pos2);
		it2 = it;
		it2++; //Arreglar
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
							cout << prob << endl;
							(*it2).change_state(1);
							(*it2).setSick(1);
						}
					}
					else if (state2 == 1) {
						prob += infectiousness;
						prob_rec = distribution(generator);
						sick_time = (*it2).getSick();
						if (prob_rec < chance_recover) {
							(*it2).change_state(2);
						}
						else if (sick_time >= death_duration) {
							(*it2).change_state(3);
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
	clear();
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

void Simulator::clear() {
	int i, j;
	for (list<Person>::iterator it = lists.begin(); it != lists.end(); ++it) {
		i = (*it).getX();
		j = (*it).getY();
		world[i][j]++;
	}
}