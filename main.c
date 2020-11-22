#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include <include.h>

int main(int argc, char *argv[])
{
	int step = 0;
	int output_freq = 1;
	struct MDSYSTEM mdsystem;

	if (argc == 1)
		print_help(argc, argv);
	if (argc >= 3)
		output_freq = atoi(argv[2]);

	print_task_info(0);

	mdsystem.dt = 0.01;
	mdsystem.m  =  1.0;
	mdsystem.k  =  1.0;
	mdsystem.v  =  1.0;
	mdsystem.q  = -1.0;

	update_f(&mdsystem);
	update_a(&mdsystem);

	printf("# step     q     v     T     Ek     Ep     E\n");

	for (step = 0; step < atoi(argv[1]); step++) {
		vv2(&mdsystem);
		if (step%output_freq == 0)
			print_data(mdsystem, (step + 1));
	}

	print_task_info(1);
	return 0;
}

void print_help(int argc, char *argv[])
{
	printf("%s %s","Uasge: ",argv[0]);
	printf(" %s\n","[Nsteps] [outputfreq]");
	printf(" %s%s","e.g.:  ",argv[0]);
	printf(" %s\n","10000000 500");
	exit(0);
}

void print_task_info(int pos)
{
	time_t t;
	double timer;
	static struct timeval start;
	static struct timeval end;

	if (pos == 0) {
		t = time(NULL);
		gettimeofday(&start, NULL);
		fprintf(stderr,"%s %s","Task started at :",ctime(&t));
#ifndef WIN32
		fprintf(stderr,"%s %d\n","Task PID        :",getpid());
#endif
	}
	if (pos == 1) {
		t = time(NULL);
		gettimeofday(&end, NULL);
		timer = (double)(end.tv_sec - start.tv_sec) + \
			(double)(end.tv_usec - start.tv_usec) / 1000000;
		fprintf(stderr,"%s %s","Task finished at:",ctime(&t));
		fprintf(stderr,"%s %0.5f %s\n", "Task finished in:", timer, "secs");;
	}
}

inline void print_data(struct MDSYSTEM mdsystem, int step)
{
	printf("%-8d ", step);
	printf("%15.9f",mdsystem.q);
	printf("%15.9f",mdsystem.v);
	printf("%15.9f",mdsystem.T);
	printf("%15.9f",mdsystem.Ek);
	printf("%15.9f",mdsystem.Ep);
	printf("%15.9f",mdsystem.E);
	printf("%s","\n");
}

inline void vv2(struct MDSYSTEM *mdsystem)
{
	// thermostat_lan(mdsystem, 10, 2.0);
	update_a(mdsystem);
	update_v(mdsystem);
	update_q(mdsystem);
	update_f(mdsystem);
	update_a(mdsystem);
	update_v(mdsystem);
	// thermostat_lan(mdsystem, 10, 2.0);
	update_Ek(mdsystem);
	update_Ep(mdsystem);
	update_E(mdsystem);
	update_T(mdsystem);
}

inline void update_a(struct MDSYSTEM *mdsystem)
{
	mdsystem->a = mdsystem->f / mdsystem->m;
}

inline void update_v(struct MDSYSTEM *mdsystem)
{
	mdsystem->v += mdsystem->a * mdsystem->dt / 2.0;
}

inline void update_q(struct MDSYSTEM *mdsystem)
{
	mdsystem->q += mdsystem->v * mdsystem->dt;
}

inline void update_E(struct MDSYSTEM *mdsystem)
{
	mdsystem->E = mdsystem->Ek + mdsystem->Ep;
}

inline void update_T(struct MDSYSTEM *mdsystem)
{
	mdsystem->T = mdsystem->Ek * 2.0 / k_B;
}

inline void update_Ek(struct MDSYSTEM *mdsystem)
{
	mdsystem->Ek = mdsystem->v * mdsystem->v * mdsystem->m * 0.5;
}

inline double thermostat_ber(struct MDSYSTEM *mdsystem, \
	double tau, double temp_target)
{
	return sqrt(1.0 + 1.0 / tau * (temp_target / mdsystem->T - 1));
}

inline void thermostat_lan(struct MDSYSTEM *mdsystem, \
	double tau, double temp_target)
{
	static int count = 0;
	static double c1 = 0.0;
	static double c2 = 0.0;
	if (count == 0) {
		c1 = exp(-1.0 * 0.5 / tau);
		c2 = sqrt((1 - c1 * c1) * temp_target * k_B);
		srand(time(NULL));
		count++;
	}
	mdsystem->v *= c1;
	mdsystem->v += gasdev() * c2 / sqrt(mdsystem->m);
}


/* 
 * This subroutine comes from here:
 * https://sites.google.com/site/giovannibussi/Research/algorithms
 */
inline double gasdev(void)
{
	static int iset=0;
	static double gset;
	double fac,rsq,v1,v2;

	if (iset == 0) {
		do {
			v1 = 2.0 * (double)rand() / (double)RAND_MAX - 1.0;
			v2=  2.0 * (double)rand() / (double)RAND_MAX - 1.0;
			rsq= v1 * v1 + v2 * v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac = sqrt(-2.0 * log(rsq) / rsq);
		gset = v1 * fac;
		iset = 1;
		return v2 * fac;
	} else {
		iset = 0;
		return gset;
	}
}
