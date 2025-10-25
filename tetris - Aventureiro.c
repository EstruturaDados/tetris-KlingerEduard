#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Desafio Tetris Stack - Aventureiro

#define CAP_FILA 5   // Capacidade máxima da fila
#define CAP_PILHA 3  // Capacidade máxima da pilha

// Estrutura para representar uma peça do jogo
typedef struct {
    char tipo;  // Tipo da peça (I, O, T, S, Z, J, L)
    int id;     // Identificador único da peça
} Peca;

// Estrutura para a fila circular
typedef struct {
    Peca dados[CAP_FILA];  // Array de peças
    int frente;            // Índice da frente da fila
    int tamanho;           // Número de elementos na fila
} Fila;

// Estrutura para a pilha linear
typedef struct {
    Peca dados[CAP_PILHA];  // Array de peças
    int topo;               // Índice do topo da pilha (-1 indica vazia)
} Pilha;

int nextId = 1;  // Contador global para IDs únicos das peças

// Gera um tipo de peça aleatoriamente (I, O, T, S, Z, J, L)
char gerarTipoAleatorio() {
    const char tipos[] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};
    return tipos[rand() % 7];
}

// Cria uma nova peça com tipo aleatório e ID único
Peca gerarPeca() {
    Peca p;
    p.tipo = gerarTipoAleatorio();
    p.id = nextId++;
    return p;
}

/* --- Funções da Fila --- */

// Inicializa a fila com frente e tamanho zerados
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == CAP_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Adiciona uma peça ao final da fila
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    int pos = (f->frente + f->tamanho) % CAP_FILA; // Calcula posição circular
    f->dados[pos] = p;
    f->tamanho++;
    return 1;
}

// Remove a peça da frente da fila
int dequeue(Fila *f, Peca *out) {
    if (filaVazia(f)) return 0;
    *out = f->dados[f->frente];
    f->frente = (f->frente + 1) % CAP_FILA; // Avança a frente circularmente
    f->tamanho--;
    return 1;
}

/* --- Funções da Pilha --- */

// Inicializa a pilha com topo -1 (vazia)
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == CAP_PILHA - 1;
}

// Adiciona uma peça ao topo da pilha
int push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->dados[p->topo] = x;
    return 1;
}

// Remove a peça do topo da pilha
int pop(Pilha *p, Peca *out) {
    if (pilhaVazia(p)) return 0;
    *out = p->dados[p->topo];
    p->topo--;
    return 1;
}

/* --- Funções de Exibição --- */

// Exibe o estado atual da fila
void mostrarFila(Fila *f) {
    printf("Fila de peças [tamanho=%d]: ", f->tamanho);
    if (filaVazia(f)) {
        printf("<vazia>\n");
        return;
    }
    for (int i = 0; i < f->tamanho; ++i) {
        int idx = (f->frente + i) % CAP_FILA;
        printf("[%c %d] ", f->dados[idx].tipo, f->dados[idx].id);
    }
    printf("\n");
}

// Exibe o estado atual da pilha (do topo à base)
void mostrarPilha(Pilha *p) {
    printf("Pilha de reserva [tamanho=%d]: ", p->topo + 1);
    if (pilhaVazia(p)) {
        printf("<vazia>\n");
        return;
    }
    printf("(Topo -> Base): ");
    for (int i = p->topo; i >= 0; --i) {
        printf("[%c %d] ", p->dados[i].tipo, p->dados[i].id);
    }
    printf("\n");
}

// Exibe o menu em formato de tabela
void mostrarMenu() {
    printf("\n+-----------------+-----------------------+\n");
    printf("| Código          | Ação                  |\n");
    printf("+-----------------+-----------------------+\n");
    printf("| 1               | Jogar peça            |\n");
    printf("| 2               | Reservar peça         |\n");
    printf("| 3               | Usar peça reservada   |\n");
    printf("| 0               | Sair                  |\n");
    printf("+-----------------+-----------------------+\n");
    printf("Opção: ");
}

// Preenche a fila com 5 peças iniciais
void preencherInicial(Fila *f) {
    while (!filaCheia(f)) {
        enqueue(f, gerarPeca());
    }
}

int main() {
    srand((unsigned)time(NULL)); // Inicializa o gerador de números aleatórios
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    preencherInicial(&fila);

    int opc;
    printf("=== Tetris Stack - Nível Aventureiro ===\n");
    mostrarFila(&fila);
    mostrarPilha(&pilha);

    do {
        mostrarMenu();
        if (scanf("%d", &opc) != 1) {
            while (getchar() != '\n'); // Limpa o buffer de entrada
            printf("Entrada inválida! Digite um número entre 0 e 3.\n");
            continue;
        }
        while (getchar() != '\n'); // Limpa o buffer após entrada válida

        if (opc == 1) {
            // Jogar peça: remove da frente da fila e adiciona nova peça
            Peca p;
            if (dequeue(&fila, &p)) {
                printf("Jogou peça: [%c %d]\n", p.tipo, p.id);
                Peca novo = gerarPeca();
                enqueue(&fila, novo);
                printf("Repondo fila com: [%c %d]\n", novo.tipo, novo.id);
            } else {
                printf("Fila vazia!\n");
            }
            mostrarFila(&fila);
            mostrarPilha(&pilha);
        } else if (opc == 2) {
            // Reservar peça: move da fila para a pilha e repõe a fila
            if (pilhaCheia(&pilha)) {
                printf("Reserva cheia! Não é possível enviar.\n");
            } else if (filaVazia(&fila)) {
                printf("Fila vazia! Nada para enviar.\n");
            } else {
                Peca p;
                dequeue(&fila, &p);
                push(&pilha, p);
                printf("Enviado da fila para reserva: [%c %d]\n", p.tipo, p.id);
                Peca novo = gerarPeca();
                enqueue(&fila, novo);
                printf("Repondo fila com: [%c %d]\n", novo.tipo, novo.id);
            }
            mostrarFila(&fila);
            mostrarPilha(&pilha);
        } else if (opc == 3) {
            // Usar peça reservada: remove do topo da pilha
            Peca p;
            if (pop(&pilha, &p)) {
                printf("Usou peça da reserva: [%c %d]\n", p.tipo, p.id);
            } else {
                printf("Reserva vazia! Nenhuma peça para usar.\n");
            }
            mostrarFila(&fila);
            mostrarPilha(&pilha);
        } else if (opc == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opção inválida! Escolha entre 0 e 3.\n");
        }
    } while (opc != 0);

    return 0;
}