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
	void clear(int);
	void update(int, int, int, double, double);
	void change_world(int, int, double, double, int);
	int movePos(int, int);
};

#endif // !Simulador