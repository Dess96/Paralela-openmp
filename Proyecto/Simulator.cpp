#include"Simulator.h"
#include"Person.h"
#include<iostream>
#include<time.h>
#include<stdlib.h>

using namespace std;

void Simulator::initialize(int world_size, int number_people, int death_duration, int tic, double infectiousness, double chance_recover, double infected) {
	Person p, p1;
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
//		cout << world[pos1][pos2].back().getState() << endl;
	}
}