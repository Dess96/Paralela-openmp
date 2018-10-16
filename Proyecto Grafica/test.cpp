#include<iostream>
#include<string>
#include<omp.h>
#include<thread>
#include<chrono>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include"Person.h"
#include"Simulator.h"

using namespace std;
using namespace std::chrono;

#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

bool validateProb(double, double);
bool validatePeople(int);
void display();

int main(int argc, char *argv[]) {
	unsigned n = std::thread::hardware_concurrency(); //Saca la cantidad de nucleos en la computadora
	int thread_countM = 2 * n;
	int world_sizeM, death_durationM, ticM, number_peopleM, healthy_people;
	int new_sim = 1;
	int sims = 1;
	double infectedM, infectiousnessM, chance_recoverM, arch_time;
	string number;
	string name = " ";
	Simulator sim;
	do {
		infectiousnessM = -1;
		chance_recoverM = -1;
		number_peopleM = -1;
		while (!validateProb(infectiousnessM, chance_recoverM) || !validatePeople(number_peopleM)) { //Pedir y validar datos
			cout << "Ingrese el numero de personas en el mundo (de 0 a 10 000)" << endl;
			cin >> number_peopleM;
			cout << "Ingrese la potencia infecciosa del mundo (decimal entre 0 y 1)" << endl;
			cin >> infectiousnessM;
			cout << "Ingrese la probabilidad de recuperacion (decimal entre 0 y 1)" << endl;
			cin >> chance_recoverM;
			cout << "Ingrese el tiempo maximo que puede vivir una persona infectada" << endl;
			cin >> death_durationM;
			cout << "Ingrese el porcentaje de personas infectadas inicialmente" << endl;
			cin >> infectedM;
			cout << "Ingrese el tamano del espacio bidimensional" << endl;
			cin >> world_sizeM;
			cout << "Ingrese la cantidad de tics" << endl;
			cin >> ticM;
		}
		cout << "Se usaran " << thread_countM << " hilos en esta simulacion" << endl;
		name = "report_"; //Nos encargamos de crear el nombre del futuro archivo por simulacion
		number = to_string(sims);
		name.append(number);
		name.append(".txt");
		healthy_people = sim.initialize(number_peopleM, infectiousnessM, chance_recoverM, death_durationM, infectedM, world_sizeM, ticM, thread_countM); //Metodo inicializador
		arch_time = sim.update(name, healthy_people); //Metodo que actualiza el mundo por tic
		//GRAFICACION
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB);
		glutInitWindowSize(640, 480);
		glutCreateWindow("Data");
		glutDisplayFunc(display);
		glutMainLoop();
		//sim.init_resources();
		cout << endl;
		cout << "Desea ver otra simulacion?" << endl;
		cout << "1. Si   2. No" << endl;
		cin >> new_sim;
		sims++;
		name = " ";
	} while (new_sim == 1);
	cin >> number_peopleM; //Para que no se cierre
	return 0;
}

bool validateProb(double infectiousness, double chance_recover) {
	return infectiousness >= 0 && infectiousness <= 1 && chance_recover >= 0 && chance_recover <= 1;
}

bool validatePeople(int number_people) {
	return number_people >= 0 && number_people <= 100000;
}

void display() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	double msg;
	double coordx = -1.0;
	double coordy = -1.0;
	int rank;
	int i = 0;
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(2);
	glVertex2f(-1.0, -1.0);
	while (i < 20) {
		coordx += 0.3;
		coordy += 0.1;
		glVertex2f(coordx, coordy);
		i++;
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
}
