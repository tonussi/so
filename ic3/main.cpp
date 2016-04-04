#include <iostream>
#include <pthread.h>

/**
  \1 Criar um aplicativo que gere duas threads

  \2 Uma delas (racing) deve implementar uma variável global 100 vezes.

  \3 Enquanto a outra (racing) deve decrementar essa mesma variável 100 vezes.

  \4 No final o valor dessa variável deve ser apresentado.

  \5 A variável global deve ser acessada para escrita com condição de exclusão
     mutua.

  \6 +6.1 pthread_mutex_lock,
     +6.2 pthread_mutex_unlock,
     +6.3 pthread_mutex_init,
     +6.4 pthread_create,
     +6.5 thread_join,
     +6.6 pthread_mutex_destroy

  \7 +7.1 pthread_t
     +7.2 pthread_mutex_t


  Observação: Do jeito que o enunciado está escrito, eu extraí que a princípio
  não se sabe quem das threads está incrementando e qual está decrementando.
  Ou seja, o sistema é caótico.

  Na implementação computacional 1 (IC 1) o que acontecia com a variável global
  count quando ela era incrementada pelos diferentes processos? O mesmo vai
  acontecer agora quando a variável global x for incrementada e decrementada
  pelas diferentes threads? Qual é o valor esperado para a variável x após o
  término do aplicativo? Se não houver mutex, qual será o valor final da
  variável global x (ou sua distribuição de probabilidade)?

  No IC1 a variável count não era global. Porém como se tratava de criação de
  processos filhos, cujas funções era incrementar uma variável count e depois
  retornar o valor do incremento, a variável count bem como todo o resto do
  programa era duplicado para outro local da memória. Uma vez copiado o SO
  tomava controle sobre os processos e os executava conforme achasse melhor
  escalonando, trocando contexto, etc. Uma vez que cada processo contava um em
  sua própria cópia de count, essa mesma ia de 0 para 1, pois 0+1=1. Uma vez
  que todos os filhos fizeram 0+1=1 e retornaram count=1. Existe ainda o
  processo pai que tinha a funcionalidade de esperar que todos os filhos fossem
  encerrados e uma vez encerrados atribuia a um somador o valor de retorno de
  seu processo filho que deve ser count=1. Somando todos os valores de retorno
  de seus processos filhos, a variável sum deve ter o valor 5 armazenado nela.
  Os filhos não competem entre si por memória, e entre os filhos não há corrida
  para ver quem incrementa sobre a variável count, pois cada processo é uma
  cópia do processo pai (com pid diferente) e está executando paralelamente se
  possível e em outro local da memória.

  Já nesse exercício do IC3 o caso é diferente. As threads estão no mesmo
  processo disputando pelo mesmo local na memória. Quando uma thread se atrela
  àquele local de memória que diz respeito à variável global count a outra
  thread pode tentar acessar para leitura esse local, mas não pode acessar para
  gravação, isto é, ela deverá ficar tentando try { ... } catch {
  std::exception &e { ... } até que a outra thread sinalize uma flag dizendo
  que o local de memória está livre para ser gravado por outrem, e vice versa.

  Exemplo onde dois processadores disputam pelo mesmo local na memória:

  Vocês acham essa modificação/implementação do IC3 válida para multicores?

  PROC_1
  1 pthread_mutex_lock(&mutexA);              // Tenta travar o mutexA
  2 while (pthread_mutex_trylock(&mutexB))    // Espera até que mutexB de unlock
  3 {
  4    pthread_mutex_unlock(&mutexA);         // Libera o mutexA para evitar Deadlock
  5    pthread_mutex_lock(&mutexA);           // Trava o mutexA
  6 }
  7 ++(*ptr);                                 // Faz o incremento
  8 pthread_mutex_unlock(&mutexA);            // Destrava o mutexA
  9 pthread_mutex_unlock(&mutexB);            // Destrava o mutexB

  PROC_2
  1 pthread_mutex_lock(&mutexB);
  2 while (pthread_mutex_trylock(&mutexA))
  3 {
  4    pthread_mutex_unlock(&mutexB);
  5    pthread_mutex_lock(&mutexB);
  6 }
  7 --(*ptr);
  8 pthread_mutex_unlock(&mutexB);
  9 pthread_mutex_unlock(&mutexA);

  Com mutex é possível prever probabilísticamente os resultados finais.
  Se as regiões críticas são protegidas por mutexes então eu consigo saber que
  ou a sequẽncia de execução é ++(*ptr) seguida de --(*ptr) OU é o contrário.¹

  Outra situação é que se ambos os ++(*ptr) e --(*ptr) estão sobre a guarda de
  mutexes lock e unlock, então esses mesmos irão sinalizar para a SO que o
  escalonador preemptivo não deve trocar o contexto da Thread A para a Thread B
  quando o Temporizador der um tick() interrompendo a SO para que ela troque o
  contexto do fluxo de execução.¹ Se a Thread A está no meio da execução de
  ++(*ptr) e o Sistema Operacional troca o contexto de execução para a Thread B é
  porquê não existe Trava de segurança (Mutex) impedindo que o Contexto seja
  trocado, pelo menos na região que é crítica para a operação de incremento da
  variável global, assim como a de decremento.¹

  Condição de Corrida é um termo usado para designar uma operação de escrita
  por Threads sobre um mesmo local de memória que não está protegido por
  Mutexes. Se esse local de memória está devidamente protegido com exclusão
  mutua então não deverá ocorrer condição de corrida (race condition).¹
  Outra coisa a se considerar é que quando Threads são inicializadas o SO toma
  controle sobre suas Threads e irá ordenar aleatóriamente a execução delas.
  Elas poderão inclusive serem executadas a velocidades diferentes.²

  Sem mutex não é possível prever os resultados finais. Eu não consigo prever
  com exatidão quem que segue quem, se é ++(*ptr) ou --(*ptr), ou o contrário.
  ou dois ++(*ptr) seguidos e depois 3 --(*ptr) seguidos. Eu não posso acreditar
  que a SO vai especialmente cuidar de regiões críticas para mim. Eu tenho que
  me preocupar em programar corretamente para que a SO não execute código que
  acessa memória sem regras.

  Referências
  ¹ Race Conditions - Revisited
    <https://www.cs.mtu.edu/~shene/NSF-3/e-Book/RACE/overview.html>

  ² POSIX thread (pthread) libraries - Thread Pitfalls
    <http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html>
 */

using namespace std;

/* declare a mutex */ pthread_mutex_t mymutex;

void *inc_(void *void_ptr)
{
	/* increment x to 100 */
	int *ptr = (int *) void_ptr;
	int i = 0;
	for (; i < 100; i++)
	{
		/* enter critical region */
    pthread_mutex_lock(&mymutex);
		++(*ptr);
    pthread_mutex_unlock(&mymutex);
		/* leave critical region */
	}
	cout << "increment finished" << endl;
	return NULL;
}

void *dec_(void *void_ptr)
{
  /* decrement x to 100 */
  int *ptr = (int *) void_ptr;
	int i=0;
	for (; i<100; i++)
	{
		/* enter critical region */
    pthread_mutex_lock(&mymutex);
    --(*ptr);
    pthread_mutex_unlock(&mymutex);
		/* leave critical region */
	}
  cout << "decrement finished" << endl;
  return NULL;
}


int main()
{
	int x = 0, status_inc_thread, status_dec_thread;
	cout << "x: " << x << endl;

	/* declare threads */ pthread_t inc_thread, dec_thread;

	/* init mutexes */
  mymutex = PTHREAD_MUTEX_INITIALIZER;

  /* create a first thread which executes inc_(&x) */
  status_inc_thread = pthread_create(&inc_thread, NULL, inc_, &x);
  if (status_inc_thread) {
    cerr << "Error - pthread_create() return code: " << status_inc_thread << endl;
    exit(EXIT_FAILURE);
  }

	/* create a second thread which executes dec_(&x) */
  status_dec_thread = pthread_create(&dec_thread, NULL, dec_, &x);
  if (status_dec_thread) {
    cerr << "Error - pthread_create() return code: " << status_inc_thread << endl;
    exit(EXIT_FAILURE);
  }

  /* wait for the first thread to finish */
  // Function call: pthread_join - wait for termination of another thread
  pthread_join(dec_thread, NULL);

	/* wait for the second thread to finish */
  pthread_join(inc_thread, NULL);

	/* destroy mutex */
  pthread_mutex_destroy(&mymutex);

	cout << "x: " << x << endl;

	return 0;
}
