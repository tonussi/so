# INE5412-04208A (20162) - Sistemas Operacionais I

## Discussion about Processes and Threads

Apresente um grafo que apresenta os principais estados em que um processo pode estar durante seu ciclo de vida.Para cada transição de estados, apresente os nomes das das chamadas de sistema (e apenas chamadas de sistema) de processos (pelo menos três), de threads (pelo menos quatro) e de sincronização (pelo menos duas) que podem causar essas mudanças de estados. Informe se a existência de alguma dessas transições depende do fato do algoritmo de escalonamento ser preemptivo ou não preemptivo.

Numa implementação eficiente da chamada fork() o espaço de endereçamento do processo pai é imediatamente duplicado? Explique como funcionaria uma alternativa eficiente a essa cópia em memória, assumindo que haja um mecanismo de gerenciamento de memória como segmentação. Explique o que um programador veria se analisasse o valor do ponteiro a uma variável global antes e depois de um processo filho alterá-la.

Sistemas que suportam múltiplas threads precisam tratar algumas questões importantes, como qual(is) thread(s) recebem(m) um sinal enviado ao processo, se as threads de um processo são duplicadas ou não quando uma chamada fork() é executada, e o que ocorre quando uma thread bloqueada é encerrada. Comente sobre as implicações (vantagens, desvantagens) das alternativas.

Algoritmos de escalonamento devem minimizar ou maximizar algum indicador de desempenho. Para isso, ordenam a execução processos (ou threads) conforme um critério específico, e podemos avaliá-los usando diferentes técnicas.

Descreva (cinco) indicadores de desempenho (métricas) adequados a escalonadores batch, interativos e de tempo-real. Comente sobre qual seria a principal vantagem de utilizar cada um.

Apresente o critério usado por algoritmos preemptivos para ordenar a execução de processos ou threads em: (b.1) sistemas iterativos (pelo menos seis) e (b.2) de tempo-real (pelo menos 3), e informe se eles têm prioridade estática ou dinâmica e se podem ou não causar postergação indefinida.

Descreva diferentes técnicas que podem ser usadas para avaliar um algoritmo de escalonamento, comentando sobre a principal vantagem de utilizar cada uma.

5. O escalonamento em máquinas multiprocessadas (UMA ou NUMA) trata de questões envolvendo balanceamento de carga e afinidade de processador.

Descreva o que são esses dois mecanismos, a motivação arquitetural para utilizá-las e suas  vantagens, não esquecendo de mencionar aspectos de comunicação (migração de processos) e porquê, frequentemente, o balanceamento de carga contrapõe os benefícios da afinidade de processador.

Dê exemplos de quando seria melhor usar um ou outro, incluindo critérios que você usaria para tomar uma decisão entre um ou outro.
6. Existem 4 condições necessárias (mas não suficientes) para a ocorrência de deadlock. Assim, bastaria o Sistema Operacional garantir que uma dessas condições não ocorre para prevenir a ocorrência de deadlocks.

Quais são essas condições e quais são as implicações ou complicações envolvidas em garantir que cada condição não ocorrerá?
Se a ocorrência de deadlock não for prevenida,como o Sistema Operacional pode detectar a ocorrência de deadlock e quais são as opções que ele tem para recuperar o sistema do estado de deadlock? Apresente as principais implicações (vantagens e desvantagens) de cada opção citada.
