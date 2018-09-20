#pragma once
#ifndef Simulador
#define Simulador

#include<vector>
#include<list>
#include"Person.h"

using namespace std;

class Simulator {
private:
	vector<vector< list<Person> > > world;
	vector<list<Person>> v;
public:
	void initialize(int, int, int, int, double, double, double);
	void update(int);
	void change_world(int, int, int, int, double, double, double);
};

#endif // !Simulador
