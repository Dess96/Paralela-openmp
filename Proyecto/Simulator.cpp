#include <stdlib.h>
#include <iostream>
#include<omp.h>
#include"Person.h"
#include"Simulator.h"
using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

void Simulator::inicialize(int world_size, int number_people, int death_duration, int tic, double infectiousness, double chance_recover, double infected) {
	Person p;
	int perc;
	v.resize(world_size);
	world.resize(world_size, v);
	perc = number_people * infected / 100;
	for (int i = 0; i < perc; i++) {
		p.assign(world_size);
	}
//	p.assign(world_size);

}

void Simulator::update(int tics) {

}
