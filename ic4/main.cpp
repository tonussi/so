/**
 * Semaphore meaning
 * wait(S) indivisible (until calling process is blocked)
 *
 * If S is positive (S > 0), decrement S and return.
 * Will not cause the process to block.
 *
 * If S is not positive, then the calling process is put to sleep
 * (blocked), until someone does a signal and this process is selected to wakeup
 *
 * Signal (S): indivisible (never blocks the calling process)
 * If there is one or more processes sleeping on S, then one process is selected
 * and waken up, and signal returns.
 *
 * If there is no process sleeping, then S is simply incremented by 1 and signal
 * returns.
 *
 * 1) Estado 1> Sete crianças quietas. 3 B | 4 NB
 * 2) Estado 2> 
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
/* ... other includes ... */
// add library to include miscellaneous functions
#include <unistd.h>
// add library to work with semaphores
#include <semaphore.h>
// add library to work with posix threads
#include <pthread.h>
// Nome do aluno: Lucas Tonussi

// note: semaphore has some operations that we dont need to use here
// because they are operations to work with multiple processes interacting
// with one another. here we just need to use sem_t, sem_init, sem_wait, and sem_post

using namespace std;

/* declare whenever global variables you need to synchronize pthreads using posix semaphores */
// first we need a semaphore regulating the flow of pids wanting information inside the basket

/* sem_t is a type of semaphore to use semaphore operations */
sem_t basket;

// this is simutaneosly equivalent to 7 kids trying to take a ball from the basket
#define PIDCHILDS_N 7

// this is the number of itens in the basket
#define BALL_N 3

// true: initiates with resource
// false: do not initiates with the resources
bool KIDS[PIDCHILDS_N] = {true, true, true, false, false, false, false};

// this is the running child thread (we will start 7 of these to race)
void *child (void *void_ptr) {

  unsigned short *childNum = (unsigned short *) void_ptr;

  for (unsigned short i=1; i<=5; i++) {
    cout << "Child " << *childNum << " wants to play with the ball for the " << i << "th time" << endl;

    /* if the child has no ball, need to take one from the basket if there is one, or will wait until there is a ball in the basket */
    cout << " Child " << *childNum << " wants to take a ball from the basket" << endl;

    /* the sem_wait or sem_trywait will fail if:

       [EAGAIN]  the semaphore was already locked
       [EDEADLK] a deadlock condition was detected
       [EINTR]   a signal interrupted this function
       [EINVAL]  the sem argument does not refer to a valid semaphore

       the sem_wait operation will perform a semaphore a lock in the basket semaphore
       if the semaphore value is currently zero, then the calling thread shall not
       return from the call to sem_wait until it either locks the semaphore or the
       call is interrupted by a signal.
    */
    if (KIDS[*childNum] == false) {
        sem_wait(&basket);
        KIDS[*childNum] = true;
    }

    /* once the child has a ball, he/she starts to play */
    cout << "  Child " << *childNum << " is playing with the ball" << endl;
    /* play with the ball for 1 second */

    sleep(1);
    
    cout << "  Child " << *childNum << " wants to leave the ball in the basket" << endl;

    /* when the child is tired of playing, he/she has to drop the ball into the basket, if there is room for it (basket holds only 3 balls), or will wait until another hild to take a ball */
    cout << " Child " << *childNum << " has droped the ball in the basket" << endl;

    /* sem_post function that we are using here unlock the semaphore
       is the semaphore value resulting from this operation is positive
       then no threads were blocked waiting for the semaphore to become
       unlocked, the semaphore value is simply incremented.

       to put in simple words:
            it increments the semaphore counter and wakes up
            a blocked process waiting on the semaphore, if any */

    if (KIDS[*childNum] == true) {
        sem_post(&basket);
        KIDS[*childNum] = false;
    }

  }
  cout << "Child " << *childNum << " will no longer play" << endl;  
  /* exit the thread*/
}

int main() {
    cout << "The kindengarten is open" << endl;
    
    /* declare local variable, such as threads
       pthread_t is used to identify a thread
     */
    pthread_t kids[PIDCHILDS_N];

    /* init semaphores to synchronize the threads */
    int semaphore_stats = 0;

    // &basket is a reference to the basket samaphore for we to init using it
    // the second parameter (called pshared) is zero, which means that this semaphore is SHARED between threads of this process, in other
    // words each one of the threads in this process can perform semaphore operations using the same semaphore (called basket)
    // the third value {BALL_N} have the number of itens in the basket semaphore, this means that the semaphore initiates at counting 3

    /* sem_init will get the semaphore to start  */
    semaphore_stats = sem_init(&basket, 0, BALL_N);

    // printf("this should be 0 (same as no errors) stats={%d}\n", semaphore_stats == 0);

    /* create 7 threads for the children, passing to each one a different number (child 0 to 6) */
    // we will create a array of 'kids' to play they will have the values from 0 to 6
    /* kids ids for threads have a distinct reference */

    //
    // This way we force the 3 first threads to take a ball if the data structure has 3 trues
    // for the 3 first threads
    //

    int id[] = {0, 1, 2, 3, 4, 5, 6};
    for (int k = 0; k < PIDCHILDS_N; ++k) {
        if (k < 3 && KIDS[k] == true) {
            pthread_create(&kids[k], NULL, child, (void *) &id[k]);
            sem_wait(&basket);
        } else {
            pthread_create(&kids[k], NULL, child, (void *) &id[k]);
        }
    }

    /* wait for all children to finish */
    for (int k = 0; k < PIDCHILDS_N; ++k) {
        /* the first input is the reference of what threads we want it to enter the join state
           the join state finishes the thread and gather the result to go in the second parameter
           but in this case we dont want to save any output from the child's running threads so
           we just leave it NULL
         */
        pthread_join(kids[k], NULL);
    }

    /* destroy semaphore because this is the end of the program */
    semaphore_stats = sem_destroy(&basket);

    // printf("this should be 0 (same as no errors) stats={%d}\n", semaphore_stats == 0);
    
    cout << "The kindengarten is closed" << endl;

    return 0;

    /* Respostas das perguntas "para pensar":
        its not a good ideia put local semaphores for this example because we need a global
        counter to show all threads that are runnning a reference regulating the basket so
        the threads might have to wait to pick an item from the basket, in this case a "ball".
        in this program all children threads are called to join so all threads are called to stop
    
        here in this exercise the up and down of the semaphore are being used like in a way
        that appears to be pthread_mutex_lock and pthread_mutex_unlock. to make a semaphore
        go 'up' we just make sem_post and to make a semaphore go 'down' we make sem_wait
    
        so it must happen threading racing for up's and down's in some way that the semaphore
        will go down 3 times sometime downards his global counter to 0, them no itens will be
        available in the basket, no thread will have itens to play
    
        joining a thread that has already exited is possible (i think), so by doing this
        will finally free the resource previously allocated for this thread to run, these
        threads are called zombies.
    
        to see if a thread have finished operations we have to use pthread_join() like
        i did, in a block fashion, making the main thread ask for the other ones at a time
        to join and finish.
    */
}
