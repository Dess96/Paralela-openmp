#include<stdlib.h>
#include<iostream>
#include<omp.h>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include"queue_lk.h"

using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1 // para deshabilitar errores por uso de funciones deprecated sobre CRT o consola
#pragma warning(disable : 4996)

queue_lk< string > msg_queues;
void producers(int);
void consumers();

int main(int argc, char* argv[]) {
	int thread_countPr, thread_countCon, thread_count, rank;
	int processed = 0;
	bool done = false;
	cout << "Ingrese la cantidad de hilos productores >= 1" << endl;
	cin >> thread_countPr;
	cout << "Ingrese la cantidad de hilos consumidores >= 1" << endl;
	cin >> thread_countCon;
	thread_count = thread_countCon + thread_countPr;
	//cout << thread_count << "Threads" << endl;
#pragma omp parallel num_threads(thread_count) shared(msg_queues)
	{
	rank = omp_get_thread_num();
		if (rank < thread_countPr) {
			producers(rank);
			processed++;
		}
		else {
			while (processed <= thread_countPr) {
				consumers();
			}
		}
	}
	cin >> thread_countPr; //Para que no se cierre
	return 0;
}

void producers(int rank) {
	string number;
	string name = "arch_";
	number = to_string(rank);
	name.append(number);
	name.append(".txt");
	ifstream file(name);
	if (file) {
		msg_queues.set_lock();
		for (std::string line; getline(file, line); )
		{
			queue_lk<string>::msg_t<string> n_msg(rank, line); /*Crear mensaje con atributos de hilo consumidor que lo debe tokenizar y la linea del archivo*/												  //Critical para evitar que que varios hilos productores modifiquen la cola
			msg_queues.push(n_msg);
		}
		msg_queues.unset_lock();
	}
}

void consumers() {
	queue_lk< string >::msg_t< string > tokenize; //msg vacio
	char* result;
	char arraych[1024];
	while (!msg_queues.empty()) {
		if (msg_queues.size() == 1) {
			msg_queues.set_lock();
			tokenize = msg_queues.front();
			msg_queues.pop();
			msg_queues.unset_lock();
		}
		else {
			tokenize = msg_queues.front();
			msg_queues.pop();
		}
//Tokenizando
		strcpy(arraych, tokenize.msg.c_str());
		result = strtok(arraych, " ");
#pragma omp critical
		while (result != NULL) { //Imprime cada palabra de una linea
			printf("%s\n", result);
			result = strtok(NULL, " ");
			cout << "Archivo" << tokenize.src_rank << endl;
		}
	}
}