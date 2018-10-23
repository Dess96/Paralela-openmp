#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono> // para medir el tiempo de ejecución
#include <omp.h> 
#include<iomanip>
using namespace std::chrono;
using namespace std;

void leeAdyacencias(ifstream& ae, vector< vector< int > >& ma, int& cntVertices);
void algoritmoFloydWarshall(const vector< vector< int > >& ma, vector< vector< int > >& mc, int& cntVertices);
void algoritmoFloydWarshall_v2(const vector< vector< int > >& ma, vector< vector< int > >& mc, int& cntVertices);
void algoritmoFloydWarshall_v3(const vector< vector< int > >& ma, vector< vector< int > >& mc, int& cntVertices);


int main() {
	string nombreArchivoEntrada = "gmediano.txt"; // formato *.txt, por ejemplo "grafo.txt
	string nombreArchivoSalida;  // formato *.txt, por ejemplo matriz_costos.txt 
	ifstream archivoEntrada(nombreArchivoEntrada, ios::in);
	ofstream archivoSalida(nombreArchivoSalida, ios::out);
	vector< vector< int > > matrizAdyacencias;
	vector< vector< int > > matrizCostos;
	int cntVertices;

	if (!archivoEntrada) { // operador ! sobrecargado
		cout << "No se pudo abrir el archivo de entrada" << endl;
		cin.ignore();
		return 1;
	}

	// se cargan y despliegan las adyacencias del archivo
	leeAdyacencias(archivoEntrada, matrizAdyacencias, cntVertices);
/*	cout << "MATRIZ DE ADYACENCIAS" << endl;
	for (int i = 0; i < cntVertices; i++) {
		for (int j = 0; j < cntVertices; j++)
			cout << matrizAdyacencias[i][j] << ',';
		cout << endl;
	}*/

	// se toma una marca de tiempo:
	using namespace std::chrono;
	steady_clock::time_point t1 = steady_clock::now();

	// se genera la matriz de costos
	algoritmoFloydWarshall(matrizAdyacencias, matrizCostos, cntVertices);
	algoritmoFloydWarshall_v2(matrizAdyacencias, matrizCostos, cntVertices);
	algoritmoFloydWarshall_v3(matrizAdyacencias, matrizCostos, cntVertices);

	// se toma otra marca de tiempo
	steady_clock::time_point t2 = steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	// se despliega la duración:
	std::cout << endl << "Duracion " << time_span.count() << " segundos.";
	std::cout << std::endl;

	// se despliega la matriz de costos:
/*	cout << endl << "MATRIZ DE COSTOS" << endl;
	for (int i = 0; i < cntVertices; i++) {
		for (int j = 0; j < cntVertices; j++)
			cout << matrizCostos[i][j] << ',';
		cout << endl;
	}*/
	cin.ignore(); // para que la consola no se cierre sin ver los resultados al ejecutar desde Visual Studio
}

void leeAdyacencias(ifstream& ae, vector< vector< int > >& ma, int& cntVertices) {
	int pe;
	char finLinea = ' ';
	int contadorLineas = 0;

	ae >> cntVertices; // el primer número del archivo es la cantidad de vértices
	vector< int > v;
	v.resize(cntVertices, INT_MAX);
	ma.resize(cntVertices, v);

	ae.get(); // consume un blanco
	finLinea = ae.peek(); // intenta leer fin de línea

						  // salta a la siguiente para leer el primer número de la línea #2
	ae >> pe;
	for (int i = 0; i < cntVertices; i++) {
		while (!ae.eof() && (finLinea != '\n')) { // 10 ascii de fin de línea
			ma[i][pe] = 1;
			ae >> pe;
			ae.get(); // consume un blanco
			finLinea = ae.peek(); // intenta leer fin de línea
		}
		if (!ae.eof())
			ma[i][pe] = 1;
		ae >> pe;
		ae.get();
		finLinea = ae.peek();
	}
}

void algoritmoFloydWarshall(const vector< vector< int > >& ma, vector< vector< int > >& mc, int& cntVertices) {
	int pe;
	char finLinea = ' ';
	int contadorLineas = 0;
	ifstream ae;
	
	ae >> cntVertices; // el primer número del archivo es la cantidad de vértices
	vector< int > v;
	v.resize(cntVertices, INT_MAX);
	mc.resize(cntVertices, v);

	ae.get(); // consume un blanco
	finLinea = ae.peek(); // intenta leer fin de línea

						  // salta a la siguiente para leer el primer número de la línea #2
	ae >> pe;
	for (int i = 0; i < cntVertices; i++) {
		for (int j = 0; j < cntVertices; j++) {
			mc[i][j] = ma[i][j];
		}
	}
	steady_clock::time_point t1 = steady_clock::now();
#pragma omp parallel for num_threads(2)
	for (int k = 1; k < cntVertices; k++) {
		for (int i = 1; i < cntVertices; i++) {
			for (int j = 1; j < cntVertices; j++) {
				if ((mc[i][k] != 2147483647) && (mc[k][j] != 2147483647)) {
					if (mc[i][j] > mc[i][k] + mc[k][j]) {
						mc[i][j] = mc[i][k] + mc[k][j];
					}
				}
			}
		}
	}
	steady_clock::time_point t2 = steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	std::cout << "Duracion version 1 " << fixed << std::setprecision(12) << (double)time_span.count() << " segundos.";
	std::cout << std::endl;
}

void algoritmoFloydWarshall_v2(const vector< vector< int > >& ma, vector< vector< int > >& mc, int& cntVertices) {
	int pe;
	char finLinea = ' ';
	int contadorLineas = 0;
	ifstream ae;

	ae >> cntVertices; // el primer número del archivo es la cantidad de vértices
	vector< int > v;
	v.resize(cntVertices, INT_MAX);
	mc.resize(cntVertices, v);

	ae.get(); // consume un blanco
	finLinea = ae.peek(); // intenta leer fin de línea

						  // salta a la siguiente para leer el primer número de la línea #2
	ae >> pe;
	for (int i = 0; i < cntVertices; i++) {
		for (int j = 0; j < cntVertices; j++) {
			mc[i][j] = ma[i][j];
		}
	}
	steady_clock::time_point t1 = steady_clock::now();
#pragma omp_set_nested(2)
#pragma omp parallel num_threads(2)
	for (int k = 1; k < cntVertices; k++) {
#pragma omp parallel num_threads(2)
		for (int i = 1; i < cntVertices; i++) {
			for (int j = 1; j < cntVertices; j++) {
				if ((mc[i][k] != 2147483647) && (mc[k][j] != 2147483647)) {
					if (mc[i][j] > mc[i][k] + mc[k][j]) {
						mc[i][j] = mc[i][k] + mc[k][j];
					}
				}
			}
		}
	}
	steady_clock::time_point t2 = steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	std::cout << "Duracion version 2 " << fixed << std::setprecision(12) << (double)time_span.count() << " segundos.";
	std::cout << std::endl;
}

void algoritmoFloydWarshall_v3(const vector< vector< int > >& ma, vector< vector< int > >& mc, int& cntVertices) {
	int pe;
	char finLinea = ' ';
	int contadorLineas = 0;
	ifstream ae;

	ae >> cntVertices; // el primer número del archivo es la cantidad de vértices
	vector< int > v;
	v.resize(cntVertices, INT_MAX);
	mc.resize(cntVertices, v);

	ae.get(); // consume un blanco
	finLinea = ae.peek(); // intenta leer fin de línea

						  // salta a la siguiente para leer el primer número de la línea #2
	ae >> pe;
	for (int i = 0; i < cntVertices; i++) {
		for (int j = 0; j < cntVertices; j++) {
			mc[i][j] = ma[i][j];
		}
	}
	steady_clock::time_point t1 = steady_clock::now();
#pragma omp_set_nested(3)
#pragma omp parallel for num_threads(2)
	for (int k = 1; k < cntVertices; k++) {
#pragma omp parallel for num_threads(2)
		for (int i = 1; i < cntVertices; i++) {
#pragma omp parallel for num_threads(2)
			for (int j = 1; j < cntVertices; j++) {
				if ((mc[i][k] != 2147483647) && (mc[k][j] != 2147483647)) {
					if (mc[i][j] > mc[i][k] + mc[k][j]) {
						mc[i][j] = mc[i][k] + mc[k][j];
					}
				}
			}
		}
	}
	steady_clock::time_point t2 = steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	std::cout << "Duracion version 2 " << fixed << std::setprecision(12) << (double)time_span.count() << " segundos.";
	std::cout << std::endl;
}