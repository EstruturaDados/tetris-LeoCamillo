#include <stdio.h>
#include <stdlib.h>

// --- 1. Estrutura de Dados ---

// Definição da estrutura da Peça (Struct)
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único
} Peca;

// Constantes e variáveis globais para a Fila Circular
#define CAPACIDADE 5        // Tamanho máximo da fila
Peca fila[CAPACIDADE];      // Array para armazenar os elementos da fila
int frente = -1;            // Índice da frente da fila
int tras = -1;              // Índice do final da fila
int proximo_id = 0;         // Contador para gerar IDs únicos das peças

// --- 2. Funções Auxiliares ---

/**
 * @brief Gera uma nova peça automaticamente.
 * * Atribui um tipo de peça e um ID único sequencial.
 * @return Peca A nova peça gerada.
 */
Peca gerarPeca() {
    Peca novaPeca;
    // Tipos de peças disponíveis. Usa o resto da divisão para ciclar entre os tipos.
    char tipos[] = {'T', 'O', 'L', 'I'}; 
    
    novaPeca.nome = tipos[proximo_id % 4];
    novaPeca.id = proximo_id;
    proximo_id++;
    
    return novaPeca;
}

/**
 * @brief Exibe o estado atual da fila de peças no formato [T ID].
 */
void exibirFila() {
    printf("\nFila de Pecas:");
    
    if (frente == -1) {
        printf(" [Vazia]\n");
        return;
    }

    int i = frente;
    // Percorre a fila de forma circular, começando pela 'frente'
    do {
        printf(" [%c %d]", fila[i].nome, fila[i].id);
        if (i == tras) break; // Sai se chegou ao final ('tras')
        
        // Move para o próximo índice (circular: (i + 1) % CAPACIDADE)
        i = (i + 1) % CAPACIDADE; 
    } while (1); // Loop infinito interrompido pelo 'break' acima
    
    printf("\n");
}

// --- 3. Funções de Manipulação da Fila ---

/**
 * @brief Inicializa a fila preenchendo-a com 5 peças.
 */
void inicializarFila() {
    printf("Inicializando a fila com %d pecas...\n", CAPACIDADE);
    
    for (int i = 0; i < CAPACIDADE; i++) {
        // Verifica se a fila está vazia para definir 'frente' e 'tras' em 0
        if (frente == -1) {
            frente = 0;
            tras = 0;
        } else {
            // Move 'tras' para o próximo local circular
            tras = (tras + 1) % CAPACIDADE;
        }
        
        // Gera a nova peça e insere no índice 'tras'
        fila[tras] = gerarPeca();
    }
}

/**
 * @brief Insere uma nova peça ao final da fila (enqueue).
 * * Verifica se a fila está cheia antes de inserir.
 */
void enfileirar() {
    // Condição de Fila Cheia em Fila Circular: O próximo de 'tras' é igual a 'frente'
    if (frente == (tras + 1) % CAPACIDADE) {
        printf("\n❌ ERRO: A fila de pecas esta **CHEIA**. Nao e possivel inserir nova peca.\n");
        return;
    }
    
    Peca novaPeca = gerarPeca();

    // Caso de fila vazia (frente == -1). Só ocorre se a fila foi esvaziada.
    if (frente == -1) {
        frente = 0;
        tras = 0;
    } else {
        // Move 'tras' para o próximo local circular
        tras = (tras + 1) % CAPACIDADE;
    }
    
    fila[tras] = novaPeca;
    printf("\n✅ Peça **%c %d** inserida (**enqueue**).\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Remove a peça da frente da fila (dequeue).
 * * Verifica se a fila está vazia antes de remover.
 */
void desenfileirar() {
    // Condição de Fila Vazia
    if (frente == -1) {
        printf("\n❌ ERRO: A fila de pecas esta **VAZIA**. Nao ha pecas para jogar (**dequeue**).\n");
        return;
    }

    Peca pecaRemovida = fila[frente]; // A peça na frente é a que será removida/jogada

    // Caso 1: Fila tem apenas 1 elemento (frente == tras)
    if (frente == tras) {
        // Reinicializa a fila como vazia
        frente = -1;
        tras = -1;
    } 
    // Caso 2: Fila tem mais de 1 elemento
    else {
        // Move 'frente' para o próximo local circular
        frente = (frente + 1) % CAPACIDADE;
    }

    printf("\n✅ Peça **%c %d** jogada/removida da frente (**dequeue**).\n", pecaRemovida.nome, pecaRemovida.id);
}

// --- 4. Função Principal (Menu) ---

/**
 * @brief Função principal que gerencia o menu de interação.
 */
int main() {
    int opcao;
    
    // Inicializa a fila com as 5 primeiras peças
    inicializarFila();
    
    // Loop principal do menu
    do {
        // Exibe o estado da fila antes de pedir a ação
        exibirFila(); 

        printf("\n--- Opcoes de Acao ---\n");
        printf("1 - Jogar Peca (**dequeue**)\n");
        printf("2 - Inserir Nova Peca (**enqueue**)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        // Leitura da opção
        if (scanf("%d", &opcao) != 1) {
            // Trata entrada inválida (não numérica)
            while (getchar() != '\n'); // Limpa o buffer de entrada
            opcao = -1; // Define uma opção inválida para não entrar em nenhum case
            printf("\n❌ Opcao invalida. Digite um numero.\n");
            continue; 
        }

        // Processa a opção
        switch (opcao) {
            case 1:
                desenfileirar();
                break;
            case 2:
                enfileirar();
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
