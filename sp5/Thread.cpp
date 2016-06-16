/*
 * File:   Thread.cpp
 * Author: <preencher>
 *
 * Created on September 27, 2015, 10:30 AM
 */

#include "Thread.h"
#include "Debug.h"
#include "System.h"
#include "Simulator.h"
#include "CPU.h"

Thread* Thread::_running;

Thread::Thread(Process* task, Thread::State state) {
    this->_id = Thread::getNewId();
    this->_process = task;
    this->_state = state;
    // INSERT YOUR CODE HERE
    // ...
}

Thread::Thread(const Thread& orig) {
}

Thread::~Thread() {
}

Thread* Thread::running() {
    return _running;
    //return System::scheduler()->choosen();
}

Process* Thread::getProcess() {
    return _process;
}

int Thread::getPriority() const {
    return _accountInfo._priority;
}

void Thread::sleep(Queue * q) {
    Debug::cout(Debug::Level::trace, "Thread::sleep(" + std::to_string(reinterpret_cast<unsigned long> (q)) + ")");
    // INSERT YOUR CODE HERE
    // ...
}

void Thread::wakeup(Queue * q) {
    Debug::cout(Debug::Level::trace, "Thread::wakeup(" + std::to_string(reinterpret_cast<unsigned long> (q)) + ")");
    // INSERT YOUR CODE HERE
    // ...
}

/**
 * Threads podem decidir deixar a CPU invocando a chamada void Thread::yield(), que precisa ser implementada. A implementação
 * dessa chamada deve colocar a thread que está executando no estado READY, incluí-la na fila de threads prontas, chamar
 * o escalonador para escolher outra thread, invocando Thread* Scheduler::choose() e então o despachador para iniciar
 * a execução da thread escolhida pelo escalonador, invocando static void Thread::dispatch(Thread* previous, Thread* next).
 **/
void Thread::yield() {
    Debug::cout(Debug::Level::trace, "Thread::yield()");
    // INSERT YOUR CODE HERE
    // ...
}

/**
 * Threads aguardarão pelo término de outras threads quando for invocada a chamada void Thread::join(), que precisa ser 
 * implementada. A implementação dessa chamada deve verificar se a thread this passada como parâmetro está no estado FINISHING 
 * ou não. Se estiver, então a thread que invocou a chamada (a que está executando --running--) simplesmente continua sua execução. 
 * Se não estiver, então a thread executando deve ser colocada no estado WAITING e colocada na fila de threads esperando pela 
 * thread this. Ainda nesse caso, deve-se chamar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose() 
 * e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando 
 * static void Thread::dispatch(Thread* previous, Thread* next). Para melhor ilustrar essa chamada, se o código da thread T1 
 * tiver a chamada T2->join(), então T1 é quem está executando (running) e T2 é quem foi invocada (this), e é T1 que deve ser 
 * bloqueada esperando por T2 (se T2 não estiver FINISHING).
 **/
int Thread::join() {
    Debug::cout(Debug::Level::trace, "Thread::join()");
    // INSERT YOUR CODE HERE
    // ...
}

/**
 * Threads serão finalizadas quando for invocada a chamada void Thread::exit(), que precisa ser implementada. A implementação
 * dessa chamada deve colocar a thread que está executando no estado FINISHED, verificar se há alguma thread na lista
 * de threads bloqueadas esperando por essa thread. Se houver, todas as threads na lista devem ser colocadas no estado
 * READY e colocadas na fila de threads pronas para executar. Em qualquer caso, deve-se ainda chamar o escalonador para
 * escolher outra thread, invocando Thread* Scheduler::choose() e então o despachador para iniciar a execução da thread
 * escolhida pelo escalonador, invocando static void Thread::dispatch(Thread* previous, Thread* next).
 **/
void Thread::exit(int status) {
    Debug::cout(Debug::Level::trace, "Thread::exit(" + std::to_string(status) + ")");
    // INSERT YOUR CODE HERE
    // ...
}

/**
 * Threads são despachadas, ou seja, têm seus contextos trocados, quando se invoca a chamada 
 * static void Thread::dispatch(Thread* previous, Thread* next), que precisa ser implementada. A implementaçao desse  método 
 * deve inicialmente atualizar o atributo _running,  verificar se a próxima thread (next) é nula ou não. 
 * Se for, nada precisa ser feito (isso só ocorre quando  a fila de prontos é vazia e não há thread para ser escalonada). 
 * Se não for, então a thread a ser executada precisa ser  colocada no estado RUNNING e retirada da fila de prontos. 
 * Então deve ser verificado se a thread anterior  (previous) é diferente de nula e também se é diferente da próxma thread. 
 * Se não for, então basta restaurar o contexto da  próxima thread, invocando static void CPU::restore_context(Thread* next). 
 * Se for, então é preciso verificar se a thread  anterior estava no estado RUNNING e caso sim, então a thread anterior deve 
 * passar para o estado READY. Após esse teste  deve-se fazer a troca de contexto entre as threads, invocando o método 
 * static void CPU::switch_context(Thread* previous, Thread* next).
 * **/
void Thread::dispatch(Thread* previous, Thread* next) {
     Debug::cout(Debug::Level::trace, "Thread::dispatch(" + std::to_string(reinterpret_cast<unsigned long> (previous)) + "," + std::to_string(reinterpret_cast<unsigned long> (next)) + ")");
    // INSERT YOUR CODE HERE
    // ...
    // o atributo _running deve ser atualizado
    _running = next;
    // verificar se a próxima thread (next) é nula ou não. Se for, nada precisa ser feito
    if (next != nullptr) {
        // a thread a ser executada precisa ser colocada no estado RUNNING
        next->_state = Thread::State::RUNNING;
        // e retirada da fila de prontos
        System::scheduler()->remove(next);
        // deve ser verificado se a thread anterior (previous) é diferente de nula e também se é diferente da próxma thread
        if ((previous != nullptr) && (previous != next)) {
            //  verificar se a thread anterior estava no estado RUNNING
            if (previous->_state == Thread::State::RUNNING) {
                // a thread anterior deve passar para o estado READY
                previous->_state = Thread::State::READY;
            }
            // fazer a troca de contexto entre as threads
            CPU::switch_context(previous, next);
        } else {
            // Se não for, então basta restaurar o contexto da próxima thread
            CPU::restore_context(next);
        }
    }
}

/**
 * Threads criarão novas threads quando for invocada a chamada static Thread* Thread::thread_create(), que precisa ser
 * implementada. A implementação dessa chamada deve criar um objeto Thread, inicializar seus novos atributos, colocá-la na
 * lista de threads no sistema, inseri-la na fila de prontos inovando o método void Scheduler::insert(Thread* thread), 
 * e se o escolador for preemptivo, então deve chamar o escalonador para escolher outra thread, invocando 
 * Thread* Scheduler::choose() e então o despachador para iniciar a execução da thread escolhida pelo escalonador, 
 * invocando static void Thread::dispatch(Thread* previous, Thread* next).
 **/
Thread* Thread::thread_create(Process* parent) {
    Debug::cout(Debug::Level::trace, "Thread::create(" + std::to_string(reinterpret_cast<unsigned long> (parent)) + ")");
    // INSERT YOUR CODE HERE
    // ...
}