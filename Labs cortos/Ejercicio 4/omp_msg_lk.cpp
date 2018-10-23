/* File:     omp_msglk.cpp
*
* Purpose:  Simulate message-passing using OpenMP.  This version uses
*           an atomic directive and OpenMP locks to protect critical
*           sections.
*
* Compile:  g++ -g -Wall -fopenmp -o omp_msglk omp_msglk.c queue_lk.c
*           needs queue_lk.h
* Usage:    ./omp_msglk <number of threads> <number of messages each
*                  thread sends>
*
* Input:    None
* Output:   Source, destination and contents of each message received.
*
*
* Notes:
* 1.  DEBUG flag for more verbose output
* 2.  This version uses locks to control access to the message queues.
*
* IPP:      Section 5.8.9 (pp. 248 and ff.)
*/
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <vector>
using namespace std;

#include "queue_lk.h"

//#define DEBUG 1 // activa código de depuración
const int MAX_MSG = 10000;

bool validaCntHilos(int ch);
bool validaCntMensajes(int cm);
void send_msg(vector< queue_lk< int > >& msg_queues, int my_rank, int thread_count, int msg_number);
void try_receive(queue_lk< int >& q_p, int my_rank);
int done(queue_lk< int >& q_p, int done_sending, int thread_count);
int main(int argc, char* argv[]) {
	int thread_count = 0;
	int send_max = 0;
	int done_sending = 0;
	vector< queue_lk< int > > msg_queues;

	while (!validaCntHilos(thread_count) || !validaCntMensajes(send_max)) {
		cout << "Digite la cantidad de hilos ( >= 1 ): ";
		cin >> thread_count;
		cout << "Digite la cantidad de mensajes ( >= 1 ): ";
		cin >> send_max;
	}

	//msg_queues.resize(thread_count, queue_lk< int >()); // innecesario y genera error
	msg_queues.resize(thread_count); // se crean e inicializan las colas de mensajes usando queue_lk< int >(), una para cada hilo
#  pragma omp parallel num_threads(thread_count) default(none) shared(cin, thread_count, send_max, msg_queues, done_sending)
	{
		int my_rank = omp_get_thread_num();
		int msg_number;
		srand(my_rank);
#     ifdef DEBUG
		if (my_rank == 0)
			cin >> msg_number;
#     endif
#     pragma omp barrier /* Don't let any threads send messages until all queues are constructed */
		for (msg_number = 0; msg_number < send_max; msg_number++) {
			send_msg(msg_queues, my_rank, thread_count, msg_number);
			try_receive(msg_queues[my_rank], my_rank);
		}
#     pragma omp atomic
		done_sending++;
#     ifdef DEBUG
		printf("Thread %d > done sending\n", my_rank);
#     endif

		while (!done(msg_queues[my_rank], done_sending, thread_count))
			try_receive(msg_queues[my_rank], my_rank);
	}
	cin >> done_sending;
	return 0;
}

bool validaCntHilos(int ch) {
	return ch >= 1;
}

// ct es la cantidad de términos de la serie
bool validaCntMensajes(int cm) {
	return cm >= 1;
}

/*-------------------------------------------------------------------*/
void send_msg(vector< queue_lk< int > >& msg_queues, int my_rank, int thread_count, int msg_number) {
	int mesg = rand() % MAX_MSG;
	//int mesg = -msg_number;
	int dest = rand() % thread_count;

	msg_queues[dest].set_lock();
	queue_lk< int >::msg_t<int> n_msg(my_rank, mesg);
	msg_queues[dest].push(n_msg);
	msg_queues[dest].unset_lock();

#  ifdef DEBUG
	cout << "Thread > " << my_rank << " sent " << mesg << " to " << dest << endl;
#  endif
}  /* send_msg */

   /*-------------------------------------------------------------------*/
void try_receive(queue_lk< int >& q_p, int my_rank) {
	queue_lk< int >::msg_t< int > msg; // se crea un mensaje vacío
	if (!q_p.empty()) {
		if (q_p.size() == 1) {
			q_p.set_lock();
			msg = q_p.front();
			q_p.pop();
			q_p.unset_lock();
		}
		else {
			msg = q_p.front();
			q_p.pop();
		}
#	pragma omp critical
		{
			cout << "Thread > " << my_rank << " received " << msg.msg << " from " << msg.src_rank << endl;
		}
	}
}   /* try_receive */

	/*-------------------------------------------------------------------*/
int done(queue_lk< int >& q_p, int done_sending, int thread_count) {
	if (q_p.size() == 0 && done_sending == thread_count)
		return 1;
	else
		return 0;
}   /* done */