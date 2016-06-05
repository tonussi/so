/*
 * File:   MemoryManager.cpp
 * Authors: Lucas e Ilê
 * Strategy: The strategy that we are using here is create specialized to each
 *           Allocation Algorithm {First Fit, Worst Fit, Best Fit, Next Fit}
 *           Basically there are two Linked Lists called SimpleList, that we
 *           store separatelly Used Partitions Blocks and Unused Partitions Blocks.
 *
 * Created on ...
 */
#include "Debug.h"
#include "MemoryManager.h"
#include "Simulator.h"
#include "Traits.h"

#include <iostream>

#define NOTHING_ALLOCATED -1

MemoryManager::MemoryManager(MemoryAllocationAlgorithm algorithm) : _algo(algorithm) {
    // _algo is 'setted' on the same line from the constructor.

    struct Traits<MemoryManager> configuration;

    _listPartitions = new SimpleList();
    _indexLastPartition = 0;

}

MemoryManager::MemoryManager(const MemoryManager& orig) {}
MemoryManager::~MemoryManager() {}

Partition* MemoryManager::firstFit(unsigned int size) {
    
    // Partição a ser usada
    Partition* p = (Partition*) nullptr;
    // Endereços de inicio e fim da partição
    unsigned int startAddr = 0;
    unsigned int endAddr = size - 1;
    
    //std::cout << "_list size { " << _listPartitions->getSize() << " } " << std::endl;
    
    // variável usada para saber se os valores já foram setados
    bool setted = false;
    
    // variável usada na iteração para saber onde inserir na lista
    int i = 0;
    
    // Se a lista de partições não for vazia
    if(_listPartitions->getSize()) {
        // valores auxiliares no cálculo
        unsigned int begin = 0;
        unsigned int aux_begin = 0;
        unsigned int end = 0;
        unsigned int aux_end = 0;
        for(i = 0 ; i < _listPartitions->getSize() ; i++) {
            // pegamos o endereço de inicio da partição
            aux_begin = _listPartitions->getPartition(i)->getBeginAddress();
            // pegamos o endereço de final da partição
            aux_end = _listPartitions->getPartition(i)->getEndAddress();
            
            // Caso o endereço de começo da partição seja maior que o final da 
            // partição anterior analisada, ou estejamos com um 'buraco' no
            // começo das partições
            if(aux_begin > end) {
                // se a partição couber no espaço ali existente
                if(aux_begin - end >= size) {
                    // o começo da partição é no fim da última
                    startAddr = end + 1;
                    // o fim da partição é antes do começo da próxima
                    endAddr = startAddr + (size - 1);
                    // paramos o laço
                    setted = true;
                    break;
                }
            }
            // corrigimos os valores anteriores
            begin = aux_begin;
            end = aux_end;
        }
            
        if(!setted) {
            // setamos os valores de inicio e fim das partições se não estão setados ainda
            startAddr = end + 1;
            endAddr = startAddr + (size - 1);
        }
    }
    
    // Partição criada com endereços
    //std::cout << "startAddr { " << startAddr << " }, endAddr { " << endAddr << " }" << std::endl;
    p = new Partition(startAddr, endAddr, false);
    // Colocamos a partição na lista
    Element* el = new Element(p);
    _listPartitions->insertSpecific(el, i);
    
    // retornamos a partição criada
    return p;
    
}

Partition* MemoryManager::nextFit(unsigned int size) {
        // Partição a ser usada
    Partition* p = (Partition*) nullptr;
    // Endereços de inicio e fim da partição
    unsigned int startAddr = 0;
    unsigned int endAddr = size - 1;
    
    //std::cout << "_list size { " << _listPartitions->getSize() << " } " << std::endl;
    
    // variável usada para saber se os valores já foram setados
    bool setted = false;
    
    // variável usada na iteração para saber onde inserir na lista
    int i = 0;
    // variável usada para iterar pelas partições
    int e = _indexLastPartition;
    
    // Se a lista de partições não for vazia
    if(_listPartitions->getSize()) {
        // valores auxiliares no cálculo
        unsigned int begin = 0;
        unsigned int aux_begin = 0;
        unsigned int end = 0;
        unsigned int aux_end = 0;
        
        // se a partição não for a primeira, devemos preparar os valores
        if(e != 0) {
            // pegamos o endereço de inicio da partição anterior
            begin = _listPartitions->getPartition(e-1)->getBeginAddress();
            // pegamos o endereço de final da partição anterior
            end = _listPartitions->getPartition(e-1)->getEndAddress();
            //std::cout << ">>> e!=0 : begin { " << begin << " }, end { " << end << " }" << std::endl;
        }
        for(i = 0 ; i < _listPartitions->getSize() ; i++) {
            // offset para partições
            e = i + _indexLastPartition;
            // aqui pegamos o valor modular. Se tentar acessar um valor maior do que o 
            // tamanho da lista de partições, a aritimética modular faz este valor 
            // voltar a numeros mais baixos.
            e = e % _listPartitions->getSize();
            // pegamos o endereço de inicio da partição
            aux_begin = _listPartitions->getPartition(e)->getBeginAddress();
            // pegamos o endereço de final da partição
            aux_end = _listPartitions->getPartition(e)->getEndAddress();
            //std::cout << ">>> new  : aux_begin { " << aux_begin << " }, aux_end { " << aux_end << " }" << std::endl;
            
            // Caso o endereço de começo da partição seja maior que o final da 
            // partição anterior analisada, ou estejamos com um 'buraco' no
            // começo das partições
            if(aux_begin > end) {
                //std::cout << ">>> entrou 1" << std::endl;
                // se a partição couber no espaço ali existente
                if(aux_begin - end >= size) {
                    //std::cout << " aux_begin { " << aux_begin << " }, end { " << end << " }" << std::endl;
                    // o começo da partição é no fim da última
                    startAddr = end + 1;
                    // o fim da partição é antes do começo da próxima
                    endAddr = startAddr + (size - 1);
                    // paramos o laço
                    setted = true;
                    break;
                }
            }
            // corrigimos os valores anteriores
            begin = aux_begin;
            end = aux_end;
            
            if(e == 0 && i != 0) {
                //std::cout << ">>> entrou 2" << std::endl;
                begin = 0;
                end = 0;
            }
        }
        // caso os valores ainda não tenham sido setados
        if(!setted) {
            // setamos os valores de inicio e fim das partições caso não estejam
            // setados ainda. Este é o caso onde não existe partições livres do tamanho
            // desejado e precisamos alocar ao final da lista.
            int list_size = _listPartitions->getSize();
            // pegamos o endereço de final da partição
            end = _listPartitions->getPartition(list_size - 1)->getEndAddress();
            // acertamos os valores para a última partição
            startAddr = end + 1;
            endAddr = startAddr + (size - 1);
        }
    }
    
    // Partição criada com endereços
    //std::cout << "startAddr { " << startAddr << " }, endAddr { " << endAddr << " }" << std::endl;
    p = new Partition(startAddr, endAddr, false);
    // Colocamos a partição na lista
    Element* el = new Element(p);
    // setamos a última partição usada. Se e for igual ao tamanho da lista
    if(i == _listPartitions->getSize()) {
        // setamos para a última partição
        e = i;
    }
    // setamos a última partição
    _indexLastPartition = e;

    // inserimos nossa nova partição
    _listPartitions->insertSpecific(el, _indexLastPartition);
    //std::cout << "index and lenght =  { " << _indexLastPartition << ", " << p->getLength() <<" } " << std::endl;
    
    // adicionamos +1 ao índice de última partição usada
    _indexLastPartition++;
    // se a partição inserida não for a última partição da lista
    if(_indexLastPartition != _listPartitions->getSize()) {
        // somamos +1 para que a próxima partição efetiva seja buscada
        _indexLastPartition++;
    }
    
    // retornamos a partição criada
    return p;
}

// @import{ MemoryManager::firstFit commentaries }
// Best fit looks at the entire list and picks the smallest block
// that is at least as large as the request
Partition* MemoryManager::bestFit(unsigned int size) {

    // partição a ser criada
    Partition* p = (Partition*) nullptr;
    // endereços de alocação da partição
    unsigned int startAddr = 0;
    unsigned int endAddr   = size - 1;

    // variável usada para saber se já foi setada a partição
    bool setted = false;

    int i = 0;

    // hipótese: assumimos que o final da memória é a melhor partição
    unsigned int indexBestPartition = _listPartitions->getSize();

    // Se a lista de partições não for vazia
    if(_listPartitions->getSize()) {

        // valores auxiliares
        unsigned int begin     = 0;
        unsigned int aux_begin = 0;
        unsigned int end       = _listPartitions->getPartition(0)->getEndAddress();
        unsigned int aux_end   = 0;
        unsigned int sum       = 9999999;
        unsigned int sum_aux   = 0;

        for (i = 1 ; i < _listPartitions->getSize() ; i++) {

            aux_begin = _listPartitions->getPartition(i)->getBeginAddress();
            aux_end   = _listPartitions->getPartition(i)->getEndAddress();

            // se existir algum espaço entre a partição atual e a antiga partição
            sum_aux = aux_begin - end;

            // se existir um espaço entre partições
            if(aux_begin > end) {

                // se o espaço existir e for maior ou igual ao espaço necessário para
                // alocação 
                if(sum_aux >= size) {
                    
                    // se o espaço for menor do que o último espaço identificado
                    // como bom para alocar
                    if(sum_aux < sum) {
                        // esta é a melhor partição até agora para se inserir
                        indexBestPartition = i;
                        // o espaço é o com menor fragmentação
                        sum = sum_aux;
                        // atualizamos os valores
                        startAddr = end + 1;
                        endAddr = startAddr + (size - 1);
                        // avisamos que os valores estão setados
                        setted = true;
                        // se o tamanho for IDEAL, paramos o laço
                        if(sum == size) {
                            break;
                        }
                    }
                }
            }

            // atualizamos os valores
            begin = aux_begin;
            end   = aux_end;

        }

        // caso os valores ainda não tenham sido setados, setamos
        if(!setted) {
            startAddr = end + 1;
            endAddr   = startAddr + (size - 1);
        }
    }
    
    //std::cout << "startAddr { " << startAddr << " }, endAddr { " << endAddr << " }" << std::endl;
    // criamos a nova partição
    p = new Partition(startAddr, endAddr, false);

    // adicionamos no melhor espaço possível
    Element* el = new Element(p);
    _listPartitions->insertSpecific(el, indexBestPartition);

    return p;

}

// @import{ MemoryManager::firstFit commentaries }
// Like best fit, worst fit requires searching the entire
// freelist at each memory request to find the largest block
Partition* MemoryManager::worstFit(unsigned int size) {

    // partição a ser criada
    Partition* p = (Partition*) nullptr;
    // endereços de alocação da partição
    unsigned int startAddr = 0;
    unsigned int endAddr   = size - 1;

    // variável usada para saber se já foi setada a partição
    bool setted = false;

    int i = 0;

    // hipótese: assumimos que o final da memória é a pior partição
    unsigned int indexBestPartition = _listPartitions->getSize();

    // Se a lista de partições não for vazia
    if(_listPartitions->getSize()) {

        // valores auxiliares
        unsigned int begin     = 0;
        unsigned int aux_begin = 0;
        unsigned int end       = _listPartitions->getPartition(0)->getEndAddress();
        unsigned int aux_end   = 0;
        unsigned int sum       = 0;
        unsigned int sum_aux   = 9999999;

        for (i = 1 ; i < _listPartitions->getSize() ; i++) {

            aux_begin = _listPartitions->getPartition(i)->getBeginAddress();
            aux_end   = _listPartitions->getPartition(i)->getEndAddress();

            // se existir algum espaço entre a partição atual e a antiga partição
            sum_aux = aux_begin - end;

            // se existir um espaço entre partições
            if(aux_begin > end) {

                // se o espaço existir e for maior ou igual ao espaço necessário para
                // alocação 
                if(sum_aux >= size) {
                    
                    // se o espaço for maior do que o último espaço identificado
                    // como bom para alocar
                    if(sum_aux > sum) {
                        // esta é a pior partição até agora para se inserir
                        indexBestPartition = i;
                        // o espaço é o com maior fragmentação
                        sum = sum_aux;
                        // atualizamos os valores
                        startAddr = end + 1;
                        endAddr = startAddr + (size - 1);
                        // avisamos que os valores estão setados
                        setted = true;
                    }
                }
            }

            // atualizamos os valores
            begin = aux_begin;
            end   = aux_end;

        }

        // caso os valores ainda não tenham sido setados, setamos
        if(!setted) {
            startAddr = end + 1;
            endAddr   = startAddr + (size - 1);
        }
    }
    
    //std::cout << "startAddr { " << startAddr << " }, endAddr { " << endAddr << " }" << std::endl;
    // criamos a nova partição
    p = new Partition(startAddr, endAddr, false);

    // adicionamos no melhor espaço possível
    Element* el = new Element(p);
    _listPartitions->insertSpecific(el, indexBestPartition);

    return p;

}


Partition* MemoryManager::allocateMemory(unsigned int size) {
    Debug::cout(Debug::Level::trace, "MemoryManager::allocateMemory(" + std::to_string(size) + ")");
    // INSERT YOUR CODE TO ALLOCATE MEMOTY FOR THE PROCESS

    Partition* p = nullptr;

    std::cout << "call" << std::endl;
    // ############################################ //
    switch(_algo) {

		case FirstFit:
		    p = firstFit(size);
			break;

		case NextFit:
            p = nextFit(size);
			break;

		case BestFit: 
            p = bestFit(size);
			break;

		case WorstFit:
            p = worstFit(size);
			break;

		default:
		    break;
	}
    // ############################################ //

    //std::cout << "Partition =  { Length: " << p->getLength() << " }" << std::endl;

    return p; // CHANGE TO THE CORRET RETURN VALUE
}

void MemoryManager::deallocateMemory(Partition* partition) {
    Debug::cout(Debug::Level::trace, "MemoryManager::deallocateMemory(" + std::to_string(reinterpret_cast<unsigned long> (partition)) + ")");
    // INSERT YOUR CODE TO DEALLOCATE MEMORY OF THE PROCESS

    unsigned int size = _listPartitions->getSize();

    // Se a lista de partições não for vazia
    if(size) {
        
        // Partição usada para comparação
        Partition* p;
        
        // bucamos aqui, pela partição
        int i = 0;
        for(i = 0 ; i < size ; i++) {
            // percorrendo a lista
            p = _listPartitions->getPartition(i);
            
            // se a partição for a partição que queremos
            if(p->getBeginAddress() == partition->getBeginAddress()) {
                // removemos a partição e paramos o laço
                _listPartitions->removeSpecific(i);
                
                // se o algoritmo for NextFit e a partição removida for a última alocada
                if(_algo == NextFit) {
                    // corrigimos a última visitada
                    _indexLastPartition--;
                }
                //std::cout << "partition and _index { " << i << ", " << _indexLastPartition << " } " << std::endl;
                return;
            }
        }
        
    }
    
}

unsigned int MemoryManager::getNumPartitions() {
    // INSERT YOUR CODE TO RETURN THE QUANTITY OF PARTITIONS
    // ...
    return _listPartitions->getSize();
}

Partition* MemoryManager::getPartition(unsigned int index) {
    // INSERT YOUR CODE TO RETURN THE PARTITION AT INDEX index
    // ...
    // p is a return partition
    Partition* p = _listPartitions->getPartition(index);
    return p;
}

void MemoryManager::printPartition(Partition* p) {
    std::cout << p->getBeginAddress() << "-" << p->getEndAddress() << ": ALLOCATED " << p->getLength() << std::endl;
}

void MemoryManager::printFreeSpace(unsigned int start, unsigned int end) {
    std::cout << start << "-" << end << ": FREE " << end - start + 1 << std::endl;
}

void MemoryManager::showMemory() {
    // INSERT YOUR CODE TO SHOW THE MEMORY MAP, IN THE FOLLOWING FORMAT
    // <beginAddress>-<endAddress>: <FREE|ALLOCATED> <size>

    // Exemplo:
    /*
      0-1499:FREE 1500
      1500-1999:ALLOCATED 500
      2000-2999:ALLOCATED 1000
      3000-9999:FREE 7000
      10000-19999:ALLOCATED 10000
      20000-1000000:FREE 800000
    */

    // INSERT YOUR CODE TO SHOW THE MEMORY MAP
    // ...
    
    // tamanho da lista
    unsigned int size = _listPartitions->getSize();
    
    /*######################################*/
    switch(_algo) {

		case FirstFit:
            std::cout << ">>FirstFit" << std::endl;
			break;

		case NextFit:
            std::cout << ">>NextFit" << std::endl;
			break;

		case BestFit: 
            std::cout << ">>BestFit" << std::endl;
			break;

		case WorstFit:
            std::cout << ">>WorstFit" << std::endl;
			break;

		default:
		    break;
	}
	/*######################################*/
	
	Partition* p;
	Partition* next;
    int i = 0;
    for(i = 0 ; i < size ; i++) {
        // pegamos a partição em questão
        p = _listPartitions->getPartition(i);
        // caso for a primeira partição e exista um 'buraco' antes dela
        if(i == 0 && p->getBeginAddress() > 0) {
            // printe espaço livre
            printFreeSpace(0, p->getBeginAddress() - 1);
        }
        // printar partição
        printPartition(p);
        // se não for a última partição
        if(i+1 < size) {
            // pegamos a próxima partição
            next = _listPartitions->getPartition(i+1);
            // pegamos valores
            int end = p->getEndAddress();
            int beg = next->getBeginAddress();
            int result = beg - end;
            // se o resultado for maior que 1, existe espaço entre as partições
            if(result > 1) {
                printFreeSpace(p->getEndAddress() + 1, next->getBeginAddress() - 1); 
            }
        }
    }
    
}
