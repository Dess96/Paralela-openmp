/* File:      histogram.c
* Purpose:   Build a histogram from some random data
*
* Compile:   gcc -g -Wall -o histogram histogram.c
* Run:       ./histogram <bin_count> <min_meas> <max_meas> <data_count>
*
* Input:     None
* Output:    A histogram with X's showing the number of measurements
*            in each bin
*
* Notes:
* 1.  Actual measurements y are in the range min_meas <= y < max_meas
* 2.  bin_counts[i] stores the number of measurements x in the range
* 3.  bin_maxes[i-1] <= x < bin_maxes[i] (bin_maxes[-1] = min_meas)
* 4.  DEBUG compile flag gives verbose output
* 5.  The program will terminate if either the number of command line
*     arguments is incorrect or if the search for a bin for a
*     measurement fails.
*
* IPP:  Section 2.7.1 (pp. 66 and ff.)
*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include<time.h>
#include<omp.h>
using namespace std;

//#define DEBUG = 1

int main(int argc, char * argv[]);

void usage(string prog_name);

void get_args(
	char*    argv[]        /* in  */,

	int&     bin_count_p   /* out */,

	float&   min_meas_p    /* out */,

	float&   max_meas_p    /* out */,
	int&     data_count_p, int & hilos  /* out */);

void gen_data(
	float   min_meas    /* in  */,
	float   max_meas    /* in  */,
	vector<float>&   data /* out */,
	int     data_count  /* in  */,
	int		hilos		/* in */);

void gen_bins(
	float min_meas      /* in  */,
	float max_meas      /* in  */,
	vector<float>& bin_maxes   /* out */,
	vector<int>&   bin_counts  /* out */,
	int   bin_count     /* in  */);

int which_bin(
	float    data         /* in */,
	vector<float>&    bin_maxes  /* in */,
	int      bin_count    /* in */,
	float    min_meas     /* in */);

void print_histo(
	vector<float>    bin_maxes   /* in */,
	vector<int>     bin_counts   /* in */,
	int      bin_count     /* in */,
	float    min_meas      /* in */);

int main(int argc, char* argv[]) {
	int bin_count, bin, hilos;		  // cantidad de bins, bin actual, bin == rango
	float min_meas, max_meas; // valor inferior de datos, valor superior de datos
	vector<float> bin_maxes;  // vector de m�ximos por bin
	vector<int> bin_counts;   // vector para contar valores por bin
	int data_count;			  // cantidad de datos
	vector<float> data;		  // vector de datos
	double start_time = omp_get_wtime(); //TIME
							  /* Check and get command line args */
	if (argc != 5) usage(argv[0]);
	get_args(argv, bin_count, min_meas, max_meas, data_count, hilos);

	/* Allocate arrays needed */

	bin_maxes.resize(bin_count);
	bin_counts.resize(bin_count);
	data.resize(data_count);

	/* Generate the data */
	gen_data(min_meas, max_meas, data, data_count, hilos);

	/* Create bins for storing counts */
	gen_bins(min_meas, max_meas, bin_maxes, bin_counts, bin_count);

	/* Count number of values in each bin */
#pragma omp parallel for num_threads(hilos) private(bin)
	for (int i = 0; i < data_count; i++) {
		int bin = which_bin(data[i], bin_maxes, bin_count, min_meas);
#pragma omp atomic 
		//dos hilos pueden intentar escribir sobre la misma posicion a la misma vez. Solo uno la modificaria
		bin_counts[bin]++;
	}

#  ifdef DEBUG
	cout << "bin_counts = ";
	for (int i = 0; i < bin_count; i++)
		cout << " " << bin_counts[i];
	cout << endl;
#  endif

	/* Print the histogram */
	cout << endl << endl;
	print_histo(bin_maxes, bin_counts, bin_count, min_meas);
	double time = omp_get_wtime() - start_time;
	cout << "El programa paralelo tardo " << time << " segundos haciendo el histograma" << endl;
	cin >> bin;
	return 0;
}  /* main */

   /*---------------------------------------------------------------------
   * Function:  usage
   * Purpose:   Print a message showing how to run program and quit
   * In arg:    prog_name:  the name of the program from the command line
   */
void usage(string prog_name /* in */) {
	cerr << "usage: " << prog_name << "<bin_count> <min_meas> <max_meas> <data_count>\n" << endl;
	//	exit(0);
}  /* usage */

   /*---------------------------------------------------------------------
   * Function:  get_args
   * Purpose:   Get the command line arguments
   * In arg:    argv:  strings from command line
   * Out args:  bin_count_p:   number of bins
   *            min_meas_p:    minimum measurement
   *            max_meas_p:    maximum measurement
   *            data_count_p:  number of measurements
   */
void get_args(
	char*    argv[]        /* in  */,

	int&     bin_count_p   /* out */,

	float&   min_meas_p    /* out */,

	float&   max_meas_p    /* out */,
	int&     data_count_p, int & hilos  /* out */) {

	bin_count_p = strtol(argv[1], NULL, 10);
	min_meas_p = strtof(argv[2], NULL);
	max_meas_p = strtof(argv[3], NULL);
	data_count_p = strtol(argv[4], NULL, 10);
	hilos = strtol(argv[5], NULL, 10);

#  ifdef DEBUG
	cout << "bin_count = " << bin_count_p << endl;
	cout << "min_meas = " << min_meas_p << "max_meas = " << max_meas_p << endl;
	cout << "data_count = " << data_count_p << endl;
#  endif
}  /* get_args */

   /*---------------------------------------------------------------------
   * Function:  gen_data
   * Purpose:   Generate random floats in the range min_meas <= x < max_meas
   * In args:   min_meas:    the minimum possible value for the data
   *            max_meas:    the maximum possible value for the data
   *            data_count:  the number of measurements
   * Out arg:   data:        the actual measurements
   */

void gen_data(
	float   min_meas    /* in  */,
	float   max_meas    /* in  */,
	vector<float>&   data /* out */,
	int     data_count  /* in  */,
	int		hilos		/* in */) {

	srand(0);
#pragma omp parallel for num_threads(hilos)
	for (int i = 0; i < data_count; i++)
		data[i] = min_meas + (max_meas - min_meas)*rand() / ((double)RAND_MAX);

#  ifdef DEBUG
	cout << "data = ";
	for (int i = 0; i < data_count; i++)
		cout << " ", data[i]);
		cout << endl;
#  endif
}  /* gen_data */

   /*---------------------------------------------------------------------
   * Function:  gen_bins
   * Purpose:   Compute max value for each bin, and store 0 as the
   *            number of values in each bin
   * In args:   min_meas:   the minimum possible measurement
   *            max_meas:   the maximum possible measurement
   *            bin_count:  the number of bins
   * Out args:  bin_maxes:  the maximum possible value for each bin
   *            bin_counts: the number of data values in each bin
   */
void gen_bins(
	float min_meas      /* in  */,
	float max_meas      /* in  */,
	vector<float>& bin_maxes   /* out */,
	vector<int>&   bin_counts  /* out */,
	int   bin_count     /* in  */) {
	float bin_width;

	bin_width = (max_meas - min_meas) / bin_count;

	for (int i = 0; i < bin_count; i++) {
		bin_maxes[i] = min_meas + (i + 1)*bin_width;
		bin_counts[i] = 0;
	}

#  ifdef DEBUG
	cout << "bin_maxes = ";
	for (i = 0; i < bin_count; i++)
		cout << " " << bin_maxes[i]);
		cout << endl);
#  endif
}  /* gen_bins */

   /*---------------------------------------------------------------------
   * Function:  which_bin
   * Purpose:   Use binary search to determine which bin a measurement
   *            belongs to
   * In args:   data:       the current measurement
   *            bin_maxes:  list of max bin values
   *            bin_count:  number of bins
   *            min_meas:   the minimum possible measurement
   * Return:    the number of the bin to which data belongs
   * Notes:
   * 1.  The bin to which data belongs satisfies
   *
   *            bin_maxes[i-1] <= data < bin_maxes[i]
   *
   *     where, bin_maxes[-1] = min_meas
   * 2.  If the search fails, the function prints a message and exits
   */

int which_bin(
	float    data         /* in */,
	vector<float>&    bin_maxes  /* in */,
	int      bin_count    /* in */,
	float    min_meas     /* in */) {
	int bottom = 0, top = bin_count - 1;
	int mid;
	float bin_max, bin_min;

	while (bottom <= top) {
		mid = (bottom + top) / 2;
		bin_max = bin_maxes[mid];
		bin_min = (mid == 0) ? min_meas : bin_maxes[mid - 1];
		if (data >= bin_max)
			bottom = mid + 1;
		else if (data < bin_min)
			top = mid - 1;
		else
			return mid;
	}

	/* Whoops! */
	cerr << "Data = " << data << " doesn't belong to a bin!" << endl;
	cerr << "Quitting" << endl;
	exit(-1);
}  /* which_bin */

   /*---------------------------------------------------------------------
   * Function:  print_histo
   * Purpose:   Print a histogram.  The number of elements in each
   *            bin is shown by an array of X's.
   * In args:   bin_maxes:   the max value for each bin
   *            bin_counts:  the number of elements in each bin
   *            bin_count:   the number of bins
   *            min_meas:    the minimum possible measurment
   */
void print_histo(
	vector<float>    bin_maxes   /* in */,
	vector<int>     bin_counts   /* in */,
	int      bin_count     /* in */,
	float    min_meas      /* in */) {
	float bin_max, bin_min;

	for (int i = 0; i < bin_count; i++) {
		bin_max = bin_maxes[i];
		bin_min = (i == 0) ? min_meas : bin_maxes[i - 1];
		printf("%.3f-%.3f:\t", bin_min, bin_max);
		for (int j = 0; j < bin_counts[i]; j++)
			cout << "X";
		cout << endl;
	}
}  /* print_histo */