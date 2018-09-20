#include"Simulator.h"
#include"Person.h"
#include<iostream>
#include<time.h>
#include<stdlib.h>

using namespace std;

/* world_size: Tamaño del espacio bidimensional
* number_people: Cantidad total de personas
* death_duration: 
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
	//***************************************************************************************************************************************//
	//Va aqui?
	 //Aquí nos encargamos de actualizar el mundo "tic" veces.
	for (int i = 0; i < tic; i++) {
		//Aquí recorremos todos los espacios en el mundo en el tiempo n para modificarlo utilizando 
		//una estructura auxiliar como mundo en el tiempo n+1
		for (int j = 0; j < world_size; j++) {
			for (int k = 0; k < world_size; k++) {
				if (!(world[j][k].empty())) {
					//mueve a la persona una posición hacia cualquier dirección.  Podríamos hacerlo método
					movement = rand() % 8;
					if (movement == 0) {
						pos1 = (pos1 = world_size - 1) ? 0 : pos1 + 1; //DERECHA
					}
					else if (movement == 1) {
						pos1 = (pos1 = world_size - 1) ? 0 : pos1 + 1; //DIAGONAL INFERIOR DERECHA
						pos2 = (pos2 = world_size - 1) ? 0 : pos2 + 1; 
					}
					else if (movement == 2) {
						pos2 = (pos2 = world_size - 1) ? 0 : pos2 + 1; //ABAJO
					}
					else if (movement == 3) {
						pos1 = (pos1 = 0) ? world_size - 1 : pos1 - 1; //DIAGONAL INFERIOR IZQUIERDA
						pos2 = (pos2 = world_size - 1) ? 0 : pos2 + 1;
					}
					else if (movement == 4) {
						pos1 = (pos1 = 0) ? world_size - 1 : pos1 - 1; //IZQUIERDA
					}
					else if (movement == 5) {
						pos1 = (pos1 = 0) ? world_size - 1 : pos1 - 1; //DIAGONAL SUPERIOR IZQUIERDA
						pos2 = (pos2 = 0) ? world_size - 1 : pos2 - 1;
					}
					else if (movement == 6) {
						pos2 = (pos2 = 0) ? world_size - 1 : pos2 - 1; //ARRIBA
					}
					else {
						pos1 = (pos1 = world_size - 1) ? 0 : pos1 + 1; //DIAGONAL SUPERIOR DERECHA
						pos2 = (pos2 = 0) ? world_size - 1 : pos2 - 1;
					}
				//	world[j][k].setX(pos1);
				//	p.setY(pos2);
					//world2[pos1][pos2].push_back(p);
				}
			}
		} //cierra ciclo recorrido de mundos
	} //cierra recorrido de "tic" veces
}

void Simulator::change_world(int world_size, int number_people, int death_duration, int tic, double infectiousness, double chance_recover, double infected) {
	int state;
	double prob;
	list<Person>::iterator it;
	Person p;
	for (int i = 0; i < world_size; i++) {
		for (int j = 0; j < world_size; j++) {
			prob = 0;
			if(!(world[i][j].empty())) {
				for (list<Person>::iterator it = world[i][j].begin(); it != world[i][j].end(); ++it) {
					p = *it;
					state = p.getState();
					if (state == 1) {
						prob += infectiousness;
					}/*else if{
					 ????
					}*/
				}
			}
		}
	}
}
