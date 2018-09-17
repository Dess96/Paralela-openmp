#pragma once
#ifndef  Simulador
#define Simulador

#include <vector>
#include <list>
#include"Person.h"

class Simulator {
public:
	void inicialize(int, int, int, int, double, double, double);
	void update(int);
private:
	vector<vector< list<Person> > > world;
	vector<list<Person>> v;

};
#endif // ! Persona