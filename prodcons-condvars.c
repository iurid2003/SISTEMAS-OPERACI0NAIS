#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_PROD 5
#define NUM_CONS 3
#define VAGAS 6

pthread_mutex_t buffer_mutex;		// mutex para acesso ao buffer

pthread_mutex_t itens_mutex;		// vc e mutex para controle de itens
pthread_cond_t itens_cv;

pthread_mutex_t vagas_mutex;		// vc e mutex para controle de vagas
pthread_cond_t	vagas_cv;

int num_itens, num_vagas;		// contadores de itens e vagas

// espera um tempo aleatorio entre 0 e n (float)
void espera (int n)
{
	sleep(random()%n);		// pausa entre 0 e n segundos (inteiro)
	usleep(random()%1000000);	// pausa entre 0 e 1 segundo (float)
}

// corpo de produtor
void *prodBody(void *id)
{
	long tid = (long) id;

	printf("P%02ld: Olá!\n", tid);

	while(1)
	{
		// espera uma vaga
		pthread_mutex_lock (&vagas_mutex);
		while(num_vagas == 0)
		{
			pthread_cond_wait(&vagas_cv, &vagas_mutex);
		}
		num_vagas--;
		pthread_mutex_unlock(&vagas_mutex);

		// obtem acesso exclusivo ao buffer
		pthread_mutex_lock(&buffer_mutex);

		printf("P%02ld: put an item (%d itens, %d vagas)!\n", tid, num_itens, num_vagas);

		// libera acesso exclusivo ao buffer
		pthread_mutex_unlock(&buffer_mutex);

		// notifica a existência de um item
		pthread_mutex_lock(&itens_mutex);
		num_itens++;
		pthread_cond_signal(&itens_cv);
		pthread_mutex_unlock(&itens_mutex);

		espera(2);
	}
}

// corpo de consumidor
void *consBody(void *id)
{
	long tid = (long) id;

	printf("C%02lod: Olá!\n", tid);

	while(1)
	{
		// espera um item
		pthread_mutex_lock(&itens_mutex);
		while(num_itens == 0) 
		{
			pthread_cond_wait(&itens_cv, &itens_mutex);
		}
		num_itens--;
		pthread_mutex_unlock(&itens_mutex);

		// obtem acesso exclusivo ao buffer
		pthread_mutex_lock(&buffer_mutex);

		printf("C%02ld: got an item (%d itens, %d vagas)!\n", tid, num_itens, num_vagas);

		// libera acesso exclusivo ao buffer
		pthread_mutex_unlock(&buffer_mutex);

		// notifica a liberação de uma vaga
		pthread_mutex_lock(&vagas_mutex);
		num_vagas++;
		pthread_cond_signal(&vagas_cv);
		pthread_mutex_unlock(&vagas_mutex);

		espera(2);
	}
}

//programa principal

int main(int argc, char *argv[])
{
	pthread_t produtor[NUM_PROD];
	pthread_t consumidor[NUM_CONS];
	long i;

	num_itens = 0;
	num_vagas = VAGAS;

	// inicia variaveis de condição e mutexes
	pthread_mutex_init(&buffer_mutex, NULL);
	pthread_mutex_init(&itens_mutex, NULL);
	pthread_mutex_init(&vagas_mutex, NULL);
	pthread_cond_init(&itens_cv, NULL);
	pthread_cond_init(&vagas_cv, NULL);

	// cria produtores
	for(i=0; i<NUM_PROD; i++)
	{
		if(pthread_create(&produtor[i], NULL, prodBody, (void *) i))
		{
			perror("pthread_create");
			exit(1);
		}
	}

	// cria consumidores
	for(i=0; i<NUM_CONS; i++)
	{
		if(pthread_create(&consumidor[i], NULL, consBody, (void *) i))
		{
			perror("pthread_create");
			exit(1);
		}

	}

	// main encerra aqui
	pthread_exit(NULL);
}

