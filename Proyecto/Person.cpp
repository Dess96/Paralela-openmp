#include"Person.h"
//#include<iostream>

//using namespace std;

/* 0: Persona sana
* 1: Persona infectada
* 2: Persona inmune
* 3: Persona muerta*/
void Person::create() {
	Person();
}

void Person::change_state(int state) {
	this->state = state;
}

int Person::getX() {
	return x;
}

int Person::getY() {
	return y;
}

void Person::setY(int y) {
	this->y = y;
}

void Person::setX(int x) {
	this->x = x;
}

int Person::getState() {
	return state;
}