#pragma once
#ifndef Simulador
#define Simulador

#include<vector>
#include<list>
#include"Person.h"

using namespace std;

class Simulator {
private:
	vector<vector<int>> world; //Vector de vectores de enteros de la stl. Tiene la cantidad de personas en determinada posicion
	vector<int> v;
	vector<Person> peopleVec; //Vector que contiene a las personas
	double prob; //Probabilidad de infeccion por posicion
public:
	int initialize(int, double, double, int, double, int, int, int); //Inicializa el mundo y los parametros
	void update(string, int); //Actualiza el mundo por tic
	int movePos(int, int); //Mueve a la persona de posicion
	void clear(int, string); //Crea el archivo
	void changeState(int); //Cambia el estado de las personas
	void checkVec(int, int, int); //Chequea si hay mas de una persona en la misma posicion
};

#endif // !Simulador