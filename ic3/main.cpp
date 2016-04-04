#include <iostream>
#include <pthread.h>

/**
 * \1 Criar um aplicativo que gere duas threads
 *
 * \2 Uma delas (racing) deve implementar uma variável global 100 vezes.
 *
 * \3 Enquanto a outra (racing) deve decrementar essa mesma variável 100 vezes.
 *
 * \4 No final o valor dessa variável deve ser apresentado.
 *
 * \5 A variável global deve ser acessada para escrita com condição de exclusão
 *    mutua.
 *
 * \6 6.1 pthread_mutex_lock,
 *    6.2 pthread_mutex_unlock,
 *    6.3 pthread_mutex_init,
 *    6.4 pthread_create,
 *    6.5 thread_join,
 *    6.6 pthread_mutex_destroy
 *
 * \7 \7.1 pthread_t
 *    \7.2 pthread_mutex_t
 *
 *
 * Observação: Do jeito que o enunciado está escrito, eu extraí que a princípio
 * não se sabe quem das threads está incrementando e qual está decrementando.
 * Ou seja, o sistema é caótico.
 *
 * Na implementação computacional 1 (IC 1) o que acontecia com a variável global
 * count quando ela era incrementada pelos diferentes processos? O mesmo vai
 * acontecer agora quando a variável global x for incrementada e decrementada
 * pelas diferentes threads? Qual é o valor esperado para a variável x após o
 * término do aplicativo? Se não houver mutex, qual será o valor final da
 * variável global x (ou sua distribuição de probabilidade)?
 *
 * No IC1 a variável count não era global. Porém como se tratava de criação de
 * processos filhos, cujas funções era incrementar uma variável count e depois
 * retornar o valor do incremento, a variável count bem como todo o resto do
 * programa era duplicado para outro local da memória. Uma vez copiado o SO
 * tomava controle sobre os processos e os executava conforme achasse melhor
 * escalonando, trocando contexto, etc. Uma vez que cada processo contava um em
 * sua própria cópia de count, essa mesma ia de 0 para 1, pois 0+1=1. Uma vez
 * que todos os filhos fizeram 0+1=1 e retornaram count=1. Existe ainda o
 * processo pai que tinha a funcionalidade de esperar que todos os filhos fossem
 * encerrados e uma vez encerrados atribuia a um somador o valor de retorno de
 * seu processo filho que deve ser count=1. Somando todos os valores de retorno
 * de seus processos filhos, a variável sum deve ter o valor 5 armazenado nela.
 * Os filhos não competem entre si por memória, e entre os filhos não há corrida
 * para ver quem incrementa sobre a variável count, pois cada processo é uma
 * cópia do processo pai (com pid diferente) e está executando paralelamente se
 * possível e em outro local da memória.
 *
 * Já nesse exercício do IC3 o caso é diferente. As threads estão no mesmo
 * processo disputando pelo mesmo local na memória. Quando uma thread se atrela
 * àquele local de memória que diz respeito à variável global count a outra
 * thread pode tentar acessar para leitura esse local, mas não pode acessar para
 * gravação, isto é, ela deverá ficar tentando try { ... } catch {
 * std::exception &e { ... } até que a outra thread sinalize uma flag dizendo
 * que o local de memória está livre para ser gravado por outrem, e vice versa.
 *
 * Com mutex é possível prever probabilísticamente os resultados finais.
 * Sem mutex não é possível prever os resultados finais.
 */

using namespace std;

/* declare a mutex */ mymutex;

void *inc_(void *void_ptr)
{
	/* increment x to 100 */
	int *ptr = (int *) void_ptr;
	int i = 0;
	for (; i < 100; i++)
	{
		/* enter critical region */
		++(*ptr);
		/* leave critical region */
	}
	cout << "increment finished" << endl;
	return NULL;
}

void *dec_(void *void_ptr)
{
  /* decrement x to 100 */
  int *ptr = (int *)void_ptr;
	int i=0;
	for (; i<100; i++)
	{
		/* enter critical region */
		--(*ptr);
		/* leave critical region */
	}
  cout << "decrement finished" << endl;
  return NULL;
}


int main()
{
	int x = 0;
	cout << "x: " << x << endl;

	/* declare threads */ inc_thread, dec_thread;

	/* init mutexex */

  /* create a first thread which executes inc_(&x) */

	/* create a second thread which executes dec_(&x) */

  /* wait for the first thread to finish */

	/* wait for the second thread to finish */

	/* destroy miutex */

	cout << "x: " << x << endl;

	return 0;
}
