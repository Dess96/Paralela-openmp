#pragma once
#ifndef Simulador
#define Simulador

#include<vector>
#include<list>
#include"Person.h"

using namespace std;

class Simulator {
private:
	vector<vector<int>> world;
	vector<int> v;
	list<Person> lists;
public:
	void initialize(int, int, double);
	void update(int, int, int, double, double);
	void change_world(int, int, double, double, int);
	int movePos(int, int);
	int new_state(int, int, list<Person>::iterator, double, double, int, int);
	void clear();
};

#endif // !Simulador