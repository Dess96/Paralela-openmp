#pragma once
#ifndef Persona
#define Persona

class Person {
private:
	int x;
	int y;
	int state;
	int sick_time;
public:
	Person();
	void change_state(int);
	int getX();
	int getY();
	void setY(int);
	void setX(int);
	int getState();
	int getSick();
	void setSick(int);
};
#endif // !Persona