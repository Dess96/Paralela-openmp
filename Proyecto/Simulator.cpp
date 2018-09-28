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
int world_size, death_duration, tic;
double infected, infectiousness, chance_recover, number_people;

void Simulator::initialize(int number_peopleM, double infectiousnessM, double chance_recoverM, int death_durationM, double infectedM, int world_sizeM, int ticM) {
	number_people = number_peopleM;
	infectiousness = infectiousnessM;
	chance_recover = chance_recoverM;
	death_duration = death_durationM;
	infected = infectedM;
	world_size = world_sizeM;
	tic = ticM;
	Person p;
	v.resize(world_size); //Vector de vectores de tamaño world_size*world_size
	world.resize(world_size, v);
	int perc;
	int healthy;
	int pos1, pos2;
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
		p.setSick(1);
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

void Simulator::update(string name) {
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
				}
			}
			else if (state == 1) {
				prob += infectiousness;
				prob_rec = distribution(generator);
				sick_time = (*it).getSick();
				if (prob_rec < chance_recover) {
					(*it).change_state(2);
				}
				else if (sick_time >= death_duration) {
					(*it).change_state(3);
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
		clear(actual_tic, name);
	}
	lists.clear();
	world.clear();
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

void Simulator::clear(int actual_tic, string name) {
	int i, j, state;
	int sick_people = 0;
	int dead_people = 0;
	int healthy_people = 0;
	int inmune_people = 0;
	list<Person>::iterator it = lists.begin();
	while (it != lists.end()) {
		i = (*it).getX();
		j = (*it).getY();
		state = (*it).getState();
		if (state == 1) {
			sick_people++;
			++it;
		}
		else if (state == 2) {
			inmune_people++;
			++it;
		}
		else if (state == 3) {
			dead_people++;
			lists.erase(it++);
		}
		else if (state == 0) {
			healthy_people++;
			++it;
		}
		world[i][j]++;
	}
	ofstream file;
	file.open(name, ios_base::app);
	file << "Reporte del tic " << actual_tic << " Personas muertas " << dead_people << " Personas sanas " << healthy_people << " Personas enfermas " << sick_people << " Personas inmunes " << inmune_people << endl;

	file.close();//Hacer archivo
}
