// ex1: fork/wait básico
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{

    int a, b, c, status;
    a = fork();

    if (a == -1) // fork falhou
    {
        perror("fork falhou!");
        exit(-1);
    }
    else if (a == 0) // Este é o processo filho
    {
    
        sleep(1);
        wait(&status);
        printf("%d\n",status);
        printf("Processo Filho B : %d Pai    A : %d\n", getpid(), getppid()); 
        sleep(1);
        b = fork(); // 
    
        if(b == 0 ){
            wait(&status);
            printf ("%d\n",WEXITSTATUS(status) * 1);

            printf("%d\n",status);
            printf("Processo Neto  C : %d Filho  B : %d\n", getpid(), getppid()); 
            sleep(1);
            c = getpid();
            printf("Processo Neto  C : %d\n",c);
            exit(0);
            return 0 ;
            
        }else{

        wait(&status);
        sleep(1);
        printf("Processo Filho B : %d Pai    A : %d\n", getpid(), getppid());
        exit(0);

        }
        exit(0);
    }
    else // Este é o processo pai
    {
        wait(&status);
        sleep(1);
        printf("Processo Pai   A : %d Pai    A : %d\n", getpid(), getppid());
        exit(0);
    }
}