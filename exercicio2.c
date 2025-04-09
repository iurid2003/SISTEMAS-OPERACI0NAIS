// ex1: fork/wait básico
/*Exercício status/wait

 O status passado como parâmetro à função wait(&status) é, na verdade, o mecanismo de retorno de resultado do wait/waitpid.
 o retornar, esta variável contém informações sobre o resultado da execução do processo filho. Por exemplo, se um processo terminou normalmente
(i.e., chamou exit), o comando WIFEXITED(status) retorna true. Este comando retorna false se o processo foi abortado (e.g., segmentation fault) ou morto
 (e.g., kill). Investigue no manual do wait no Linux (man wait) o funcionamento do comando WEXITSTATUS(status),
  e use-o para modificar o exercício anterior para calcular o 5!, sendo que cada processo pode executar apenas uma multiplicação.*/

  #include <stdlib.h>
  #include <stdio.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  
  int main() {
      pid_t pid_filho;
      int status;
      int resultado = 1;

  
      printf("Processo Pai (PID: %d) iniciando o cálculo de 5!\n", getpid());
  
      pid_filho = fork();
  
      if (pid_filho == -1) {
          perror("fork falhou!");
          exit(EXIT_FAILURE);
      } else if (pid_filho == 0) { // Processo filho
          printf("Processo Filho (PID: %d, Pai: %d) calculando 5!\n", getpid(), getppid());
          for (int i = 2; i <= 5; i++) {
              resultado *= i;
          }
          printf("Processo Filho (PID: %d) terminando com resultado: %d\n", getpid(), resultado);
          exit(resultado); // Retorna o resultado final
      } else { // Processo pai
          printf("Processo Pai (PID: %d) esperando pelo filho (PID: %d)...\n", getpid(), pid_filho);
          if (wait(&status) == -1) {
              perror("wait falhou!");
              exit(EXIT_FAILURE);
          }
  
          if (WIFEXITED(status)) {
              int resultado_filho = WEXITSTATUS(status);
              printf("Processo Pai: Filho (PID: %d) terminou normalmente com resultado: %d\n", pid_filho, resultado_filho);
          } else if (WIFSIGNALED(status)) {
              printf("Processo Pai: Filho (PID: %d) terminou devido a um sinal %d\n", pid_filho, WTERMSIG(status));
          } else {
              printf("Processo Pai: Filho (PID: %d) terminou de forma anormal\n", pid_filho);
          }
      }
  
      printf("Processo Pai (PID: %d) terminou.\n", getpid());
  
      return 0;
  }