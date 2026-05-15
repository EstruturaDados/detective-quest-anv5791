#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.


// Estrutura rígida de árvore binária para caminhos duplos
typedef struct NoComodo {
    char nome[50];
    struct NoComodo *pai;             // Permite rastrear o caminho de volta
    struct NoComodo *ComodoEsquerda;  // Opção de rota 1 (Esquerda)
    struct NoComodo *ComodoDireita;   // Opção de rota 2 (Direita)
} NoComodo;

// Função para gerenciar histórico de comodos visitados (pilha)
typedef struct NodoHistorico {
    char nomeComodo[50];
    struct NodoHistorico *proximo;
} NodoHistorico;

void adicionarHistorico(NodoHistorico **topo, const char *nomeComodo) {
    NodoHistorico *novo = (NodoHistorico*)malloc(sizeof(NodoHistorico));
    if (novo == NULL) return;
    strncpy(novo->nomeComodo, nomeComodo, 49);   
    novo->proximo = *topo;  
    *topo = novo;  
}


void liberarHistorico(NodoHistorico **topo) {
    while (*topo != NULL) {
        NodoHistorico *temp = *topo;
        topo = &(*topo)->proximo;
        free(temp);
    }
}

// Criação de cômodo e atribuição do nó superior (pai)
NoComodo* criarComodo(const char *nome, NoComodo *pai) { // Aloca memória para um novo cômodo
    NoComodo *novo = (NoComodo*)malloc(sizeof(NoComodo)); // Verifica se a alocação foi bem-sucedida
    if (novo == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    strncpy(novo->nome, nome, 49); // Copia o nome do cômodo, garantindo que não ultrapasse o limite
    novo->nome[49] = '\0';
    novo->pai = pai;
    novo->ComodoEsquerda = NULL;
    novo->ComodoDireita = NULL;
    return novo;
}

// Desaloca a estrutura da memória ao finalizar
void liberarMansao(NoComodo *raiz) {
    if (raiz == NULL) return;
    liberarMansao(raiz->ComodoEsquerda);
    liberarMansao(raiz->ComodoDireita);
    free(raiz);
}


int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // ----------------------------------------------------
    // INICIALIZAÇÃO DA ESTRUTURA BINÁRIA DA MANSÃO
    // ----------------------------------------------------
    // Cria o Hall de Entrada como ponto de partida
    NoComodo *hall = criarComodo("Hall de Entrada", NULL);

    // Caminhos bilaterais saindo diretamente do Hall de Entrada
    NoComodo *living = criarComodo("Sala de Estar Principal", hall);
    NoComodo *lazer = criarComodo("Lazer Externo", hall);
    hall->ComodoEsquerda = living;
    hall->ComodoDireita = lazer;

    // Ramificações internas adicionais
    NoComodo *cinema = criarComodo("Cinema Privativo", living);
    living->ComodoEsquerda = cinema;

    NoComodo *piscina = criarComodo("Piscina", lazer);
    lazer->ComodoDireita = piscina;

    // ----------------------------------------------------
    // SISTEMA DE NAVEGAÇAO COM HISTORICO
    // ----------------------------------------------------
    NoComodo *atual = hall;

    // Ponteiro para controlar o topo do histórico
    NodoHistorico *historico = NULL;
    adicionarHistorico(&historico, atual->nome);

    // ----------------------------------------------------
    // SISTEMA DE INTERAÇÃO E EXIBIÇÃO DE OPÇÕES
    // ----------------------------------------------------
    
    int opcao;
    do {
        // Alerta visual imediato de qual ambiente está sendo visitado
        printf("\n             ---- MANSÃO VIEIRA ----");
        printf("\n==================================================\n");
        printf(" POSICAO ATUAL: -> [ %s ] <-\n", atual->nome);
        if (historico != NULL && historico->proximo != NULL) {
            printf(" SALA ANTERIOR  : %s\n", historico->proximo->nomeComodo);            
        } else {
            printf(" SALA ANTERIOR :[ Nenhuma (Voce esta no inicio) ]\n");
        }
        printf("==================================================\n");
        printf("Escolha uma das opcoes de navegacao disponiveis:\n\n");

        // Mapeamento fixo das 3 ações possíveis em uma árvore binária com ponteiro pai
        NoComodo *opcoes[4] = { NULL }; 
        int indice = 1;

        // Opção 1: Rota da Esquerda
        if (atual->ComodoEsquerda != NULL) {
            printf("[%d] Seguir pela ESQUERDA (Ir para: %s)\n", indice, atual->ComodoEsquerda->nome);
            opcoes[indice] = atual->ComodoEsquerda;
            indice++;
        } else {
            printf("[ ] (Caminho da Esquerda Bloqueado / Parede)\n");
        }

        // Opção 2: Rota da Direita
        if (atual->ComodoDireita != NULL) {
            printf("[%d] Seguir pela DIREITA  (Ir para: %s)\n", indice, atual->ComodoDireita->nome);
            opcoes[indice] = atual->ComodoDireita;
            indice++;
        } else {
            printf("[ ] (Caminho da Direita Bloqueado / Parede)\n");
        }

        // Opção 3: Rota de Retorno (Voltar para o pai)
        int opcaoVoltar = 0;
        if (atual->pai != NULL) {
            opcaoVoltar = indice;
            printf("[%d] RETORNAR pelo corredor anterior (Voltar para: %s)\n", opcaoVoltar, atual->pai->nome);
            indice++;
        }

        printf("[0] Finalizar exploracao e Sair\n");
        printf("\nDigite o numero da sua decisao: ");

        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); 
            printf("\nErro: Insira apenas numeros inteiros.\n");
            continue;
        }

        if (opcao == 0) {
            printf("\nSaindo do simulador da mansao. Visita encerrada!\n");
            break;
        }

        // Execução da movimentação do ponteiro
        if (opcaoVoltar > 0 && opcao == opcaoVoltar) {
            atual = atual->pai;
            adicionarHistorico(&historico, atual->nome);
        } else if (opcao > 0 && opcao < indice && opcoes[opcao] != NULL) {
            atual = opcoes[opcao];
            adicionarHistorico(&historico, atual->nome);
        } else {
            printf("\nAcao invalida! Esse caminho nao existe a partir desta sala.\n");
        }

    } while (opcao != 0);

    liberarMansao(hall);
    liberarHistorico(&historico);

   
    return 0;
}

 // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

