#include <stdlib.h>
#include <iostream>
#include<vector>
#include<omp.h>
#include<time>
#include"Person.h"

using namespace std;


#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

void assign(int world_size) {
	int random;
	int x, y;
	srand(time(NULL));
	x = rand() % world_size;
	y = rand() % world_size;
}