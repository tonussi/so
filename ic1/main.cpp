#include <string>
#include <iostream>
/*! \brief <unistd.h> library here enables the use of unsigned int sleep(unsigned int seconds); */
#include <unistd.h>
#include <errno.h>
/*! \brief header includes definitions for at least the following types used in this program:
 *  \type pid_t: Used for process IDs and process group IDs. 
 */
#include <sys/types.h>
/*! \brief The <sys/wait.h> header defines the following symbolic constants for use with waitpid(): 
 *         WEXITSTATUS(): Return exit status. 
 */
#include <sys/wait.h>

using namespace std;
int main ()
{

    // include declarations you need
    pid_t *processos_filhos = new pid_t[5];
    int count = 0;
    cout << "Count do processo pai: " << &count << endl;

    /* Repeat 5 times */
    int i = 0;
    for (;i<5;++i) {

        cout << "Parent process " << getpid() << ": Creating child" << endl;

        /* Duplicate this process
         * create 5 childs in calling sequence
         */
        processos_filhos[i] = fork();

        /* If there was an error on duplication then
        *
        * The value of pid can be:
        *  < -1 meaning wait for any child process whose process group ID is equal to the absolute value of pid.
        *    -1 meaning wait for any child process
        *     0 meaning wait for any child process whose process group ID is equal to that of the calling process.
        *  >  0 meaning wait for the child whose process ID is equal to the value of pid. 
        *
        */
        if(*(processos_filhos + i) < 0) {
            cout << "Error on fork()" << endl;
            return -1;
        }

        /* If child-process is running then */
        if (*(processos_filhos + i) == 0) {

            /*! \brief: get process identification
             *  \return: pid_t process identification;
             */
            cout << "Child process " << getpid() << ": Running" << endl;

            printf("Antes do incremento: %p\n", &count);
            count++;
            printf("Depois do incremento: %p\n", &count);

            /* Sleep for 1 second  POSIX.1-2001. */
            /*! \brief unsigned int sleep(unsigned int seconds);
             *         sleep() may be implemented using SIGALRM;
             *         mixing calls to alarm() and sleep() is a
             *         bad idea. (Source: http://linux.die.net/man/3/sleep)
             *
             *  \return ZERO VALUE (0) if the requested time has elapsed,
             *          or the number of seconds left to sleep,
             *          if the call was interrupted by a signal handler.
             */
            sleep(1);

            cout << "Child process " << getpid() << ": Exiting with status " << count << endl;

            /*! \brief: The exit() function causes normal process termination
             * here we exit(count) to make the process return the his +=1 count
             * in other words, the number 1 to sum up to 5 because we have 5 child
             * processes
             *
             *
             * The exit() function does not return. We pick the value
             * using the MACRO named WEXITSTATUS(status)
             */
            exit(count);
        }

    }

    /* if this is the parent-process then */

    cout << "Parent process " << getpid() << ": Waiting children to exit" << endl;

    /*! \var errno: number of last error */
    int errno, status, sum = 0;

    /* Parent-process waits for all children to exit, adding each status to the sum variable */
    i = 0;
    for (; i < 5; i++) {
        /*! \brief wait for process to change state 
         *  \param pid_t pid: process id
         *  \param int *stat_loc: 
         * 
         *  \prototype: pid_t waitpid(pid_t pid, int *stat_loc, int options);
         *
         *  \errors: The common error is the following
         *           The wait() function will fail if: [ECHILD]
         *           The calling process has no existing unwaited-for child processes. 
         */
        waitpid(*(processos_filhos + i), &status, 0);
        cout << "Parent process " << getpid() << ": Waiting children to exit" << endl;

        /*!
         * \brief WEXITSTATUS(status)
         * \return: the exit status of the child.
         *          the least significant 8 bits of the status argument)
         *          specified in a call to exit()
         */
        sum += WEXITSTATUS(status);
    }

    cout << "Parent process " << getpid() << ": Exiting with sum " <<sum << endl;
    cout << "Count do pai: " << count << endl;
    cout << "Count do processo pai: " << &count << endl;

   /*  */
   return count;
}

/*
 * 1) Há uma variável global count acessível a todos os processos
 *    e que é inicializada com 0. Cinco processos são criados e cada
 *    um incrementa o valor atual dessa variável, retornando o valor
 *    incrementado. Se somarmos todos os valores retornados, quanto
 *    você acha que será apresentado?
 *
 *    R: Essa variavel global count não é acessível a todos os processos pois
 *       o sistema operacional protege a área de memória copiada para os processos filhos
 *       bem como protege a área de memória do processo pai, a razão disso ser verdade
 *       é que count do processo pai está com o valor 0 ao final. O resultado final é 5
 *       teóricamente, se a memória "fosse" compartilhada teríamos 0+1+2+3+4+5=15 ao final.
 *
 * 2) Existe possibilidade de condição de corrida neste código?
 *       Não, o programa inteiro é duplicado. Significa que existem 6 programas rodando
 *       o programa pai que faz 5 fork() e cria 5 processos filhos (que são iguais ao pai)
 *       mesmo sendo iguais, existem IF's que para identificar que tipo de programa é, se é
 *       filho ou se é pai. Então, cada processo filho tem seu próprio espaço de endereçamento
 *       e o pai também tem seu próprio espaço de endereçamento, e que são restringídos pelo S.O.
 *       para haver segurança de acesso à leitura ou escrita. Existe a possibilidade de criar um túnel
 *       de comunicação entre processos filhos, porém deverá haver troca de mensagens para identificar
 *       que o processo pode estár lendo e escrevendo no mesmo espaço de endereçamento de memória que outro
 *       isso é chamado de memória compartilhada e existem chamadas de sistemas comuns para isso.
 *
 * 3) Se você mandar apresentar a tela o ponteiro da variável "count" antes do fork
 *    e então depois do fork, quando um novo processo é criado, o que você espera ver
 *    nos endereços? Eles serão iguais ou diferentes? Por quê?
 *
 *    Todos iguais, pois o SO mapeia os códigos junto com dados (cópia do espaço de
 *    endereçamento), isto é, mapeia o SS-segmento de pilha, SP-stack pointer, etc
 *    para o mesmo endereço virtual que é diferente do endereço físico, certamente
 *    os programas após o fork() estão duplicados em locais distintos da memória
 *    não sendo o mesmo endereço físico dessa memória, porém o SO mapeia todos esses
 *    espaços de endereçamento para o mesmo endereço virtual. Crazy stuff.
 *
 *    output:
 *          Antes do incremento: 0x7ffe4a1740e8
 *          Parent process 20135: Creating child
 *          Depois do incremento: 0x7ffe4a1740e8
 *          Parent process 20135: Waiting children to exit
 *          Child process 20140: Running
 *          Child process 20139: Running
 *          Antes do incremento: 0x7ffe4a1740e8
 *          Depois do incremento: 0x7ffe4a1740e8
 *          Antes do incremento: 0x7ffe4a1740e8
 *          Depois do incremento: 0x7ffe4a1740e8
 *          Child process 20136: Exiting with status 1
 *          Child process 20137: Exiting with status 1
 *          Child process 20139: Exiting with status 1
 *          Parent process 20135: Waiting children to exit
 *          Parent process 20135: Waiting children to exit
 *          Child process 20138: Exiting with status 1
 *          Child process 20140: Exiting with status 1
 *          Parent process 20135: Waiting children to exit
 *          Parent process 20135: Waiting children to exit
 *          Parent process 20135: Waiting children to exit
 *          Parent process 20135: Exiting with sum 5
 *          Count do pai: 0
 *          Count do processo pai: 0x7ffe4a1740e8
 */
