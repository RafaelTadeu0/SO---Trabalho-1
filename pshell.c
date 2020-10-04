#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// define as mensagens para o usuario como macros apenas para facilitar alteração
#define PRIMEIRA_MENSAGEM "Qual o comando quer executar? -> "
#define SEGUNDA_MENSAGEM "Quantos argumentos voce quer digitar? -> "

// macro criada para printar mensagens de debug no terminal
#define DEBUG_MODE 0

// definindo numero maximo de argumentos
#define MAX_ARGS 10

// Definindo stado inicial do processo
int STATE = 0;
/*
    Estado 0 significa: Execucao normal.
    Estado 1 significa: Sinal ou anormalidade detectado, reiniciar exigencias para o usuario.
*/




// definindo handler para o sinal SIGUSR1
void signal_handler(int signal_number)
{
  if (signal_number == SIGUSR1){
    //printf("Sinal SIGUSR1 recebido\n");
    STATE = 1;
  }
}



int main() {

  signal(SIGUSR1, signal_handler);
  while (1){
    STATE = 0;
    
    char *bin_path = (char *) malloc(100*(sizeof(char))); // variavel que será utilizada para construir caminho para a execucao do /bin
    char *comando = (char *) malloc(100*(sizeof(char))); // variavel que será usada para receber o comando que deve ser executado
    int numero_argumentos; // variavel para salvar numero de argumentos
    char *argumentos = (char *) malloc(50*(sizeof(char))); // variavel para salvar os argumentos recebidos por linha de comando
    char *args [MAX_ARGS];

    for (int i = 0; i < MAX_ARGS; ++i){
        args[i] =NULL;
    }

    printf(PRIMEIRA_MENSAGEM);
    // espera receber a mensagem de entrada
    scanf("%100s",comando);

    // iniciando verificacao da variavel comando
    int i;
    for (i=0; comando[i] != '\0'; i++) {
        if (comando[i] < 'a' || comando[i] > 'z') {
            STATE = 1;
            printf("Foram encontrados caracteres invalidos para o comando.\n");
            break;
        }
    };

    if(DEBUG_MODE){
        printf("-------- DEBUG MODE ---------\n");
        printf("Primeira entrada recebida:\n-> ");
        printf("%s\n",comando);
        printf("----------- END ------------\n");
    }

    strcpy(bin_path,"/bin/");
    strcat(bin_path,comando); // concatenar o nome do comando recebido com o /bin/ para gerar /bin/<comando>

    if(DEBUG_MODE){
        printf("-------- DEBUG MODE ---------\n");
        printf("Comando que sera chamado no /bin:\n-> ");
        printf("%s\n",bin_path);
        printf("----------- END ------------\n");
    }

    //strcat(args[0],comando); // concatenando o comando gerado nos args que serão passados

    if (STATE == 1){
        // liberando espaco alocado para salvar o comando pedido pelo usuario
        free(comando);
        free(bin_path);
        numero_argumentos = 0;
    }
    else{
        printf(SEGUNDA_MENSAGEM);
        scanf("%d", &numero_argumentos); // pede o numero de argumentos
        if (STATE == 1){
            continue;
        }
        for (int i = 0; i < numero_argumentos; ++i){
            args[i] =(char *) malloc(50*(sizeof(char)));
        }
    };

    // busca de argumentos
    for (int count = 1; count < numero_argumentos + 1; count++) {
        if (STATE == 1){
            // libera o espaço alocado para args
            for (int i = 0; i < numero_argumentos; ++i){
                free(args[i]);
            }
            break;
        }
        printf("Digite o argumento [%d] -> ",count);
        scanf("%s",argumentos);
        if(DEBUG_MODE){
            printf("-------- DEBUG MODE ---------\n");
            printf("Argumento recebido:\n-> ");
            printf("%s\n",argumentos);
            printf("----------- END ------------\n");
        }
        strcpy(args[count-1],argumentos); // salva uma copia dos argumentos em args2
        //strcat(args,argumentos); // concatena em args os argumentos recebidos
        //strcat(args," ");
    }
    if (STATE == 0){
        // executa o fork para gerar o processo filho
        pid_t child_pid = fork();
        if (child_pid == 0) {
            //printf("Inside child\n");
            //printf("Buscando valor de bin\n");
            //printf("%s\n",bin_path);
            if(DEBUG_MODE){
                printf("Iniciando execucao do comando no processo filho");
                printf("PATH:%s\n",bin_path);
                for (int i = 0; i < numero_argumentos; ++i){
                    printf("ARGS[%d]:%s\n",i,args[i]);
                }
            }
            execv(bin_path,args);
            //printf("filho executou\n");
            //printf("REsposta da execucao: %d\n", response);
            return 0;
        } else {
            if(DEBUG_MODE){
                printf("Esperando execução do processo filho\n");
            }
            wait(NULL);
            if(DEBUG_MODE){
                printf("Processo filho executado com sucesso\n");
            }
            //printf("Comando executado com sucesso\n");
            for (int i = 0; i < numero_argumentos; ++i){
                free(args[i]);
            }
            free(bin_path);
            free(comando);
            free(argumentos);
            return 0;
        }
    };
  }
};