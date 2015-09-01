#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long jacobiRichardson(double **a, double *b, int j_order, double j_error, long j_ite_max, int j_row_test, double *evaluate_j_row_test) {
	int i, j, l;
	(*evaluate_j_row_test) = a[j_row_test][j_row_test];

	double *atual = (double *) malloc(sizeof(double) * j_order);
	double *ant = (double *) malloc(sizeof(double) * j_order);
	
	for(i = 0; i < j_order; i++) {
		int diag = a[i][i];

		b[i] /= diag;
		
		ant[i] = b[i];

		for(j = 0; j < j_order; j++) {
			a[i][j] /= -diag;
		}

		a[i][i] = 0;
	}


	
	
	for(l = 0; l < j_ite_max; l++) {
		for(i = 0; i < j_order; i++) {
			atual[i] = 0;

			for(j = 0; j < j_order; j++) {
				atual[i] += ant[j] * a[i][j];
			}

			atual[i] += b[i];
		}


		double total = 0;
		for (i=0; i < j_order; i++){
			total -= atual[i]*a[j_row_test][i];
		}

		total += atual[j_row_test];
		
		if (fabs(total-b[j_row_test])<j_error){
			(*evaluate_j_row_test) *= total;
			return l;
		}

		for(i = 0; i < j_order; i++) 
			ant[i] = atual[i];
	}

	double total = 0;
	for (i=0; i < j_order; i++){
		total -= atual[i]*a[j_row_test][i];
	}

	total += atual[j_row_test];
	(*evaluate_j_row_test) *= total;
	
	return 0;

}

int main() {
	int j_order;
	int j_row_test;
	long j_ite_max;
	double j_error, evaluate_j_row_test, resultado_j_row_test;
	long ite = 0;

	int i, j;

	scanf("%d", &j_order);
	scanf("%d", &j_row_test);
	scanf("%lf", &j_error);
	scanf("%ld", &j_ite_max);
	

	double **a = (double **) malloc(sizeof(double *) * j_order);
	double *b = (double *) malloc(sizeof(double) * j_order);

	for(i = 0; i < j_order; i++) {
		a[i] = (double *) malloc(sizeof(double) * j_order);
		for(j = 0; j < j_order; j++) {
			scanf("%lf", &a[i][j]);
		}
	}

	for(i = 0; i < j_order; i++) {
		scanf("%lf", &b[i]);
	}
	resultado_j_row_test = b[j_row_test];

	ite = jacobiRichardson(a, b, j_order, j_error, j_ite_max, j_row_test, &evaluate_j_row_test);

	if(ite == 0)
		printf("Iterations: %ld\n", j_ite_max);
		
	else
		printf("Iterations: %ld\n", ite);
	printf("RowTest: %d => [%lf] =? %lf", j_row_test, evaluate_j_row_test, resultado_j_row_test);

	return 0;
}
