#pragma once
#include <string>
#include <queue>
#include <sstream>
#include <omp.h>
using namespace std;

template < typename T >
class queue_lk
{
	/* Representa una cola de mensajes de tipo msg_t con contenido T.
	   Supuestos:
	   1. T(): el constructor estándar de T está definido.
	   2. T(const T& msg_val): el constructor de copias de T está definido.
	   3. operator==(const T& msg_val): comparador está definido, se usa en "find".
	   4. operator=(const T& msg_val): asignador, se usa en "front()" como "left-value".
	   5. operator<<(ostream& ss, const T& msg_val): está definido, se usa en "operator<<".
	*/

public:
	/* Tipo de mensajes con contenido TT == T */
	template < typename TT >
	struct msg_t {
		int src_rank; // id del hilo fuente
		TT msg; // mensaje
				/* constructores */
		msg_t() : src_rank(0), msg(TT()) {};
		//msg_t(int sr) : src_rank(sr), msg() {}; genera C2535 member function already defined or declared cuando TT == int
		msg_t(TT m) : src_rank(0), msg(m) {};
		msg_t(int sr, const TT& tt) : src_rank(sr), msg(tt) {};
	};

	queue_lk();
	queue_lk(const queue< T >& orig);
	~queue_lk();
	bool empty() const;
	int size() const;
	msg_t < T >& front();
	msg_t < T > front() const;
	msg_t < T >& back();
	msg_t < T > back() const;
	void push(const msg_t < T >& msg_val);
	void pop();

	/* Métodos no estándar */
	//EFE: retorna true si t_val se encuentra en *this y false en caso contrario.
	bool find_msg(const T& msg_val) const;

	// EFE: permite que *this sea consumido por el flujo o mediante el operator<<.
	// NOTA: es una función externa a la clase que por accesar los atributos privados
	//       se debe declarar como friend.
	template < typename V >
	friend ostream& operator<<(ostream& o, const queue_lk< T >& q);

	/* Métodos no estándar para cerrar/abrir el candado */
	void set_lock(); // cierra 
	void unset_lock(); // abre
	
private:
	omp_lock_t lock; // candado omp
	int enqueued; // cantidad de mensajes encolados
	int dequeued; // cantidad de mensajes desencolados
	queue< msg_t < T > > q; // cola de mensajes
};


template < typename T >
queue_lk< T >::queue_lk()
{
	enqueued = dequeued = 0;
	omp_init_lock(&lock);
}

template < typename T >
queue_lk< T >::queue_lk(const queue< T >& orig) : enqueued(orig.enqueued), dequeued(orig.dequeued), q(orig.q){
	omp_init_lock(&lock);
}

template < typename T >
queue_lk< T >::~queue_lk()
{
	omp_destroy_lock(&lock);
}

template < typename T >
bool queue_lk< T >::empty() const {
	return q.empty();
}

template < typename T >
int queue_lk< T >::size() const {
	return enqueued - dequeued;
}

template < typename T >
typename queue_lk< T >::msg_t < T >& queue_lk< T >::front() {
	return q.front();
}

template < typename T >
typename queue_lk< T >::msg_t < T > queue_lk< T >::front() const {
	return q.front();
}

template < typename T >
typename queue_lk< T >::msg_t < T >& queue_lk< T >::back() {
	return q.back();
}

template < typename T >
typename queue_lk< T >::msg_t < T > queue_lk< T >::back() const {
	return q.back();
}

template < typename T >
void queue_lk< T >::push(const msg_t < T >& msg_val) {
	q.push(msg_val);
	enqueued++;
}

template < typename T >
void queue_lk< T >::pop() {
	q.pop();
	dequeued++;
}

template < typename T >
bool queue_lk< T >::find_msg(const T& msg_val) const {
	queue_lk< T > q_copy = q;
	bool rsl = false;
	while (!q_copy.empty() && !rsl)
		if (q_copy.front().msg == msg_val)
			rsl = true;
		else q_copy.pop();
}

template < typename T >
ostream& operator<<(ostream& o, const queue_lk< T >& q) {
	stringstream fs;
	queue_lk< T > q_copy = q;
	while (!q_copy.empty()) {
		fs << "{" << q_copy.front().src_rank << "," << q_copy.front().msg << "}";
		q_copy.pop();
	}
	return o << fs.str();
}

template < typename T >
void queue_lk< T >::set_lock() {
	omp_set_lock(&lock);
}

template < typename T >
void queue_lk< T >::unset_lock() {
	omp_unset_lock(&lock);
}