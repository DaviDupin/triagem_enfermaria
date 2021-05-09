/*
 * Baseado no problema do barbeiro dorminhoco.
 */ 
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_PACIENTES_COVID 30
#define FILA_ESPERA_COVID 5

#define N_PACIENTES_ENFERMARIA 30
#define FILA_ESPERA_ENFERMARIA 5

sem_t sem_fila_covid; //sinaliza se ha vagas para COVID
sem_t sem_fila_enfermaria; //sinaliza se ha vagas para ENFERMARIA

sem_t sem_atendimento_covid; //controla quem sera atendido na fila COVID
sem_t sem_paciente_atendido_covid; //sinaliza o fim do atendimento COVID
sem_t sem_paciente_chamado_covid; //controla a ordem da fila COVID

sem_t sem_atendimento_enfermaria; //controla quem sera atendido na fila ENFERMARIA
sem_t sem_paciente_atendido_enfermaria; //sinaliza o fim do atendimento ENFERMARIA
sem_t sem_paciente_chamado_enfermaria; //controla a ordem da fila ENFERMARIA

pthread_mutex_t mutex_sala_atendimento = PTHREAD_MUTEX_INITIALIZER; //usado para realizar a troca de consultorio entre COVID e ENFERMARIA

void * f_enfermaria_covid(void *v) {

  while(1) {
    sleep(1);
    pthread_mutex_lock(&mutex_sala_atendimento);
      printf("Enfermeiro está trocando de consultório [COVID]\n");
      sem_wait(&sem_paciente_chamado_covid);
      
      sleep(1);
      printf("Atendimento realizado [COVID]\n"); 
      
      sem_post(&sem_paciente_atendido_covid);
    pthread_mutex_unlock(&mutex_sala_atendimento);
  }
  pthread_exit(0);
}

void * f_enfermaria_comum(void *v) { //realiza o atendimento do paciente da ala convencional

  while(1) {
    sleep(2);
    pthread_mutex_lock(&mutex_sala_atendimento);
      printf("Enfermeiro está trocando de consultório [ENFERMARIA]\n");
      sem_wait(&sem_paciente_chamado_enfermaria);
      
      sleep(3);
      printf("Atendimento realizado [ENFERMARIA]\n"); 
      
      sem_post(&sem_paciente_atendido_enfermaria);
    pthread_mutex_unlock(&mutex_sala_atendimento);
  }
  pthread_exit(0);
}

void* f_paciente_covid(void* v) { //controla a entrada e espera dos pacientes da ala COVID
  int id = *(int*) v;
  sleep(id%3);
  if(sem_trywait(&sem_fila_covid) == 0){
    printf("Paciente %d chegou ao posto [COVID]\n",id);
    sem_wait(&sem_atendimento_covid);
    printf("Paciente %d é o próximo [COVID]\n",id);
    sem_post(&sem_fila_covid);
    sem_post(&sem_paciente_chamado_covid);
    sem_wait(&sem_paciente_atendido_covid);
    sem_post(&sem_atendimento_covid);
    printf("Paciente %d foi liberado [COVID]\n",id);

  }else{
	printf("Posto de saúde lotado, paciente %d evitou aglomeração [COVID]\n",id);
  }

  pthread_exit(0);
}

void* f_paciente_enfermaria(void* v) { //controla a entrada e espera dos pacientes da ala convencional
  int id = *(int*) v;
  sleep(id%9);
  if(sem_trywait(&sem_fila_enfermaria) == 0){
    printf("Paciente %d chegou ao posto [ENFERMARIA]\n",id);
    sem_wait(&sem_atendimento_enfermaria);
    printf("Paciente %d é o próximo [ENFERMARIA]\n",id);
    sem_post(&sem_fila_enfermaria);
    sem_post(&sem_paciente_chamado_enfermaria);
    sem_wait(&sem_paciente_atendido_enfermaria);
    sem_post(&sem_atendimento_enfermaria);
    printf("Paciente %d foi liberado [ENFERMARIA]\n",id);

  }else{
	printf("Posto de saúde lotado, paciente %d evitou aglomeração [ENFERMARIA]\n",id);
  }

  pthread_exit(0);
}

int main() {
  pthread_t thr_pacientes_covid[N_PACIENTES_COVID], thr_pacientes_enfermaria[N_PACIENTES_ENFERMARIA], thr_enfermaria_covid, thr_enfermaria_comum;
  int i, j, id_covid[N_PACIENTES_COVID], id_enfermaria[N_PACIENTES_ENFERMARIA];

  sem_init(&sem_fila_covid, 0, FILA_ESPERA_COVID);
  sem_init(&sem_fila_enfermaria, 0, FILA_ESPERA_ENFERMARIA);

  sem_init(&sem_atendimento_covid, 0, 1);
  sem_init(&sem_paciente_chamado_covid, 0, 0);
  sem_init(&sem_paciente_atendido_covid, 0, 0);

  sem_init(&sem_atendimento_enfermaria, 0, 1);
  sem_init(&sem_paciente_chamado_enfermaria, 0, 0);
  sem_init(&sem_paciente_atendido_enfermaria, 0, 0);
  
  for (i = 0; i < N_PACIENTES_COVID; i++) {
    id_covid[i] = i;
    pthread_create(&thr_pacientes_covid[i], NULL, f_paciente_covid, (void*) &id_covid[i]);
  }

  for (i = 0; i < N_PACIENTES_ENFERMARIA; i++) {
    id_enfermaria[i] = i;
    pthread_create(&thr_pacientes_enfermaria[i], NULL, f_paciente_enfermaria, (void*) &id_enfermaria[i]);
  }

  pthread_create(&thr_enfermaria_covid, NULL, f_enfermaria_covid, NULL);
  pthread_create(&thr_enfermaria_comum, NULL, f_enfermaria_comum, NULL);
  
  for (i = 0; i < N_PACIENTES_COVID; i++){
    for (j = 0; j < N_PACIENTES_ENFERMARIA; j++){
      pthread_join(thr_pacientes_covid[i], NULL);
      pthread_join(thr_pacientes_enfermaria[j], NULL);
    }
  }
  
  return 0;
}

