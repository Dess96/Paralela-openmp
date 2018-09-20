#include"Simulator.h"
#include"Person.h"
#include<iostream>
#include<time.h>
#include<stdlib.h>

using namespace std;

/* world_size: Tamaño del espacio bidimensional
* number_people: Cantidad total de personas
* death_duration: Cuanto pasa antes de que se mueran las personas
* tic: Cantidad de tics que dura la simulacion
* infectiousness: Potencia infecciosa
* chance_recover: Probabilidad de recuperacion
* infected: Personas infectadas inicialmente*/
void Simulator::initialize(int world_size, int number_people, int death_duration, int tic, double infectiousness, double chance_recover, double infected) {
	Person p;
	v.resize(world_size); //Vector de vectores de tamaño world_size*world_size
	world.resize(world_size, v);
	int perc;
	int pos1, pos2;
	int healthy;
	int movement = 0;
	perc = number_people * infected / 100; //Cantidad correspondiente al porcentaje dado
	healthy = number_people - perc; //Gente sana
	srand(time(NULL));
	for (int i = 0; i < healthy; i++) {
		p.create();
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
		p.setX(pos1);
		p.setY(pos2);
		world[pos1][pos2].push_back(p); //Metemos a la persona en la lista de la posicion correspondiente
										//		cout << world[pos1][pos2].back().getState() << endl;
	}
	for (int i = 0; i < perc; i++) { //Cambiamos a los infectados
		p.create(); //Se crean sanos y con su x y y
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
		p.setX(pos1);
		p.setY(pos2);
		p.change_state(1);
		world[pos1][pos2].push_back(p); //Metemos a la persona en la lista de la posicion correspondiente	
	}
}

void Simulator::change_world(int world_size, int death_duration, double infectiousness, double chance_recover, int tic_actual, int death_people, int healthy_people, int inmune_people, int infected_people) {
	srand(time(NULL));
	int state;
	double prob;
	double prob_infect, prob_rec;
	list<Person>::iterator it;
	Person p;
	for (int i = 0; i < world_size; i++) {
		for (int j = 0; j < world_size; j++) {
			prob = 0;
			if (!(world[i][j].empty())) {
				for (list<Person>::iterator it = world[i][j].begin(); it != world[i][j].end(); ++it) {
					p = *it;
					state = p.getState();
					if (state == 1) { //Si la persona esta infectada
						prob_rec = rand() % 100000;
						prob_rec /= 100000;
						prob += infectiousness; //Aumenta el chance de que un sano se infecte
						if (tic_actual >= death_duration) { //Si la persona sigue enferma despues del tiempo asignado, muere
							world[i][j].erase(it);
							death_people++;
						}
						else if (prob_rec <= chance_recover) { //Determina si la persona va a poder recuperarse
							p.change_state(2);
						}
					}
					else if (state == 0) {
						prob_infect = rand() % 100000;
						prob_infect /= 100000;
						if (prob_infect <= prob) { //Persona se inferma 
							p.change_state(1); 
						}
					}
				}
			}
		}
	}
}

void Simulator::update(int tics) {

}