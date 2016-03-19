#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;
int main ()
{

    // include declarations you need
    pid_t *processos_filhos = new pid_t[5];
    int count = 0;

    /* Repeat 5 times */
    int i = 0;
    for (;i<5;++i) {

        cout << "Parent process " << getpid() << ": Creating child" << endl;

        /* Duplicate this process */
        processos_filhos[i] = fork();

        /* If there was an error on duplication then */
        if(*(processos_filhos + i) < 0) {
            cout << "Error on fork()" << endl;
            return -1;
        }

        /* If child-process is running then */
        if (*(processos_filhos + i) == 0) {

            cout << "Child process " << getpid() << ": Running" << endl;

            try {
                count++;
            } catch (const std::exception& e) {
                return -2;
            }

            /* Sleep for 1 second */
            sleep(1);

            cout << "Child process " << getpid() << ": Exiting with status " << count << endl;

            return count;
        }

    }

    /* if this is the parent-process then */

    cout << "Parent process " << getpid() << ": Waiting children to exit" << endl;

    int errno, status, sum = 0;

    /* Parent-process waits for all children to exit, adding each status to the sum variable */
    i = 0;
    for (; i < 5; i++) {
        while (waitpid(*(processos_filhos + i), &status, 0) == -1 && errno != ECHILD) {
            cout << "Parent process " << getpid() << ": Waiting children to exit" << endl;
        }
        sum += WEXITSTATUS(status);
    }

    cout << "Parent process " << getpid() << ": Exiting with sum " <<sum << endl;

   /*  */
   return count;
}
