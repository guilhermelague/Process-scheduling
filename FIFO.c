/* 
 * Instituição: UERGS
 * Curso: Engenharia de computação
 * Arquivo: FIFO.c
 * Autor: Guilherme Lague
 * E-mail: guilhermelaguebmx@gmail.com
 * Descrição: Implementar o algoritmo de escalonamento FIFO, onde entra-se com o nome do processo, o tempo de CPU e o tempo de chegada.
 *            O programa deve simular a execução em FIFO e gerar os seguintes valores: tempo de resposta e tempo de espera.
 * Criado em 9 de Abril de 2017, 23:11
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//================================================================================
// int id: id do processo
// char nome[20]: nome do processo
// char status[20]: status do processo
// int prioridade: prioridade do processo
// int t_ex: tempo de execução do processo
// int t_en: tempo de entrada do processo
// struct processos *ant: ponteiro para o processo anterior
// struct processos *prox: ponteiro para o processo posterior
//================================================================================
typedef struct processos{
    int id;
    char nome[20];
    char status[20];
    int prioridade;
    int t_ex;
    int t_en;
    struct processos *ant;
    struct processos *prox;
}processos;

// ponteiro para os processos
processos *novo = NULL, *inicio = NULL, *fim = NULL;
int id = 1;

// protótipo da função
void inserir(int id, char* nome, char* status, int prioridade, int t_en, int t_ex);

//================================================================================
// Esta função cria um arquivo chamado bf.txt, simulando uma memória com blocos ja determinados
// FILE* file: Ponteiro para o arquivo
//================================================================================
void cria_arquivo(FILE* file){
    file = fopen("C:/Users/Guilherme Lague/Desktop/fifo.txt","w+t");
    //processo, prioridade, tempo de entrada, tempo de execução
    fprintf(file,"%d %s %s %d %d %d\n", 1, "P1", "Executando", 2, 0, 8);
    fprintf(file,"%d %s %s %d %d %d\n", 2, "P2", "Executando", 3, 3, 6);
    fprintf(file,"%d %s %s %d %d %d\n", 3, "P3", "Executando", 7, 5, 9);
    fprintf(file,"%d %s %s %d %d %d\n", 4, "P4", "Executando", 5, 8, 10);
    fprintf(file,"%d %s %s %d %d %d\n", 5, "P5", "Executando", 4, 9, 5);
    fprintf(file,"%d %s %s %d %d %d", 6, "P6", "Executando", 6, 13, 7);
    fclose(file);
}
//================================================================================
// Esta função le o arquivo criado chamado bf.txt
// FILE* file: Ponteiro para o arquivo
//================================================================================
void le_arquivo(FILE* file){
    char processo[4], status[20];
    int id, prio, en, ex;
    
    file = fopen("C:/Users/Guilherme Lague/Desktop/fifo.txt","rt");
    while(!feof(file)){
        fscanf(file, "%d %s %s %d %d %d", &id, &processo, &status, &prio, &en, &ex);
        inserir(id, processo, status, prio, en, ex);
    }
    fclose(file); 
}

//================================================================================
// Esta função insere processos na lista
//================================================================================
void inserir(int id,char *nome, char *status, int prioridade, int t_en, int t_ex){
    novo = (processos*)malloc(sizeof(processos));
    
    novo->id = id;
    strcpy(novo->nome,nome);
    strcpy(novo->status,status);
    novo->prioridade = prioridade;
    novo->t_en = t_en;
    novo->t_ex = t_ex;

    if(inicio == NULL){
        fim = novo;
        inicio = novo;
        novo->ant = NULL;
    }else{
        fim->prox = novo;
        novo->ant = fim;
        fim = fim->prox;
    }
    novo->prox = NULL;
}

//================================================================================
// Esta função imprime os processos contidos na lista dupla
//================================================================================
void listar(){
    
    if(inicio==NULL){
        printf("Lista vazia!\a\n\n");
    }
    else{
        novo = inicio;
        printf("===============LISTA DE PROCESSOS===================================================\n");
        printf(" ID |  PROCESSO  |     STATUS     |  PRIORIDADE  | TEMPO DE CHEGADA | TEMPO DE CPU =\n");
        printf("====================================================================================\n");
        while(novo!=NULL){
            printf("%3d |%11s |%15s |%13d |%17d |%13d =\n",novo->id, novo->nome, novo->status, novo->prioridade, novo->t_en, novo->t_ex);
            printf("------------------------------------------------------------------------------------\n");
            novo = novo->prox;
        }
        printf("====================================================================================\n");
        printf("\n\n");
    }
}

//================================================================================
// Esta função remove processos por id contidos na lista dupla
//================================================================================
void excluir(int id){    
    processos *anterior, *atual;
    
    int foi_excluido = 0;
    if(inicio == NULL){
        printf("Lista vazia!\a\n\n");
    }
    else{
        anterior = inicio;     
        atual = inicio;
        while(atual != NULL){
            if(atual->id == id){
                if(atual == inicio){
                    inicio = inicio->prox;
                    free(atual);
                    foi_excluido = 1;
                    break;
                }
                else if(atual == fim){
                    fim = anterior;		
                }
                anterior->prox = atual->prox;
                free(atual);
                foi_excluido = 1;
                break;	
            }
            else{
                anterior = atual;
                atual = atual->prox;

            }
        }
        if(foi_excluido == 1){
                printf("Processo [%d] excluido!\n\n", id);
        }else{
                printf("Processo [%d] nao existe na lista de processos!\n\n\a", id);
        }
    }
}

//================================================================================
// Esta função retorna o total de processos contidos na lista
//================================================================================
int total_processos(){
    int i = 0;
    if(inicio == NULL){
        printf("Lista vazia!\a\n\n");
    }
    else{
        novo = inicio;
        while(novo != NULL){ 
            i++;
            novo = novo->prox;
        }
        return i;
    }
}

//================================================================================
// Esta função calcula o tempo de espera e resposta de cada processo
//================================================================================
void calcula_tempos(){ 
    int t_espera = 0, t_resposta = 0, n_processos = total_processos();
    float t_espera_total = 0, t_resposta_total = 0;
    if(inicio == NULL){
        printf("Lista vazia!\a\n\n");
    }
    else{
        novo = inicio;
        printf("\n Processo | Tempo de espera | Tempo de resposta\n\n");
        while(novo != NULL){
            if(novo == inicio){ // se for o primeiro
                t_resposta = novo->t_ex + t_espera;
                printf("%9s |%16d |%16d \n", novo->nome, t_espera, t_resposta);
            }
            else{
                t_espera = t_resposta + (novo->ant->t_en - novo->t_en);
                t_resposta = novo->t_ex + t_espera;
                if(t_espera <= 0){
                    t_espera = 0;
                    t_resposta = novo->t_ex;
                    printf("%9s |%16d |%16d \n", novo->nome, t_espera, t_resposta);
                }
                else{
                    printf("%9s |%16d |%16d \n", novo->nome, t_espera, t_resposta);
                }
            }
            t_espera_total = t_espera_total + t_espera;
            t_resposta_total = t_resposta_total + t_resposta;
            novo = novo->prox;
        }
        printf("\nTempo medio de espera: %.2f\n", t_espera_total/n_processos);
        printf("Tempo medio de resposta: %.2f\n\n", t_resposta_total/n_processos);
    }
}

//================================================================================
// Esta função gera o grafico de processos
//================================================================================
void grafico(){
    int x, y, t_espera = 0, t_resposta = 0;
    
    printf("============================ FIFO ================================");
    for (novo = inicio; novo != NULL; novo = novo->prox){//linhas
        printf("\n");
        if(novo == inicio){ // se for o primeiro processo..
            t_resposta = novo->t_ex + t_espera;
            for (x = 0; x < novo->t_en; x++){ 
                printf("-");
            }
            for (y = 0; y < t_resposta ; y++){
                printf("#");
            }
        }
        else{ // se não for o primeiro processo...
            t_espera = t_resposta + (novo->ant->t_en - novo->t_en); 
            t_resposta = novo->t_ex + t_espera;                     
            if(t_espera < 0){
                for (x = 0; x < novo->t_en; x++){
                    printf("-");
                }
                for (y = 0; y < novo->t_ex ; y++){
                    printf("#");
                }
                t_espera = 0;
                t_resposta = novo->t_ex;
            }
            else{
                for (x = 0; x < t_espera + novo->t_en; x++){
                    printf("-");
                }
                for (y = 0; y < novo->t_ex ; y++){
                    printf("#");
                }
            }
        }
    }
    printf("\n==================================================================");
    printf("\n\n");
}

//================================================================================
// Função principal
//================================================================================
int main(int argc, char const *argv[]){
    int op = 0, op2 = 0, len, t_ex, t_en;
    char nome[20], status[20], prioridade[20];
    FILE *file;
    
    cria_arquivo(file);
    
    printf("-------------------MENU-------------------\n");
    printf("1- Inserir processo na lista\n");
    printf("2- Listar processos\n");
    printf("3- Encerrar processo\n");
    printf("4- Escalonar processos em FIFO\n");
    printf("5- Sair\n");
    printf("------------------------------------------\n");
    printf("Opcao: ");
    scanf("%d",&op);
    printf("------------------------------------------\n\n");

    while(op != 0){
        switch(op){
            case 1:
                printf("1- Inserir de um arquivo\n");
                printf("2- Inserir manualmente\n");
                printf("Opcao: ");
                scanf("%d",&op2);
                if(op2 == 1){
                    le_arquivo(file);
                    listar();
                }
                else{
                    while(op == 1){
                        printf("Digite o nome do processo ou 0 (zero) para parar de inserir: ");
                        scanf("%s",nome);
                        if(strcmp("0",nome) == 0){
                            break;
                        }
                        else{
                            /*printf("Digite o status do processo (Bloqueado, Suspenso ou Executando): ");
                            fflush(stdin);
                            gets(status);
                            printf("Digite a prioridade do processo (0 a 10): ");
                            scanf("%d",&prioridade);*/
                            printf("Digite o tempo de chegada do processo: ");
                            scanf("%d",&t_en);
                            printf("Digite o tempo de cpu do processo: ");
                            scanf("%d",&t_ex);
                            inserir(id, nome, "Executando", 0, t_en, t_ex);
                        }
                        id++;
                    }
                }
            break;
            case 2:
                listar();
            break;
            case 3:
                printf("Digite o id do processo para excluir:");
                scanf("%d",&id);
                excluir(id);
            break;
            case 4:
                calcula_tempos();
                grafico();
            break;
            case 5:
                exit(0);
            break;
            default: printf("Opcao invalida!\a\n\n");
        }
        printf("-------------------MENU-------------------\n");
        printf("1- Inserir processo na lista\n");
        printf("2- Listar processos\n");
        printf("3- Encerrar processo\n");
        printf("4- Escalonar processos em FIFO\n");
        printf("5- Sair\n");
        printf("------------------------------------------\n");
        printf("Opcao: ");
        scanf("%d",&op);
        printf("------------------------------------------\n\n");
    }
    return (EXIT_SUCCESS);
}