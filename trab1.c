#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Funcao JacobiRichardson
Parametros: a - matriz
			b - vetor resposta
			j_order - tamanho da matriz
			j_error - erro que será considerado
			j_ite_max - máximo de iteracoes
			j_row_test - coluna a ser avaliada
			evalute_j_row_test - valor antes de mudar o vetor resposta
Retorno: quantidade de iterações gastas. Caso 0, max iterações atingidas
Lógica: utiliza a funcao JR para resolver um sistema linear
*/
long jacobiRichardson(double **a, double *b, int j_order, double j_error, long j_ite_max, int j_row_test, double *evaluate_j_row_test) {
	int i, j, l;
	//salva o valor da diagonal, pois o mesmo será zerado afim de contas posteriores
	(*evaluate_j_row_test) = a[j_row_test][j_row_test];

	//vetores de resposta
	double *atual = (double *) malloc(sizeof(double) * j_order);
	double *ant = (double *) malloc(sizeof(double) * j_order);
	
	//arrumando a matriz e o vetor, dividindo pela diagonal da matriz
	for(i = 0; i < j_order; i++) {
		int diag = a[i][i];

		b[i] /= diag;
		
		ant[i] = b[i];

		for(j = 0; j < j_order; j++) {
			a[i][j] /= -diag;
		}

		a[i][i] = 0;
	}


	
	//fazendo o JR
	for(l = 0; l < j_ite_max; l++) {
		for(i = 0; i < j_order; i++) {
			atual[i] = 0;

			for(j = 0; j < j_order; j++) {
				atual[i] += ant[j] * a[i][j];
			}

			atual[i] += b[i];
		}

		//avaliando
		double total = 0;
		double maxErro = 0.0;
		double maxValor = 0.0;
		for (i=0; i < j_order; i++){
			total -= atual[i]*a[j_row_test][i];
			if (fabs(atual[i]-ant[i])>maxErro){
				maxErro = fabs(atual[i]-ant[i]);
			}
			if (fabs(atual[i])>maxValor){
				maxValor = fabs(atual[i]);
			}
		}

		total += atual[j_row_test];
		
		//retorna numero de iteracoes
		//printf("%lf %lf\n", maxErro, maxValor);
		if ((maxErro/maxValor)<j_error){
			(*evaluate_j_row_test) *= total;
			return l;
		}

		//faz anterior receber o atual
		for(i = 0; i < j_order; i++) 
			ant[i] = atual[i];
	}
	//avaliando
	double total = 0;
	for (i=0; i < j_order; i++){
		total -= atual[i]*a[j_row_test][i];
	}

	total += atual[j_row_test];
	(*evaluate_j_row_test) *= total;

	//erro
	return -1;

}

int main() {
	int j_order;
	int j_row_test;
	long j_ite_max;
	double j_error, evaluate_j_row_test, resultado_j_row_test;
	long ite = 0;

	int i, j;

	//leitura dos parametros
	scanf("%d", &j_order);
	scanf("%d", &j_row_test);
	scanf("%lf", &j_error);
	scanf("%ld", &j_ite_max);
	
	//leitura da matriz e vetor resposta
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

	if(ite == -1) //erro
		printf("Iterations: %ld\n", j_ite_max);
	else
		printf("Iterations: %ld\n", ite);
	printf("RowTest: %d => [%lf] =? %lf", j_row_test, evaluate_j_row_test, resultado_j_row_test);

	return 0;
}
