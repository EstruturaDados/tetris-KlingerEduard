#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Desafio Tetris Stack - NOVATO

// Define a capacidade máxima da fila (número fixo de peças)
#define CAP_FILA 5

// Estrutura para representar uma peça do Tetris Stack
typedef struct {
    char tipo; // Tipo da peça ('I', 'O', 'T', 'L', etc.)
    int id;    // Identificador único da peça
} Peca;

// Estrutura para a fila circular de peças
typedef struct {
    Peca dados[CAP_FILA]; // Array que armazena as peças
    int frente;           // Índice da frente da fila
    int tamanho;          // Quantidade de peças na fila
} Fila;

// Variável global para gerar IDs únicos para as peças
int nextId = 1;

// Gera um tipo de peça aleatoriamente a partir de um conjunto predefinido
char gerarTipoAleatorio() {
    const char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    int idx = rand() % (sizeof(tipos) / sizeof(tipos[0]));
    return tipos[idx];
}

// Cria uma nova peça com tipo aleatório e ID único
Peca gerarPeca() {
    Peca p;
    p.tipo = gerarTipoAleatorio(); // Atribui um tipo aleatório
    p.id = nextId++;               // Atribui um ID único e incrementa o contador
    return p;
}

// Inicializa a fila com valores padrão
void inicializarFila(Fila *f) {
    f->frente = 0;   // Frente da fila começa no índice 0
    f->tamanho = 0;  // Fila começa vazia
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == CAP_FILA; // Retorna 1 se tamanho igual à capacidade
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0; // Retorna 1 se não houver peças
}

// Insere uma peça no final da fila circular
// Retorna 1 se bem-sucedido, 0 se a fila estiver cheia
int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        return 0; // Não insere se a fila está cheia
    }
    // Calcula a posição de inserção (final da fila)
    int pos = (f->frente + f->tamanho) % CAP_FILA;
    f->dados[pos] = p; // Armazena a peça na posição calculada
    f->tamanho++;      // Incrementa o tamanho da fila
    return 1;
}

// Remove a peça da frente da fila e a armazena em *out
// Retorna 1 se bem-sucedido, 0 se a fila estiver vazia
int dequeue(Fila *f, Peca *out) {
    if (filaVazia(f)) {
        return 0; // Não remove se a fila está vazia
    }
    *out = f->dados[f->frente]; // Copia a peça da frente
    f->frente = (f->frente + 1) % CAP_FILA; // Avança a frente (circular)
    f->tamanho--; // Decrementa o tamanho da fila
    return 1;
}

// Exibe o estado atual da fila
void mostrarFila(Fila *f) {
    printf("Fila de peças\n");
    if (filaVazia(f)) {
        printf("<vazia>\n"); // Mostra mensagem se a fila está vazia
    } else {
        // Itera sobre as peças na fila, respeitando a estrutura circular
        for (int i = 0; i < f->tamanho; i++) {
            int idx = (f->frente + i) % CAP_FILA;
            printf("[%c %d] ", f->dados[idx].tipo, f->dados[idx].id);
        }
        printf("\n");
    }
    printf("Tabela: Visualização a fila de peças.\n");
}

// Preenche a fila com CAP_FILA peças no início do programa
void preencherInicial(Fila *f) {
    while (!filaCheia(f)) {
        Peca nova = gerarPeca(); // Gera uma nova peça
        enqueue(f, nova);        // Insere na fila
    }
}

int main() {
    // Inicializa o gerador de números aleatórios
    srand((unsigned)time(NULL));
    
    Fila fila;
    inicializarFila(&fila); // Inicializa a fila vazia
    preencherInicial(&fila); // Preenche com 5 peças iniciais

    int opc;
    printf("=== Tetris Stack - Nível Novato ===\n");

    do {
        // Exibe a fila e o menu no formato especificado
        mostrarFila(&fila);
        printf("\nOpções de ação:\n");
        printf("Código\tAção\n");
        printf("1\tJogar peça (dequeue)\n");
        printf("2\tInserir nova peça (enqueue)\n");
        printf("0\tSair\n");
        printf("Tabela: Comandos disponíveis para manipular a fila de peças.\n");
        printf("Escolha: ");

        // Lê a opção do usuário e protege contra entrada inválida
        if (scanf("%d", &opc) != 1) {
            while (getchar() != '\n'); // Limpa o buffer
            opc = -1; // Marca como inválido
        }

        // Processa a opção escolhida
        if (opc == 1) {
            Peca p;
            if (dequeue(&fila, &p)) {
                printf("Jogou peça: [%c %d]\n", p.tipo, p.id);
            } else {
                printf("Fila vazia! Nenhuma peça para jogar.\n");
            }
            mostrarFila(&fila); // Mostra o estado após a remoção
        } else if (opc == 2) {
            if (!filaCheia(&fila)) {
                Peca nova = gerarPeca(); // Gera uma nova peça
                enqueue(&fila, nova);    // Insere na fila
                printf("Nova peça adicionada: [%c %d]\n", nova.tipo, nova.id);
            } else {
                printf("Fila cheia! Não é possível adicionar nova peça.\n");
            }
            mostrarFila(&fila); // Mostra o estado após a inserção
        } else if (opc == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opção inválida.\n");
        }
    } while (opc != 0);

    return 0;
}