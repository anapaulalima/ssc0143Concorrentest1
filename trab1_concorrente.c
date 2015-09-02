#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

int j_order;
int j_row_test;
long j_ite_max;
double j_error;
double **a;
double *b;
double *atual;
double *ant;
int **iniFim;
pthread_t *threadVetor;
int threadsRodando;
int iteracoes;
int rodarthread;
double evaluate_j_row_test;
int numeroThreads;

pthread_mutex_t mutex;
pthread_cond_t condicional;


void calcIndice(int numeroThreads){

	int i, tam = floor(j_order/numeroThreads), inicio =0; 

	iniFim = (int **) malloc(sizeof(int *) * numeroThreads);
	
	for(i = 0; i < numeroThreads; i++) {
		iniFim[i] = (int *) malloc(sizeof(int) * 2);
		iniFim[i][0] = inicio;
		iniFim[i][1] = inicio+tam-1;
		inicio += tam;
	}
	iniFim[numeroThreads-1][1] = j_order-1;
}

int min(int a, int b) { 
	return a < b ? a : b;
}

void *fazLinha(void* numero){
	int i, j, threadAtual = (*((int *) numero));


	while (rodarthread){
		for(i = iniFim[threadAtual][0]; i <= iniFim[threadAtual][1]; i++) {
			atual[i] = 0;

			for(j = 0; j < j_order; j++) {
				atual[i] += ant[j] * a[i][j];
			}

			atual[i] += b[i];
		}

		pthread_mutex_lock(&mutex);
		threadsRodando--;

		if (threadsRodando == 0){

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
			if (((maxErro/maxValor) < j_error) || (++iteracoes == j_ite_max-1)){
				evaluate_j_row_test *= total;
				rodarthread = 0;
			}

			//faz anterior receber o atual
			for(i = 0; i < j_order; i++) 
				ant[i] = atual[i];

			pthread_cond_broadcast(&condicional);
			threadsRodando = numeroThreads;
		}
		else{
			pthread_cond_wait(&condicional, &mutex);
		}
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

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
long jacobiRichardson() {
	int i, j, l;
	int *indices;
	//salva o valor da diagonal, pois o mesmo será zerado afim de contas posteriores
	evaluate_j_row_test = a[j_row_test][j_row_test];

	//vetores de resposta
	atual = (double *) malloc(sizeof(double) * j_order);
	ant = (double *) malloc(sizeof(double) * j_order);
	
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

	calcIndice(numeroThreads);

	indices = (int *) malloc (sizeof(int)*numeroThreads);

	for (i=0; i<numeroThreads; i++){
		indices[i] = i;
	}
	
	threadVetor = (pthread_t *) malloc (sizeof(pthread_t) * numeroThreads);

	iteracoes = 0;
	threadsRodando = numeroThreads;
	rodarthread = 1;

	for (i=0; i < numeroThreads; i++){
		pthread_create (&(threadVetor[i]), NULL, &fazLinha, &indices[i]);
	}

	for (i=0; i < numeroThreads; i++){
		pthread_join (threadVetor[i], NULL);
	}

	if (iteracoes==j_ite_max-1){
		return -1;
	}

	

	//erro
	return iteracoes;

}

int main(int argc, char * argv[]) {
	double resultado_j_row_test;
	long ite = 0;

	int i, j;

	//leitura dos parametros
	scanf("%d", &j_order);
	scanf("%d", &j_row_test);
	scanf("%lf", &j_error);
	scanf("%ld", &j_ite_max);

	if (argc < 2){

		numeroThreads = min(j_order, sysconf(_SC_NPROCESSORS_ONLN));
	}
	else{
		numeroThreads = atoi(argv[1]);
	}
	
	//leitura da matriz e vetor resposta
	a = (double **) malloc(sizeof(double *) * j_order);
	b = (double *) malloc(sizeof(double) * j_order);

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

	threadsRodando = 0;

	ite = jacobiRichardson();

	if(ite == -1) //erro
		printf("Iterations: %ld\n", j_ite_max);
	else
		printf("Iterations: %ld\n", ite);
	printf("RowTest: %d => [%lf] =? %lf\n", j_row_test, evaluate_j_row_test, resultado_j_row_test);

	return 0;
}
