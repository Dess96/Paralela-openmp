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
	vector<Person> peopleVec;
	double prob;
public:
	int initialize(int, double, double, int, double, int, int);
	void update(string, int);
	int movePos(int, int);
	void clear(int, string);
	void changeState(int);
	void check_vec(int, int, int);
};

#endif // !Simulador