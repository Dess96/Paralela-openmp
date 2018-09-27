#include <stdlib.h>
#include <iostream>
#include<string>
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
	int new_sim = 1;
	int number_people;
	int sims = 1;
	string number;
	string name = " ";
	double infected;
	double infectiousness;
	double chance_recover;
	Simulator sim;
	do{
		infectiousness = -1;
		chance_recover = -1;
		number_people = -1;
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
			cout << "Ingrese el tamano del espacio bidimensional" << endl;
			cin >> world_size;
			cout << "Ingrese la cantidad de tics" << endl;
			cin >> tic;
		}
		name = "report_";
		number = to_string(sims);
		name.append(number);
		name.append(".txt");
		sim.initialize(world_size, number_people, infected);
		sim.update(tic, world_size, death_duration, infectiousness, chance_recover, name);
		cout << "¿Desea ver otra simulacion?" << endl;
		cout << "1. Si   2. No" << endl;
		cin >> new_sim;
		sims++;
		name = " ";
	}while (new_sim == 1);                               
	cin >> number_people; //Para que no se cierre
	return 0;
}

bool validateProb(double infectiousness, double chance_recover) {
	return infectiousness >= 0 && infectiousness <= 1 && chance_recover >= 0 && chance_recover <= 1;
}

bool validatePeople(int number_people) {
	return number_people >= 0 && number_people <= 100000;
}