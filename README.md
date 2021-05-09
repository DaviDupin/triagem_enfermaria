# triagem_enfermaria
O seguinte repositório contém o meu trabalho final para a disciplina de Programação Concorrente, cujo propósito era solucionar um problema que envolvesse condições de corrida. A solução apresentada a seguir é baseada no problema do Barbeiro Dorminhoco.

# Resumo do Problema
O problema apresentado pode ser resumido na seguinte situação:
na UBS há 1 enfermeiro, 2 consultórios e, em cada consultório, há um limite de vagas na fila de espera, tal que o paciente é mandado embora caso a fila esteja cheia para evitar aglomerações.

O enfermeiro não pode atender mais de um paciente ao mesmo tempo nem dar prioridade para um dos atendimentos, de forma que, após finalizado um atendimento de COVID, ele deve fazer a devida higienização de seus Equipamentos de Proteção Individual(EPI) e trocar de consultório para atender o próximo paciente ambulatorial, e vice-versa.

Se ambas as filas de espera estiverem vazias, o enfermeiro permanece na sala onde está aguardando a chegada do próximo paciente. Em caso da chegada de um paciente no outro consultório, se o enfermeiro estiver livre, ele deve mudar de consultório para realizar o atendimento.

Se um paciente chega, não há fila para o seu caso clínico, e o enfermeiro está no outro consultório realizando um atendimento, ele será o próximo a ser atendido.

# Análise Computacional
Computacionalmente, a solução empregada foi baseada no problema do Barbeiro Dorminhoco, pois, assim como naquele caso, aqui há um problema de sincronização de processos no qual é necessária a organização de semáforos em filas. A diferença entre o tradicional caso do barbeiro e esta aplicação é que, dentro da analogia, seria como se o barbeiro tivesse duas barbearias e precisasse alternar seu atendimento entre elas. Inicialmente, havia sido considerada a hipótese de ser uma mesma barbearia, porém com duas cadeiras de atendimento, porém não faz sentido nesse caso, porque as filas seriam compartilhadas caso fossem duas cadeiras dentro de uma mesma barbearia.

As principais diferenças estruturais entre o problema do barbeiro e o problema das UBS são a necessidade de duplicar todas as threads e controlá-las através de um mutex, para evitar que os atendimentos nos dois consultórios sejam feitos simultaneamente.

# Para compilar e executar
Foi utilizado o ambiente WSL2 para executar o código, compilado em C99.

```
gcc triagem_enfermaria.c -o output -lpthread
```

Depois, para executar:

```
./output
```

Obs: o nome usado como "output" pode ser livremente substituído por qualquer outro, desde que isso seja feito tanto na compilação como na execução (para aqueles que ainda não estão familiarizados com a sintaxe da linguagem C.

Obs2: a biblioteca "-lpthread" precisa ser incluída, caso contrário o código não irá funcionar.
