#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Desafio Tetris Stack - MESTRE
// Programa que simula uma fila de peças e uma pilha de reserva.

// capacidade da fila e da pilha
#define CAP_FILA 5
#define CAP_PILHA 3

// representação de uma peça (tipo e id)
typedef struct {
    char tipo;
    int id;
} Peca;

// fila circular de peças
typedef struct {
    Peca dados[CAP_FILA];
    int frente;   // índice do primeiro elemento
    int tamanho;  // número de elementos na fila
} Fila;

// pilha simples de peças
typedef struct {
    Peca dados[CAP_PILHA];
    int topo; // índice do topo (-1 quando vazia)
} Pilha;

/* --- Variáveis globais --- */
// id incremental para cada peça criada
int nextId = 1;

/* --- Fila --- */
// inicializa a fila vazia
void inicializarFila(Fila *f) { f->frente = 0; f->tamanho = 0; }
// verifica se a fila está cheia
int filaCheia(Fila *f) { return f->tamanho == CAP_FILA; }
// verifica se a fila está vazia
int filaVazia(Fila *f) { return f->tamanho == 0; }

// adiciona peça no final da fila (enqueue)
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    int pos = (f->frente + f->tamanho) % CAP_FILA;
    f->dados[pos] = p;
    f->tamanho++;
    return 1;
}

// remove peça da frente da fila (dequeue)
int dequeue(Fila *f, Peca *out) {
    if (filaVazia(f)) return 0;
    *out = f->dados[f->frente];
    f->frente = (f->frente + 1) % CAP_FILA;
    f->tamanho--;
    return 1;
}

/* --- Pilha --- */
// inicializa pilha vazia
void inicializarPilha(Pilha *p) { p->topo = -1; }
// verifica se a pilha está vazia
int pilhaVazia(Pilha *p) { return p->topo == -1; }
// verifica se a pilha está cheia
int pilhaCheia(Pilha *p) { return p->topo == CAP_PILHA - 1; }

// empilha uma peça (push)
int push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->dados[p->topo] = x;
    return 1;
}

// desempilha uma peça (pop)
int pop(Pilha *p, Peca *out) {
    if (pilhaVazia(p)) return 0;
    *out = p->dados[p->topo];
    p->topo--;
    return 1;
}

/* --- Geração de peças --- */
// retorna um tipo aleatório de peça
char gerarTipoAleatorio() {
    const char tipos[] = {'I','O','T','S','Z','J','L'};
    return tipos[rand() % 7];
}

// cria uma nova peça com id único
Peca gerarPeca() {
    Peca p;
    p.tipo = gerarTipoAleatorio();
    p.id = nextId++;
    return p;
}

/* --- Exibição --- */
// mostra o conteúdo da fila
void mostrarFila(Fila *f) {
    printf("Fila [tamanho=%d]: ", f->tamanho);
    if (filaVazia(f)) { printf("<vazia>\n"); return; }
    for (int i = 0; i < f->tamanho; i++) {
        int idx = (f->frente + i) % CAP_FILA;
        printf("[%c-%d] ", f->dados[idx].tipo, f->dados[idx].id);
    }
    printf("\n");
}

// mostra o conteúdo da pilha (do topo para base)
void mostrarPilha(Pilha *p) {
    printf("Pilha [tamanho=%d]: ", (p->topo + 1));
    if (pilhaVazia(p)) { printf("<vazia>\n"); return; }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c-%d] ", p->dados[i].tipo, p->dados[i].id);
    }
    printf("\n");
}

/* --- Inicialização --- */
// preenche a fila até a capacidade inicial
void preencherInicial(Fila *f) {
    while (!filaCheia(f)) enqueue(f, gerarPeca());
}

/* --- Operações especiais --- */
// troca a peça da frente da fila com o topo da pilha
int trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (filaVazia(f)) { printf("Fila vazia!\n"); return 0; }
    if (pilhaVazia(p)) { printf("Pilha vazia!\n"); return 0; }
    int idx = f->frente;
    Peca tmp = f->dados[idx];
    f->dados[idx] = p->dados[p->topo];
    p->dados[p->topo] = tmp;
    return 1;
}

// troca as três primeiras peças da fila com as três da pilha (se houver)
int trocarTres(Fila *f, Pilha *p) {
    if (p->topo != CAP_PILHA - 1) { 
        printf("Pilha não tem 3 peças.\n"); return 0; 
    }
    if (f->tamanho < 3) { 
        printf("Fila não tem 3 peças.\n"); return 0; 
    }
    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % CAP_FILA;
        Peca tmp = f->dados[idxFila];
        f->dados[idxFila] = p->dados[i];
        p->dados[i] = tmp;
    }
    return 1;
}

// inverte parcialmente os elementos entre fila e pilha (até o menor tamanho)
void inverterFilaPilha(Fila *f, Pilha *p) {
    int min = (f->tamanho < (p->topo + 1)) ? f->tamanho : (p->topo + 1);
    for (int i = 0; i < min; i++) {
        int idxFila = (f->frente + i) % CAP_FILA;
        Peca tmp = f->dados[idxFila];
        f->dados[idxFila] = p->dados[i];
        p->dados[i] = tmp;
    }
    printf("Fila e pilha invertidas parcialmente (base do topo correspondente).\n");
}

/* --- Sistema de Undo --- */
// estado que guarda uma cópia simples da fila e pilha
typedef struct {
    Fila fila;
    Pilha pilha;
} Estado;

Estado ultimoEstado;

// salva estado atual (para desfazer)
void salvarEstado(Fila *f, Pilha *p) {
    ultimoEstado.fila = *f;
    ultimoEstado.pilha = *p;
}

// restaura último estado salvo
void desfazer(Fila *f, Pilha *p) {
    *f = ultimoEstado.fila;
    *p = ultimoEstado.pilha;
    printf("Última jogada desfeita.\n");
}

/* --- Programa Principal --- */
int main() {
    // semente para aleatoriedade
    srand((unsigned)time(NULL));
    Fila fila; Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    preencherInicial(&fila);

    int opc;
    printf("=== Tetris Stack - Nível Mestre Completo ===\n");
    mostrarFila(&fila);
    mostrarPilha(&pilha);

    // loop do menu de opções
    do {
        printf("\nMenu:\n1-Jogar peça\n2-Reservar peça\n3-Usar peça reservada\n4-Trocar frente com topo\n5-Desfazer última jogada\n6-Inverter fila com pilha\n0-Sair\nEscolha: ");
        if (scanf("%d", &opc) != 1) { while (getchar()!='\n'); opc=-1; }

        if (opc == 1) {
            // jogar peça: remove da fila e gera nova peça
            salvarEstado(&fila, &pilha);
            Peca p;
            if (dequeue(&fila, &p)) {
                printf("Jogou: [%c-%d]\n", p.tipo, p.id);
                Peca novo = gerarPeca(); enqueue(&fila, novo);
                printf("Nova peça adicionada: [%c-%d]\n", novo.tipo, novo.id);
            }
        } else if (opc == 2) {
            // reservar peça: mover da fila para a pilha (push)
            if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                salvarEstado(&fila, &pilha);
                Peca p; dequeue(&fila, &p); push(&pilha, p);
                printf("Peça enviada para reserva: [%c-%d]\n", p.tipo, p.id);
                Peca novo = gerarPeca(); enqueue(&fila, novo);
                printf("Nova peça adicionada: [%c-%d]\n", novo.tipo, novo.id);
            } else printf("Fila vazia ou pilha cheia.\n");
        } else if (opc == 3) {
            // usar peça reservada: pop da pilha
            if (!pilhaVazia(&pilha)) {
                salvarEstado(&fila, &pilha);
                Peca p; pop(&pilha, &p);
                printf("Usou peça da reserva: [%c-%d]\n", p.tipo, p.id);
            } else printf("Pilha vazia.\n");
        } else if (opc == 4) {
            // troca frente da fila com topo da pilha
            salvarEstado(&fila, &pilha);
            if (trocarFrenteComTopo(&fila, &pilha))
                printf("Troca frente <-> topo realizada.\n");
        } else if (opc == 5) {
            // desfazer última ação
            desfazer(&fila, &pilha);
        } else if (opc == 6) {
            // inverter parcialmente fila e pilha
            salvarEstado(&fila, &pilha);
            inverterFilaPilha(&fila, &pilha);
        } else if (opc == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opção inválida.\n");
        }

        mostrarFila(&fila);
        mostrarPilha(&pilha);

    } while (opc != 0);

    return 0;
}
