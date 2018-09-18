#pragma once
#ifndef Persona
#define Persona

class Person {
private:
	int x;
	int y;
	int state;
public:
	void create();
	void change_state(int);
	int getX();
	int getY();
	void setY(int);
	void setX(int);
	int getState();
};
#endif // !Persona
