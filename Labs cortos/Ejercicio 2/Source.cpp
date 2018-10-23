#include<stdlib.h>
#include<iostream>
#include<omp.h>
#include<time.h>
#include<math.h>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

int main(int argc, char* argv[]) {
	int thread_count = 0;
	double intentos = 0.0;
	double x, y, distancia;
	double estimado;
	int aciertos = 0;
	srand(time(NULL));
	cout << "Digite la cantidad de hilos >= 1" << endl;
	cin >> thread_count;
	cout << "Digite la cantidad de intentos >= 1" << endl;
	cin >> intentos;
	if (thread_count < 1 || intentos < 1) {
		cout << "Error. Digite un numero mayor o igual a 1" << endl;
	}
	
#pragma omp parallel for num_threads(thread_count) shared(aciertos) private(x, y, distancia)
	{
		for (int i = 0; i < intentos; i++) {
			x = rand()*(-1.0) / RAND_MAX + (1.0);
			y = rand()*(-1.0) / RAND_MAX + (1.0);
			distancia = x * x + y * y;
			if (distancia <= 1) {
//Conflictos para modificar variable aciertos
#pragma omp critical 
				aciertos++;
			}
		}
	}
	estimado = 4 * aciertos / intentos;
	cout << estimado << " Estimado con MonteCarlo" << endl;
	cout << 4.0*atan(1.0) << " Estimado con formula" << endl;
	cin >> thread_count;
	return 0;
}

