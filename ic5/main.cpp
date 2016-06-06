#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* to be able to use alarm */
#include <unistd.h>

/* to be able to use basic signal handling (i.e: sigaction) */
#include <signal.h>

/* to be able to use getrusage */
#include <sys/time.h>
#include <sys/resource.h>

// Nome do Aluno: LUCAS PAGOTTO TONUSSI

using namespace std;

void timer_handler(int signalNumber)
{
    std::cout << "timer_handler" << std::endl;
    static int counter = 0;
    counter++;
    cout << "Timer interrupt invoked " << counter << " times" << endl;
}


int main ()
{
    /* declarações */
    struct sigaction sa;
    struct itimerval it_val;
    struct rusage usage;
    int who = RUSAGE_SELF;

    /**
     * \brief: This structure is used with getrlimit to receive limit values,
     * and with setrlimit to specify limit values for a particular process and
     * resource.
     *
     * \param rlim_t rlim_cur: The current limit
     * \param rlim_t rlim_max: The maximum limit.
     */
    struct rlimit newLimitTime;

    int errno;

    pid_t pid = getpid();

    if (signal(SIGALRM, timer_handler) == SIG_ERR) {
        cerr << "Erro signal() %d: %s\n" << errno << endl;
        exit(1);
    }

    /* Instalar o tratador de sinal do alarme */
    // sa.sa_handler = timer_handler;
    // sigemptyset(&sa.sa_mask);


    /**
     * \brief Sets the first num bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char)
     *
     * \param [ptr] Pointer to the block of memory to fill.
     * \param [value] Value to be set. The value is passed as an int, but the function fills the block of memory using the unsigned char conversion of this value.
     * \param [num] Number of bytes to be set to the value.
     */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);
    // sigemptyset(&sa.sa_mask);

    /* Restart functions if interrupted by handler */
    // sa.sa_flags = SA_RESTART | SA_SIGINFO;

    /**
     * \brief The sigaction() function allows the calling process to examine and/or
     *        specify the action to be associated with a specific signal.
     *
     * \param Pointer to a signal-catching function or one of the macros SIG_IGN or SIG_DFL.
     * \param Additional set of signals to be blocked during execution of signal-catching function.
     * \param Special flags to affect behavior of signal.
     * \param Pointer to a signal-catching function.
     *
     */
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        cerr << "Error trying to set a signal handler" << endl;
    }

    /* Configurar o timer para disparar depois de 333ms e a cada 333ms depois disso*/
    // alarm(0.333);

    /* Setar o timer para iniciar a contagem enquanto este processo estiver executando */
    it_val.it_value.tv_sec = 0;
    it_val.it_value.tv_usec = 333000;
    it_val.it_interval.tv_sec = 0;
    it_val.it_interval.tv_usec = 333000;

    /**
     * \brief This system call alters the system temporizer to an interval of 2 seconds
     *        see it_val.it_value.tv_sec = 2
     *
     *        int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
     *
     * \param [which] é o temporizador de intervalo que terá o valor alterado
     *                ITIMER_REAL    | decrementa no tempo real e envia o sinal SIGALRM quando expira
     *
     * \param [itimerval *new_value] new_value é um ponteiro para uma estrutura do tipo itimerval que possui o novo valor do temporizador
     * \param [itimerval *old_value] old_value é um ponteiro para uma estrutura do tipo itimerval que armazenará o valor antigo do temporizador
     *
     * \return Se a chamada de sistema setitimer( ) é executada com sucesso, o valor zero é retornado
     *         Se ocorre um erro na execução desta chamada, o valor -1 é retornado e o código de erro
     *         é colocado na variável externa errno
     *         EFAULT | O parâmetro new_value, old_value ou curr_value não possui um ponteiro válido
     *         EINVAL | O temporizador informado ou um campo de new_value não é válido
     */
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        printf("Erro setitimer() %d\n", errno);
        exit(1);
    }

    /* Obter os limites atuais deste processo */
    // if (prlimit(pid, RLIMIT_CPU, NULL, &oldLimitTime) == -1) {
    //     cerr << "prlimit-2" << endl;
    // }
    // printf("New limits: soft=%lld; hard=%lld\n", (long long) oldLimitTime.rlim_cur, (long long) oldLimitTime.rlim_max);

    /* Ajustar o limite de uso da CPU para 2 segundos */
    // newProcessorLimitTime = NULL;
    //newLimitTime.rlim_cur = 2000;
    newLimitTime.rlim_max = 2;
    // newProcessorLimitTime = &newLimitTime;

    //if (prlimit(getpid(), RLIMIT_CPU, newProcessorLimitTime, &oldLimitTime) == -1) {
    //    cerr << "prlimit-1" << endl;
    //}
    //printf("Previous limits: soft=%lld; hard=%lld\n", (long long) oldLimitTime.rlim_cur, (long long) oldLimitTime.rlim_max);

    setrlimit(RLIMIT_CPU, &newLimitTime);

    /* Ficar preso num laço infinito */
    int i=0;
    while (1)
    {
        i = i + 1;
        /* a cada 1 mihão de iterações, obter a utilização atual da CPU */
        if(i == 0) {

            /**
             *
             * \param ru_utime: A struct timeval field containing the amount of
             * user time, in seconds, that the process has used. User time is CPU
             * time spent executing the user program, rather than in kernel system calls.
             *
             * \param ru_stime: A struct timeval field containing the amount of
             * system time, in seconds, that the process has used. System time
             * is the CPU time spent executing system calls on behalf of the process.
             *
             * \return:
             *        On success, zero is returned.  On error, -1 is returned, and errno is
             *        set appropriately.
             *
             * \ERRORS:
             *        EFAULT usage points outside the accessible address space.
             *        EINVAL who is invalid.
             */
            getrusage(who, &usage);
            printf ("CPU time: %ld.%06ld sec user, %ld.%06ld sec system\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,  usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
            i = (i + 1) % 1000000;
        }
    }
    return 0;
}
