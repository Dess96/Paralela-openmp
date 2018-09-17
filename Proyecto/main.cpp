#include <stdlib.h>
#include <iostream>
#include<vector>
#include<omp.h>
#include"Person.h"
#include"Simulator.h"
using namespace std;


#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

int main(int argc, char * argv[]) {
	int number_people, world_size, death_duration, tic;
	double infectiousness, chance_recover, infected;
	cout << "Ingrese el numero de personas en el mundo (de 0 a 10 000)" << endl;
	cin >> number_people;
	cout << "Ingrese la potencia infecciosa del mundo (decimal entre 0 y 1)" << endl;
	cin >> infectiousness;
	cout << "Ingrese la probabilidad de recuperacion (decimal entre 0 y 1)" << endl;
	cin >> chance_recover;
	cout << "Ingrese el tiempo maximo que puede vivir una persona infectadad" << endl;
	cin >> death_duration;
	cout << "Ingrese el porcentaje de personas infectadas inicialmente" << endl;
	cin >> infected;
	cout << "Ingrese el tamaño del espacio bidimensional" << endl;
	cin >> world_size;
	cout << "Ingrese la cantidad de tics" << endl;
	cin >> tic;

	cin >> number_people; //Para que no se cierre
	return 0;
}



