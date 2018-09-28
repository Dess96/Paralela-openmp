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
	int world_sizeM, death_durationM, ticM, number_peopleM;
	int new_sim = 1;
	int healthy_people;
	int sims = 1;
	string number;
	string name = " ";
	double infectedM, infectiousnessM, chance_recoverM;
	Simulator sim;
	do {
		infectiousnessM = -1;
		chance_recoverM = -1;
		number_peopleM = -1;
		while (!validateProb(infectiousnessM, chance_recoverM) || !validatePeople(number_peopleM)) {
			cout << "Ingrese el numero de personas en el mundo (de 0 a 10 000)" << endl;
			cin >> number_peopleM;
			cout << "Ingrese la potencia infecciosa del mundo (decimal entre 0 y 1)" << endl;
			cin >> infectiousnessM;
			cout << "Ingrese la probabilidad de recuperacion (decimal entre 0 y 1)" << endl;
			cin >> chance_recoverM;
			cout << "Ingrese el tiempo maximo que puede vivir una persona infectada" << endl;
			cin >> death_durationM;
			cout << "Ingrese el porcentaje de personas infectadas inicialmente" << endl;
			cin >> infectedM;
			cout << "Ingrese el tamano del espacio bidimensional" << endl;
			cin >> world_sizeM;
			cout << "Ingrese la cantidad de tics" << endl;
			cin >> ticM;
		}
		name = "report_";
		number = to_string(sims);
		name.append(number);
		name.append(".txt");
		sim.initialize(number_peopleM, infectiousnessM, chance_recoverM, death_durationM, infectedM, world_sizeM, ticM);
		sim.update(name);
		cout << "Desea ver otra simulacion?" << endl;
		cout << "1. Si   2. No" << endl;
		cin >> new_sim;
		sims++;
		name = " ";
	} while (new_sim == 1);
	cin >> number_peopleM; //Para que no se cierre
	return 0;
}

bool validateProb(double infectiousness, double chance_recover) {
	return infectiousness >= 0 && infectiousness <= 1 && chance_recover >= 0 && chance_recover <= 1;
}

bool validatePeople(int number_people) {
	return number_people >= 0 && number_people <= 100000;
}