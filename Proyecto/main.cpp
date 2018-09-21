#include <stdlib.h>
#include <iostream>
#include<omp.h>
#include"Person.h"
#include"Simulator.h"
using namespace std;


#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

bool validateProb(double, double);
bool validatePeople(int);

int main(int argc, char * argv[]) {
	int world_size, death_duration, tic;
	int number_people = -1;
	double infected;
	double infectiousness = -1;
	double chance_recover = -1;
	Simulator sim;
	while (!validateProb(infectiousness, chance_recover) || !validatePeople(number_people)) {
		cout << "Ingrese el numero de personas en el mundo (de 0 a 10 000)" << endl;
		cin >> number_people;
		cout << "Ingrese la potencia infecciosa del mundo (decimal entre 0 y 1)" << endl;
		cin >> infectiousness;
		cout << "Ingrese la probabilidad de recuperacion (decimal entre 0 y 1)" << endl;
		cin >> chance_recover;
		cout << "Ingrese el tiempo maximo que puede vivir una persona infectada" << endl;
		cin >> death_duration;
		cout << "Ingrese el porcentaje de personas infectadas inicialmente" << endl;
		cin >> infected;
		cout << "Ingrese el tamaño del espacio bidimensional" << endl;
		cin >> world_size;
		cout << "Ingrese la cantidad de tics" << endl;
		cin >> tic;
	}
	sim.initialize(world_size, number_people, infected);
	sim.update(tic, world_size, death_duration, infectiousness, chance_recover);

	cin >> number_people; //Para que no se cierre
	return 0;
}

bool validateProb(double infectiousness, double chance_recover) {
	return infectiousness >= 0 && infectiousness <= 1 && chance_recover >= 0 && chance_recover <= 1;
}

bool validatePeople(int number_people) {
	return number_people >= 0 && number_people <= 100000;
}