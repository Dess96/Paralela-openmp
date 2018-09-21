#include"Simulator.h"
#include"Person.h"
#include<iostream>
#include<time.h>
#include <fstream>
#include<stdlib.h>

using namespace std;

/* world_size: Tama�o del espacio bidimensional
* number_people: Cantidad total de personas
* death_duration: Cuanto pasa antes de que se mueran las personas
* tic: Cantidad de tics que dura la simulacion
* infectiousness: Potencia infecciosa
* chance_recover: Probabilidad de recuperacion
* infected: Personas infectadas inicialmente*/
void Simulator::initialize(int world_size, int number_people, double infected) {
	Person p;
	v.resize(world_size); //Vector de vectores de tama�o world_size*world_size
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
	cout << "Posiciones iniciales" << endl;
	for (int i = 0; i < world_size; i++) {
		for (int j = 0; j < world_size; j++) {
			if (!(world[i][j].empty())) {
				for (list<Person>::iterator it = world[i][j].begin(); it != world[i][j].end(); ++it) {
					p = *it;
					cout << "Esta persona de estado " << p.getState() << "estaba en la posicion x " << p.getX() << "y y " << p.getY()<<endl;
				}
			}
		}
	}
}

void Simulator::update(int tics, int world_size, int death_duration, double infectiousness, double chance_recover) {
	for (int i = 0; i < tics; i++) {
		change_world(world_size, death_duration, infectiousness, chance_recover, i);
	}
}

void Simulator::change_world(int world_size, int death_duration, double infectiousness, double chance_recover, int tic_actual) {
	srand(time(NULL));
	int state, pos1, pos2;
	int death_people = 0;
	int healthy_people = 0;
	int inmune_people = 0;
	int sick_people = 0;
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
					pos1 = movePos(i, world_size);
					pos2 = movePos(j, world_size);
					p.setX(pos1);
					p.setY(pos2);
					world[pos1][pos2].push_back(p);
				}
			}
		}
	}
	ofstream file;
	file.open("report.txt");
	file << "Personas que murieron en el tic " << tic_actual << ": "
		<< death_people << "\n Personas sanas en el tic " << tic_actual << ": " << healthy_people << "\n Personas infectadas en el tic: " << tic_actual << ": "
		<< sick_people << "\n Personas inmunes en el tic: " << tic_actual << ": " << inmune_people;
	file.close();//Hacer archivo
	cout << "Estados cambiados" << endl;
	for (int i = 0; i < world_size; i++) {
		for (int j = 0; j < world_size; j++) {
			if (!(world[i][j].empty())) {
				for (list<Person>::iterator it = world[i][j].begin(); it != world[i][j].end(); ++it) {
					p = *it;
					cout << "Ahora tiene estado " << p.getState() << "ahora esta en la posicion x " << p.getX() << "y y " << p.getY() << endl;
				}
			}
		}
	}
}

int Simulator::movePos(int pos, int world_size) {
	srand(time(NULL));
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

