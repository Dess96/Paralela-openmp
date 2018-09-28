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
	void initialize(int, double, double, int, double, int, int);
	void update(string);
	int movePos(int, int);
	void clear(int, string);
};

#endif // !Simulador