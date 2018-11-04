#include<stdlib.h>
#include<iostream>
#include<time.h>
#include<omp.h>
#include<vector>
#include<algorithm>
#include<exception>
#include<chrono>
#include <iomanip> 
using namespace std::chrono;

using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

std::vector<int> arreglo;
void generate(int, int);
void mergeSort(int, int);
void merge(int, int);
void merge_v2(int, int, int);

int main(int argc, char* argv[]) {

	int thread_count = 0;
	int cant = 0;
	cout << "Ingrese la cantidad de hilos >= 1" << endl;
	cin >> thread_count;
	cout << "Ingrese la cantidad de numeros > 1" << endl;
	cin >> cant;

	generate(cant, thread_count);
	mergeSort(cant, thread_count);
	cin >> thread_count; //Para que no se cierre
	return 0;
}

void generate(int cant, int thread_count) {
	int random;
	srand(time(NULL));
	arreglo.resize(cant);
	for (int i = 0; i < cant; i++) {
		random = rand();
		arreglo[i] = random;
	}
}

void mergeSort(int cant, int thread_count) {
	int quantity = cant / thread_count;
	int rango;
	vector<int>::iterator it;
#pragma omp parallel num_threads(thread_count)
#pragma omp critical
	for (int i = 0; i < cant; i++) {
		rango = omp_get_thread_num();
		it = arreglo.begin() + (rango*quantity);
		if (it + quantity != arreglo.end()) {
			sort(it, it + quantity);
		}
	}
	merge_v2(cant, quantity, thread_count);
	merge(cant, quantity);

}

void merge(int cant, int quantity) {
	vector<vector<int>> vectors;
	int shift = quantity;
	for (int i = 0; i < cant; i++) {
		vector<int> temp;
		temp.resize(cant);
		vectors.push_back(temp);
	}
	steady_clock::time_point t1 = steady_clock::now();
	vector<int>::iterator it1 = arreglo.begin();
#pragma omp single
	merge(it1, it1 + quantity, it1 + quantity, it1 + 2 * quantity, vectors[0].begin());
	vector<int>::iterator it2 = vectors[0].begin();
	for (int i = 1; i <= cant; i++) {
		if ((quantity * shift) < cant && (quantity * shift + quantity) <= cant) {
			merge(it2, it2 + quantity * shift, it1 + quantity * shift, it1 + quantity * shift + quantity, vectors[i].begin());
			it2 = vectors[i].begin();
			shift++;
			it1 = vectors[i].begin();
		}
	}
	steady_clock::time_point t2 = steady_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	std::cout << "Duracion en un hilo      " << fixed << std::setprecision(12) << (double)time_span.count() << " segundos.";
	std::cout << std::endl;
}

void merge_v2(int cant, int quantity, int thread_count) {
	vector<vector<int>> vectors;
	vector<int>::iterator it1 = arreglo.begin();
	vector<int>::iterator it2 = arreglo.begin();
	duration<double> time_span;
	int half;
	int j = 0;
	int shift = quantity;
	for (int i = 0; i < cant; i++) {
		vector<int> temp;
		temp.resize(cant);
		vectors.push_back(temp);
	}
	for (int niveles = 2; niveles < thread_count; niveles = niveles * 2) {
		half = thread_count / 2;
#pragma omp parallel for num_threads(half)
		for (int i = 0; i < cant; i++) {
			steady_clock::time_point t1 = steady_clock::now();
#pragma omp critical
			merge(it1, it1 + shift, it2 + shift + 1, (it2 + (2 * shift)), vectors[i].begin());
			if ((arreglo.begin() + (2 * shift)) != arreglo.end()) {
				it1 = arreglo.begin() + 2 * shift;
			}
			else {
				it1 = vectors[j].begin();
				j++;
				it2 = vectors[j].begin();
				shift = 2 * quantity;
			}
			steady_clock::time_point t2 = steady_clock::now();
			time_span = duration_cast<duration<double>>(t2 - t1);
		}
	}
	std::cout << "Duracion en varios hilos " << fixed << std::setprecision(12) << (double)time_span.count() << " segundos.";
	std::cout << std::endl;
}

