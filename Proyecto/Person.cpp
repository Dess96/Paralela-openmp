#include"Person.h"
//#include<iostream>

//using namespace std;

/* 0: Persona sana
* 1: Persona infectada
* 2: Persona inmune
* 3: Persona muerta*/
Person::Person() {
	this->state = 0;
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

int Person::getSick() {
	return sick_time;
}

void Person::setSick(int sick_time) {
	this->sick_time = sick_time;
}