#include <iostream>
#include <pthread.h>

using namespace std;

/* declare a mutex */
/**
 * Para esse exercício não precisa mais que duas mutex.
 */

pthread_mutex_t mymutexA;
// [its not necessary] pthread_mutex_t mymutexB;

/**
 * Quando o processo main desse programa é criado com seu pid específico
 * e prioridade normal. Duas Threads são criadas e elas estão contidas
 * nesse processo e não fora dele.
 *
 * A região crítica em questão é a região à qual se identifica acesso via
 * ponteiro (nesse caso *ptr) para um mesmo local de memória. É obviu que
 * nesse exemplo as threads não poderão GRAVAR ao mesmo tempo no mesmo
 * local de memória, isso acarretaria em uma incoerência de dados caótica.
 *
 *
 * @see
 * @link https://moodle.ufsc.br/mod/forum/discuss.php?d=266604
 */

void *inc_(void *void_ptr)
{
	/* increment x to 100 */
	int *ptr = (int*) void_ptr;
	int i=0;
	for (; i<100; i++)
	{
		/* enter critical region */
		/**
		 * Essa funcionalidade da biblioteca pthreads
		 * tenta bloquear o mutex, se o mutex já está
		 * BLOQUEADO, a thread chamadora irá ter que
		 * esperar até que o mutex fique liberado para uso
		 *
		 * Isso vai acontecer com a Thread B tentando bloquear
		 * a Thread A, a Thread B vai esperar o mymutexA ficar
		 * liberado até que ela possa utilizá-lo.
		 *
		 * Eu comentei a parte com while e trylock
		 * pois ela não é necessária para o IC3
		 */
		pthread_mutex_lock(&mymutexA);
		// while(pthread_mutex_trylock(&mymutexB)) {
		    // pthread_mutex_unlock(&mymutexA);
		    /* stall region */
		    // pthread_mutex_lock(&mymutexA);
		// }
		++(*ptr);
		// cout << "Thread(A) está Incrementando: " << *ptr << endl;
		pthread_mutex_unlock(&mymutexA);
		// pthread_mutex_unlock(&mymutexB);
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

        /**
         *
         * Existe um problema aqui que é o seguinte
         * se você não colocar proteção alguma ambas as threads
         * executam APARENTEMENTE normal, pois mesmo que a exec.
         * delas seja atrapalhada, uma incrementará até 100
         * e a outra decrementará até 0; Nenhum problema aparente
         * surge.
         *
         * O único papel aqui dos mutex é ORDENAR a execução das Threads.
         *
         * Só que aí sim, quando você ORDENA a ordem de execução das Threads
         * você garante uma certa segurança no acesso à memória.
         *
         */

		/* enter critical region */
		pthread_mutex_lock(&mymutexA);
		// while(pthread_mutex_trylock(&mymutexA)) {
		    // pthread_mutex_unlock(&mymutexB);
		    /* stall region */
		    // pthread_mutex_lock(&mymutexB);
		// }
		--(*ptr);
		// cout << "Thread(B) está Decrementando: " << *ptr << endl;
		pthread_mutex_unlock(&mymutexA);
		// pthread_mutex_unlock(&mymutexA);
		/* leave critical region */
	}
	cout << "decrement finished" << endl;
	return NULL;
}


int main()
{
    int x = 0, status_inc_thread, status_dec_thread;
    cout << "x: " << x << endl;

    /* declare threads */
    pthread_t inc_thread, dec_thread;

    /* init mutexex */
    /**
     * Versões antigas do padrão POSIX só se usava inicializadores estáticos
     * Porém ainda se usa PTHREAD_MUTEX_INIT.. mesmo em contextos onde mais
     * tarde se usa a palavra chave 'auto' para buscar um tipo de dado auto-
     * maticamente. Em tempo de execução com inicializadores estáticos
     * eu posso ter certeza de que o mutex foi inicializado.
     *
     * Hoje em dia o pessoal usa mais inicializador de mutexes dinâmico
     * que para certos contextos é mais viável.
     *
     * Se eu usasse pthread_mutex_init (&mymutex, NULL); Eu estaria fazendo
     * a mesma coisa, porém dinâmicamente. A diferença é que no lugar de NULL
     * eu poderia setar coisas diferentes como:
     *
     *
     */

    // Aqui se você não passar o endereço dos mutexes,
    // seu código nunca irá funcionar.

    /**
     * Caso você estiver trabalhando com uma
     * struct objMutexes { pthread_mutex_t mymutexA, mymutexB; }
     * você poderia, de certa forma, fazer &obj->mymutexA, mas
     * ainda tera que passar o endereço da variável.
     *
     * pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
     *
     * Se um pthread_mutex_init foi chamado então
     * um pthread_mutex_destroy deverá ser chamado
     * até o termino da thread.
     */
    pthread_mutex_init (&mymutexA, NULL);

    /**
     * passing this CONSTANT called PTHREAD_MUTEX_INITIALIZER is another
     * way to start initiate the mutex
     */

    // mymutexA = PTHREAD_MUTEX_INITIALIZER;

    // [its not necessary] pthread_mutex_init (&mymutexB, NULL);

    // mymutexB = PTHREAD_MUTEX_INITIALIZER;

    /**
     * create a first thread which executes inc_(&x)
     * int pthread_create(pthread_t            *thread,
     *                    const pthread_attr_t *attr,
     *                    void                 *(*start_routine) (void*),
     *                    void                 *arg);
     * Criação de duas threads 'joinable'.
     * Se uma thread é 'joinable' então outra
     * thread pode chamar pthread_join
     */
    status_inc_thread = pthread_create(&inc_thread, NULL, inc_, &x);
    if (status_inc_thread) {
        cerr << "Error - pthread_create() return code: " << status_inc_thread << endl;
    }

    /* create a second thread which executes dec_(&x) */
    status_dec_thread = pthread_create(&dec_thread, NULL, dec_, &x);
    if (status_dec_thread) {
        cerr << "Error - pthread_create() return code: " << status_dec_thread << endl;
    }

    /**
     * wait for the first thread to finish
     * pthread_join (pthread_t thread, void **value_ptr);
     * A funcionalidade pthread_join suspende a execução a thread chamada
     * até que a thread finalmente termina. A não ser que a thread já
     * terminou.
     *
     * A funcionalidade join pode levantar erros também:
     * Um deles que é importânte cuidar é de DEADLOCK
     *
     * [EDEADLK] Um deadlock foi detectado.
     *
     * Exemplo conceitual que esse conjunto de 2 joins poderia
     * levantar é que quando duas threads tentam juntar-se uma a outra
     * ocorre DEADLOCK ( ver man pthread_join )
     *
     * [EINVAL] Thread não é do tipo 'joinable'
     *
     */
    pthread_join(inc_thread, NULL);

    /* wait for the second thread to finish */
    // pthread_join (pthread_t thread, void **value_ptr);
    pthread_join(dec_thread, NULL);

    /* destroy mutex */
    /* Um mutex pode ser destruído imediatamente após seu UNLOCK
       do contrário não faz sentido destruir um mutex durante o seu LOCK */
    pthread_mutex_destroy(&mymutexA);


    // [its not necessary] pthread_mutex_destroy(&mymutexB);

    cout << "x: " << x << endl;
    return 0;
}
