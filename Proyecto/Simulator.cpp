#include"Simulator.h"
#include"Person.h"
#include<iostream>
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
	for (int i = 0; i < healthy; i++) {
		p.create();
		pos1 = rand() % world_size;
		pos2 = rand() % world_size;
		p.setX(pos1);
		p.setY(pos2);
		world[pos1][pos2]++;
		lists.push_back(p);
	}
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
/*	for (list<Person>::iterator it = lists.begin(); it != lists.end(); ++it) {
		cout << "Pos X"<<(*it).getX()<<"Pos Y"<< (*it).getY() <<"Estado"<<(*it).getState()<< endl;
	}*/
	for (int i = 0; i < world_size; i++) {
		for (int j = 0; j < world_size; j++) {
			cout<<world[i][j]<<" ";
		}
		cout << endl;
	}
}

void Simulator::update(int tics, int world_size, int death_duration, double infectiousness, double chance_recover) {
	for (int i = 0; i < tics-1; i++) {
		change_world(world_size, death_duration, infectiousness, chance_recover, i);
	}
}

void Simulator::change_world(int world_size, int death_duration, double infectiousness, double chance_recover, int tic_actual) {
	srand(time(NULL));
	double prob_rec, prob_infect;
	int state, state2, i, j, i2, j2, pos1, pos2;
	double prob;
	list<Person>::iterator it;
	list<Person>::iterator it2;
	for (list<Person>::iterator it = lists.begin(); it != lists.end(); ++it) {
		prob = 0;
		i = (*it).getX();
		j = (*it).getY();
		//Cambiar estado si se le debe cambiar el estado
		state = (*it).getState();
		prob = new_state(prob, state, it, infectiousness, chance_recover, death_duration,tic_actual);
		pos1 = movePos(i, world_size);
		pos2 = movePos(j, world_size);
		(*it).setX(pos1);
		(*it).setY(pos2);
		it2 = it;
		it2++; //Arreglar
		if(world[i][j] > 0){
			world[i][j]--;
			while ((world[i][j] > 0) && (it2 != lists.end())) {
				i2 = (*it2).getX();
				j2 = (*it2).getY();
				if (i == i2 && j == j2) {
					world[i][j]--;
					state2 = (*it2).getState();
					prob = new_state(prob, state2, it2, infectiousness, chance_recover, death_duration, tic_actual);
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
/*		cout << "Despues de enfermar" << endl;
		for (list<Person>::iterator it = lists.begin(); it != lists.end(); ++it) {
			cout << "Pos X" << (*it).getX() << "Pos Y" << (*it).getY() << "Estado" << (*it).getState() << endl;
		}*/
	cout << "Matriz nueva" << endl;
	for (int k = 0; k < world_size; k++) {
		for (int l = 0; l < world_size; l++) {
			cout<<world[k][l]<<" ";
		}
		cout<<endl;
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

int Simulator::new_state(int prob, int state, list<Person>::iterator it, double infectiousness, double chance_recover, int death_duration, int tic_actual) {
	int prob_rec, prob_infect;
	int death_people = 0;
	int healthy_people = 0;
	int sick_people = 0;
	int inmune_people = 0;
	if (state == 1) { //Si la persona esta infectada
		prob_rec = rand() % 100000;
		prob_rec /= 100000;
		prob += infectiousness; //Aumenta el chance de que un sano se infecte
		if (tic_actual >= death_duration) { //Si la persona sigue enferma despues del tiempo asignado, muere
			death_people++;
		}
		else if (prob_rec <= chance_recover) { //Determina si la persona va a poder recuperarse
			(*it).change_state(2);
			inmune_people++;
		}
		else {
			sick_people++;
		}
	}
	else if (state == 0) {
		prob_infect = rand() % 100000;
		prob_infect /= 100000;
		if (prob_infect <= prob) { //Persona se inferma 
			(*it).change_state(1);
			sick_people++;
		}
		else {
			healthy_people++;
		}
	}
	return prob;
}

void Simulator::clear() {
	int i, j;
	for (list<Person>::iterator it = lists.begin(); it != lists.end(); ++it) {
		i = (*it).getX();
		j = (*it).getY();
		world[i][j]++;
	}
}