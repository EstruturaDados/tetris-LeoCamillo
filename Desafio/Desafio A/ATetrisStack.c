#include <stdio.h>
#include <stdlib.h>

// --- 1. Estruturas de Dados e Variáveis Globais ---

// Definição da estrutura da Peça (Struct)
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único
} Peca;

// Variáveis da Fila Circular (FIFO)
#define CAPACIDADE_FILA 5   // Tamanho máximo da fila
Peca fila[CAPACIDADE_FILA];
int frente = -1;            // Índice da frente da fila
int tras = -1;              // Índice do final da fila

// Variáveis da Pilha Linear (LIFO)
#define CAPACIDADE_PILHA 3  // Tamanho máximo da pilha de reserva
Peca pilha[CAPACIDADE_PILHA];
int topo = -1;              // Índice do topo da pilha (-1 indica pilha vazia)

// Variável para gerar IDs únicos
int proximo_id = 0;         

// --- 2. Funções Auxiliares (Geração e Exibição) ---

/**
 * @brief Gera uma nova peça automaticamente com um tipo cíclico e ID único.
 * @return Peca A nova peça gerada.
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'T', 'O', 'L', 'I'}; 
    
    novaPeca.nome = tipos[proximo_id % 4];
    novaPeca.id = proximo_id;
    proximo_id++;
    
    return novaPeca;
}

/**
 * @brief Exibe o estado atual da fila e da pilha.
 */
void exibirEstado() {
    // --- Exibir Fila ---
    printf("\nFila de Pecas:");
    if (frente == -1) {
        printf(" [Vazia]");
    } else {
        int i = frente;
        do {
            printf(" [%c %d]", fila[i].nome, fila[i].id);
            if (i == tras) break; 
            i = (i + 1) % CAPACIDADE_FILA; 
        } while (1);
    }

    // --- Exibir Pilha ---
    printf("\nPilha de Reserva (Topo -> Base):");
    if (topo == -1) {
        printf(" [Vazia]\n");
    } else {
        for (int i = topo; i >= 0; i--) {
            printf(" [%c %d]", pilha[i].nome, pilha[i].id);
        }
        printf("\n");
    }
}

// --- 3. Funções de Manipulação da Fila ---

/**
 * @brief Insere uma peça no final da fila (enqueue).
 * @param novaPeca A peça a ser inserida.
 */
void enfileirar(Peca novaPeca) {
    if (frente == -1) {
        frente = 0;
        tras = 0;
    } else {
        // Move 'tras' para o próximo local circular
        tras = (tras + 1) % CAPACIDADE_FILA;
    }
    fila[tras] = novaPeca;
}

/**
 * @brief Inicializa a fila de peças com a capacidade máxima (5).
 */
void inicializarFila() {
    printf("Inicializando a fila com %d pecas...\n", CAPACIDADE_FILA);
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        // Inicializa inserindo 5 peças
        if (frente == -1) {
            frente = 0;
            tras = 0;
        } else {
            tras = (tras + 1) % CAPACIDADE_FILA;
        }
        fila[tras] = gerarPeca();
    }
}

/**
 * @brief Remove a peça da frente da fila (dequeue) e insere uma nova para repor. (Opção 1)
 */
void jogarPeca() {
    if (frente == -1) {
        printf("\n❌ ERRO: A fila esta VAZIA. Nao ha pecas para jogar.\n");
        return;
    }

    Peca pecaRemovida = fila[frente]; 
    
    // Remove (dequeue): Move 'frente' para o próximo local circular
    frente = (frente + 1) % CAPACIDADE_FILA;

    // Repõe: Enfileira a nova peça no espaço liberado
    Peca novaPeca = gerarPeca();
    enfileirar(novaPeca); 

    printf("\n✅ Peça **%c %d** jogada/removida da frente (**dequeue**).\n", pecaRemovida.nome, pecaRemovida.id);
    printf("↪️ Nova peca **%c %d** adicionada ao final da fila para manter o tamanho.\n", novaPeca.nome, novaPeca.id);
}

// --- 4. Funções de Manipulação da Pilha ---

/**
 * @brief Insere uma peça no topo da pilha (push).
 * @param peca A peça a ser inserida.
 * @return int 1 se sucesso, 0 se pilha cheia.
 */
int empilhar(Peca peca) {
    if (topo == CAPACIDADE_PILHA - 1) {
        return 0; // Pilha cheia
    }
    topo++; 
    pilha[topo] = peca; 
    return 1;
}

/**
 * @brief Remove a peça do topo da pilha (pop).
 * @return Peca A peça removida (ou peça com id -1 se pilha vazia).
 */
Peca desempilhar() {
    Peca pecaVazia = {' ', -1};

    if (topo == -1) {
        return pecaVazia; // Pilha vazia
    }
    Peca pecaRemovida = pilha[topo];
    topo--; 
    return pecaRemovida;
}

/**
 * @brief Move a peça da frente da fila para a pilha (reservar). (Opção 2)
 */
void reservarPeca() {
    // 1. Verifica se a Pilha de Reserva está cheia
    if (topo == CAPACIDADE_PILHA - 1) {
        printf("\n❌ ERRO: A Pilha de Reserva esta **CHEIA** (Max %d). Nao e possivel reservar.\n", CAPACIDADE_PILHA);
        return;
    }
    
    // 2. Pega a peça da frente da fila
    Peca pecaReservada = fila[frente]; 

    // 3. Empilha a peça (Push)
    empilhar(pecaReservada);

    // 4. Remove a peça da Fila (similar ao dequeue)
    frente = (frente + 1) % CAPACIDADE_FILA; 

    // 5. Repõe: Insere nova peça na Fila (para manter o tamanho)
    Peca novaPeca = gerarPeca();
    enfileirar(novaPeca); 

    printf("\n✅ Peça **%c %d** reservada, movida da fila para a pilha (**push**).\n", pecaReservada.nome, pecaReservada.id);
    printf("↪️ Nova peca **%c %d** adicionada ao final da fila para manter o tamanho.\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Remove a peça do topo da pilha (pop) e insere nova peça na fila para repor. (Opção 3)
 */
void usarPecaReservada() {
    Peca pecaUsada = desempilhar();
    
    // 1. Verifica se a Pilha está vazia
    if (pecaUsada.id == -1) {
        printf("\n❌ ERRO: A Pilha de Reserva esta **VAZIA**. Nao ha pecas para usar.\n");
        return;
    }
    
    // 2. Repõe: Insere nova peça na Fila (para manter o tamanho)
    Peca novaPeca = gerarPeca();
    enfileirar(novaPeca);
    
    printf("\n✅ Peça **%c %d** usada, removida do topo da pilha (**pop**).\n", pecaUsada.nome, pecaUsada.id);
    printf("↪️ Nova peca **%c %d** adicionada ao final da fila para manter o tamanho.\n", novaPeca.nome, novaPeca.id);
}

// --- 5. Função Principal (Menu) ---

/**
 * @brief Função principal que gerencia o menu de interação.
 */
int main() {
    int opcao;
    
    // Inicializa a fila com a capacidade máxima
    inicializarFila();
    
    // Loop principal do menu
    do {
        // Exibe o estado de ambas as estruturas
        exibirEstado(); 

        printf("\n--- Opcoes de Acao ---\n");
        printf("1 - Jogar Peca (dequeue) e repor fila\n");
        printf("2 - Reservar Peca (Fila -> Pilha push) e repor fila\n");
        printf("3 - Usar Peca Reservada (pop) e repor fila\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        // Leitura da opção
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); 
            opcao = -1;
            printf("\n❌ Opcao invalida. Digite um numero.\n");
            continue; 
        }

        // Processa a opção
        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 0:
                printf("\n👋 Saindo do Tetris Stack Simulator. Ate logo!\n");
                break;
            default:
                printf("\n❌ Opcao invalida. Por favor, tente novamente.\n");
                break;
        }
        
    } while (opcao != 0);

    return 0;
}