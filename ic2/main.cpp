#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>

#include <stdlib.h>

#define SHARED_OBJ_NAME     "ANNONYMOUS"
#define MAX_MSG_LENGTH      26

struct message_struct {
    pid_t sender;
    int type;
    char content[MAX_MSG_LENGTH];
};

using namespace std;

int main() {
    srand (clock());
    int uniqkey = rand()%100;



    int shared_object;
    int shared_seg_size = sizeof(struct message_struct);
    struct message_struct *shared_message;

    cout << "Parent process " << getpid() << " is creating a child process" << endl;

    /* duplicate process */
    pid_t child = fork();
    /* if error in duplicating */
    if (child < 0) {
        return -1;
    }
    /* if child-process running then */
    else if (child == 0) {

        cout << "Child process " << getpid() << " is creating a shared memory object to write a message in" << endl;

        //  int shmget(key_t key, int size, int shmflg);
        //  key pode ter IPC_PRIVATE = 0
        //  On success, mmap() returns a pointer to the mapped area

        /**
         * MAP_ANONYMOUS
         *     The mapping is not backed by any file; its contents are
         *     initialized to zero. The fd and offset arguments are ignored;
         *     however, some implementations require fd to be -1 if
         *     MAP_ANONYMOUS (or MAP_ANON) is specified, and portable
         *     applications should ensure this. The use of MAP_ANONYMOUS in
         *     conjunction with MAP_SHARED is supported on Linux only since
         *     kernel 2.4.
         *
         * MAP_SHARED
         *     Share this mapping.
         *
         * PROT_READ
         *     Pages may be read.
         *
         * PROT_WRITE
         *     Pages may be written.
         *
         */

        //  mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
        shared_object = shmget(uniqkey, shared_seg_size, 0);
        if (shared_object < 0) {
          cout << "Error "<< shared_object << " creating shared object" << endl;
          return -1;
        }



        /*  make room for the shared object to fit a message */
        /* map the shared onject to memory */
        shared_message = (struct message_struct *) shmat(shared_object, 0, 0);
        if (shared_message == NULL) {
          cout << "Error in memory map" << endl;
          return -1;
        }
        // shared_message = (struct message_struct *) mmap (NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
        //OFFSET = 0, OK.
        // if (shared_message == MAP_FAILED) {
        //    cout << "Error "<< shared_object << " creating shared object" << endl;
        //    return -1;
        // }

        /**
         * If addr is NULL, then the kernel chooses the address at which to
         * create the mapping; this is the most portable method of creating a
         * new mapping.
         */

        /* producing a message on the shared segment */
        shared_message->type = 1;
  	    shared_message->sender = getpid();

        /**
         * Esse stringcopy ignora e burla completamente o tamanho maximo
         * definido no cabecalho desse arquivo, isto e, MAX_MSG_LENGTH nao
         * tem influencia alguma. A minha mensagem tem 26 chars
         */
        // const char* s = "there is no hidden~ place";
        // shared_message->content = (char*) malloc (strlen(s) + 1);
        strcpy(shared_message->content, "there is no hidden~ place");

        cout << "Child process " << getpid() << " wrote message \"" << shared_message->content << "\" in memory" << endl;

        shmdt(shared_message);

        // free(shared_message);
        // shared_message->content = NULL;
        return 0;
    }

    /* if parent-process running then */
    else {
        cout << "Parent process " << getpid() << " is waiting for child to exit" << endl;

        int errno, status;

        /* wait for child process to exit and get its status */
        // waitpid(getpid(), &status, errno);
        // Knowing that its only one child executing after the father process
        // then its not really necessary to build a while statement.
        cout << "Parent process " << getpid() << ": Waiting children to exit" << endl;
        waitpid(getpid(), &status, errno);

  	    /* if status is not success */
  	    if (errno != ECHILD) {
  	        cout << "Parent process " << getpid() << " is exiting since child could not write message in memory" << endl;
  	        return -1;
  	    }

        /**
         * SHM_R (=0400) e SHM_W(=200) = 0600 que dão o direito de leitura e escrita ao segmento
         * create the shared object to read from
         */

         /**
          * Se por algum momento você tenta acessar a memória no "tempo"
          * errado, já buga o programa. Por exemplo um shared_message->sender
          * agora iria bugar o programa, pois a memória já não está mais
          * livre. Ou está fora de contexto?
          */
        cout << "Parent process " << getpid() << " will read message from process " <<
        child << " finished with status "<< status << endl;

        //IPC_CREAT - Create the segment if it doesn't already exist in the kernel.
        //IPC_CREAT | 0666 for a server (i.e., creating and granting read and write access to the server)
        //http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmget.html
        shared_object = shmget(uniqkey, shared_seg_size, IPC_CREAT | SHM_R | SHM_W);

        if (shared_object < 0) {
            // Valor de retorno: o identificador do segmento de memória compartilhada shmid, ou -1 em caso de erro.
            cout << "Error in shm_open()" << endl;
            return -1;
        }

        // void *shmat ( int shmid, const void *shmaddr, int shmflg )
        // Valor de retorno: endereço do segmento de memória compartilhada, ou -1 em caso de erro.
        // Se shmaddr é 0, o segmento é acoplado ao primeiro endereço possível determinado pelo sistema (caso mais comum na prática);
        shared_message = (struct message_struct *) shmat (shared_object, 0, 0);
        /* map the shared object to memory */

        if (shared_message == NULL) {
            cout << "Error in memory map" << endl;
            return -1;
        }

        wait(NULL);

        cout << "Parent process " << getpid() << " read the message '" << shared_message->content << "' from sender " << shared_message->sender << " in memory " << endl;

        shmdt(shared_message);

        // int munmap(void *addr, size_t length);
        // munmap(&shared_message, shared_seg_size);
        int removed = shmctl(shared_object, IPC_RMID, 0);
        if (removed != 0) {
            cout << "Error removing the shared object" << endl;
            return -1;
        }
    }
}
