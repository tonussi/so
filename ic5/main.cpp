/* Copyright (C) 2010-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>. */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
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
    struct sigaction sa;
    struct itimerval despertador;
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
    struct rlimit oldLimitTime, newLimitTime;
    struct rlimit *newProcessorLimitTime;

    int errno;

    pid_t pid = getpid();

    if (signal(SIGALRM, timer_handler) == SIG_ERR) {
        cerr << "Erro signal() %d: %s\n" << errno << endl;
        exit(1);
    }

    /*! Instalar o tratador de sinal do alarme
     *
     * \brief Sets the first num bytes of the block of memory pointed by ptr to the specified value (interpreted as an unsigned char)
     *
     * \param [ptr] Pointer to the block of memory to fill.
     * \param [value] Value to be set. The value is passed as an int, but the function fills the block of memory using the unsigned char conversion of this value.
     * \param [num] Number of bytes to be set to the value.
     */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);
    sigemptyset(&sa.sa_mask);

    /*! Restart functions if interrupted by handler */
    sa.sa_flags = SA_RESTART | SA_SIGINFO;

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
    if (sigaction(SIGALRM | SIGPROF, &sa, NULL) == -1) {
        cerr << "Error trying to set a signal handler" << endl;
    }

    /* Setar o timer para iniciar a contagem enquanto este processo estiver executando */
    despertador.it_value.tv_sec = 0;
    despertador.it_value.tv_usec = 333000;
    despertador.it_interval.tv_sec = 0;
    despertador.it_interval.tv_usec = 333000;

    /**
     * \brief This system call alters the system temporizer to an interval of 2 seconds
     *        see despertador.it_value.tv_sec = 2
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
    if (setitimer(ITIMER_REAL, &despertador, NULL) == -1) {
        printf("Erro setitimer() %d\n", errno);
        exit(1);
    }

    /*! Obter os limites atuais deste processo */

    /**
     * \param __pid_t:           a process pid
	   * \param __rlimit_resource: resource
	   * \param struct rlimit:     new_rlimit
	   * \param struct rlimit:     old_rlimit
     */
    if (prlimit(pid, RLIMIT_CPU, NULL, &oldLimitTime) == -1) {
        cerr << "prlimit-2" << endl;
    }
    printf("New limits: soft=%lld; hard=%lld\n", (long long) oldLimitTime.rlim_cur, (long long) oldLimitTime.rlim_max);

    /* Ajustar o limite de uso da CPU para 2 segundos */
    newProcessorLimitTime = NULL;
    newLimitTime.rlim_cur = 2;
    newLimitTime.rlim_max = 2;
    newProcessorLimitTime = &newLimitTime;

    if (prlimit(getpid(), RLIMIT_CPU, newProcessorLimitTime, &oldLimitTime) == -1) {
       cerr << "prlimit-1" << endl;
    }
    printf("Previous limits: soft=%lld; hard=%lld\n", (long long) oldLimitTime.rlim_cur, (long long) oldLimitTime.rlim_max);

    setrlimit(RLIMIT_CPU, &newLimitTime);

    /* Ficar preso num laço infinito */
    int i=0;
    while (1) {
        i = i + 1;
        /* a cada 1 mihão de iterações, obter a utilização atual da CPU */
        if(i == 1000000) {

            /**
             *
             * \param ru_utime: A struct timeval field containing the amount of
             * user time, in seconds, that the process has used. User time is CPU
             * time spent executing the user program, rather than in kernel system calls.
             *
             * \param ru_stime: A struct timeval field containing the amount of
             * system time, in seconds, that the process has used. System time
             * is the CPU time spent executing system calls on behalf of the process.
             * \return: On success, zero is returned.
             *          On error, -1 is returned, and errno is set.
             * \ERRORS: EFAULT usage points outside the accessible address space.
             *          EINVAL who is invalid.
             */
            getrusage(who, &usage);
            printf ("User CPU time => [%ld.%06ld sec user], [%ld.%06ld sec system]\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,  usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
            i = 0;
        }
    }
    return 0;
}

/*
Qual é a precisão do timer?

Eu ajustei o itimerval despertador para fazer o seguinte:

    despertador.it_value.tv_sec = 0;
    despertador.it_value.tv_usec = 333000;

    => O primeiro tick vai ser 0 + 0.333 segundos -> porém ele acontece um pouco antes.
       O ideal de ticks deveriam ser:   0.333, 0.666, 0.999, 1.332, 1,665, 1.998.
       Porém é quase, no caso real são: 0.326, 0.653, 0.983, 1.315, 1.645, 1.973.

    despertador.it_interval.tv_sec = 0;
    despertador.it_interval.tv_usec = 333000;

    => Depois do primeiro tick, todos os outros são 0 + 0.333 = 0.333 segundos.

CPU time: [0.326813 sec user] , [0.000000 sec system] (Primeiro tick)
CPU time: [0.653204 sec user] , [0.003993 sec system] (Segundo tick)
CPU time: [0.983443 sec user] , [0.004003 sec system] (Terceiro tick)
CPU time: [1.315751 sec user] , [0.004003 sec system] (Quarto tick)
CPU time: [1.645706 sec user] , [0.004003 sec system] (Quinto tick)
CPU time: [1.973032 sec user] , [0.004003 sec system] (Sexto tick)

...

CPU time: [1.975300 sec user] , [0.004003 sec system]
CPU time: [1.977498 sec user] , [0.004003 sec system]
CPU time: [1.979657 sec user] , [0.004003 sec system]
CPU time: [1.981972 sec user] , [0.004003 sec system]
CPU time: [1.984138 sec user] , [0.004003 sec system]
CPU time: [1.986427 sec user] , [0.004003 sec system]
CPU time: [1.988616 sec user] , [0.004003 sec system]

=> CPU atingiu o limite de 2 segundos e finaliza.

Ele disparou a execução do timer a cada 333 ms?

Não, pois idealmente teriamos: 0.333, 0.666, 0.999, 1.332, 1,665, 1.998 (6 ticks).
Porém temos quasei isso: 0.326, 0.653, 0.983, 1.315, 1.645, 1.973 (6 ticks).

Como você verificou isso?

Utilizando struct rusage usage; int who = RUSAGE_SELF; getrusage(who, &usage);.

Quando ele começou a contar?

CPU time: 0.003508 sec user, 0.000000 sec system

Como você sabe quanto tempo se passou desde que o sinal SIGALRM
foi disparado pelo sistema operacional até o processo responder,
executando o procedimento?

Tem uma quantidade de ciclos de clocks que eu não consigo contar porém
a cada tick tem que passar 333 ms então cada intervalo tem apróximadamente isso.

Qual é a fração de tempo que o processo gastou executando
instruções do usuário e instruções do sistema operacional?

Pegando a última saída antes de os 2 segundos limites da CPU chegarem temos:

        User CPU time => [1.973694 sec user], [0.015956 sec system]

Mas esses resultados variam um pouco, porém é raríssimo passar de 2 segundos no CPU user time.
*/
