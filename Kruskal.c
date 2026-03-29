#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Funções principais utilizadas no programa
void inicializaCidades();
void listaCidades();
void criaCidade();
void abreviaNome (char *nome);
void adicionaCidade(char *nome);
void checaCidade();
void kruskalAGM();

// Matrizes e variáveis globais para armazenar informações
float matrizAdjascencia[20][20] = {0}; // Matriz de adjacência para distâncias entre cidades
char matrizNomes[20][40];              // Matriz para armazenar nomes das cidades
int numCidades = 0;                    // Contador do número de cidades adicionadas

// Estrutura para representar uma aresta no grafo
struct Aresta {
    int origem, destino; // Índices de origem e destino da aresta
    float peso;          // Peso (distância) da aresta
};

// Função para encontrar o conjunto ao qual um vértice pertence (usada em Kruskal)
int encontrarConjunto(int conjuntos[], int vertice) {
    if (conjuntos[vertice] == -1) // Se o vértice não pertence a nenhum conjunto
        return vertice;
    return encontrarConjunto(conjuntos, conjuntos[vertice]); // Busca recursiva
}

// Função para unir dois conjuntos (usada em Kruskal)
void unirConjuntos(int conjuntos[], int origem, int destino) {
    int conjuntoOrigem = encontrarConjunto(conjuntos, origem);
    int conjuntoDestino = encontrarConjunto(conjuntos, destino);
    conjuntos[conjuntoOrigem] = conjuntoDestino;
}

int main(void) {
    int choice = 0;

    // Menu principal para interação com o usuário
    while (choice != 9) {
        printf("\n"
               "1. Listar Cidades: \n"
               "2. Adicionar Cidade: \n"
               "3. Checar cidade \n"
               "4. Gerar Árvore Geradora Mínima (AGM)\n");
        scanf("%d", &choice); getchar();

        // Chama as funções correspondentes às escolhas do usuário
        switch (choice) {
        case 1:
            listaCidades();
            break;
        case 2:
            criaCidade();
            break;
        case 3:
            checaCidade();
            break;
        case 4:
            kruskalAGM();
            break;
        }
    }
}

// Inicializa as matrizes com valores padrão
void inicializaCidades() {
    int i;
    for (i = 0; i < 20; i++) {
        matrizNomes[i][0] = '\0';  // Marca todos os nomes como vazios
        matrizAdjascencia[i][0] = 0; // Inicializa pesos com 0
        matrizAdjascencia[0][i] = 0;
    }
}

// Lista as cidades adicionadas e suas distâncias
void listaCidades() {
    int i, j;
    for (i = 0; i <= numCidades; i++) {
        if (i == 0)
            printf("%13s |", ""); // Cabeçalho da tabela
        else
            printf("%d: %10s |", i, matrizNomes[i - 1]);

        for (j = 1; j <= numCidades; j++) {
            if (i == 0) { // Imprime os nomes das cidades no cabeçalho
                printf("%10s |", matrizNomes[j - 1]);
            } else { // Imprime os valores de distância
                printf("%10.1f |", matrizAdjascencia[i - 1][j - 1]);
            }
        }
        printf("\n");
    }
}

// Adiciona uma nova cidade à matriz
void criaCidade() {
    int i = 0;
    char cityName[40];

    if (numCidades == 20) { // Verifica o limite máximo de cidades
        printf("Limite máximo de cidades atingido! ");
        return;
    }

    printf("Nome da cidade: ");
    scanf("%[^\n]%*c", cityName);

    fflush(stdin); // Limpa o buffer de entrada

    if (strlen(cityName) > 8) { // Abrevia o nome se for muito longo
        abreviaNome(cityName);
    }

    for (i = 0; i < 20; i++) {
        if (matrizNomes[i][0] == '\0') { // Encontra o próximo espaço vazio
            strcpy(matrizNomes[i], cityName);
            numCidades++;
            return;
        }
    }
}

// Checa as conexões de uma cidade com outras
void checaCidade() {
    int cityIndex, neighborIndex;
    char choice;
    float distance;

    printf("Indice da cidade escolhida: "); 
    scanf("%d", &cityIndex); 
    cityIndex--;

    while (choice != 'N') {
        getchar();
        printf("Essa cidade tem algum vizinho? (S/N)");
        scanf("%c", &choice);
        choice = toupper(choice);

        if (choice == 'N') return;

        printf("Qual o índice da cidade vizinha? ");
        scanf("%d", &neighborIndex); 
        neighborIndex--;

        printf("Qual a distância entre essas cidades? (Use somente valores positivos) ");
        scanf("%f", &distance);

        if (distance < 0) { // Garante que a distância seja positiva
            printf ("Somente distâncias positivas são permitidas: ");
            scanf("%f", &distance);
        }

        // Atualiza a matriz de adjacência
        matrizAdjascencia[cityIndex][neighborIndex] = distance;
        matrizAdjascencia[neighborIndex][cityIndex] = distance;
    }
}

// Abrevia nomes longos de cidades
void abreviaNome (char *nome) {
    for (int j = 0; nome[j] != '\0'; j++) {
        if (nome[j] == ' ') {
            nome[j+2] = '.';
            nome[j+3] = '\0';
        }
    }
}

// Algoritmo de Kruskal para encontrar a AGM
void kruskalAGM() {
    struct Aresta arestas[190];
    int numArestas = 0;
    int conjuntos[20];

    // Inicializa os conjuntos para Kruskal
    for (int i = 0; i < numCidades; i++) {
        conjuntos[i] = -1;
    }

    // Cria uma lista de arestas a partir da matriz de adjacência
    for (int i = 0; i < numCidades; i++) {
        for (int j = i + 1; j < numCidades; j++) {
            if (matrizAdjascencia[i][j] > 0) {
                arestas[numArestas].origem = i;
                arestas[numArestas].destino = j;
                arestas[numArestas].peso = matrizAdjascencia[i][j];
                numArestas++;
            }
        }
    }

    // Ordena as arestas pelo peso (Bubble Sort)
    for (int i = 0; i < numArestas - 1; i++) {
        for (int j = 0; j < numArestas - i - 1; j++) {
            if (arestas[j].peso > arestas[j + 1].peso) {
                struct Aresta temp = arestas[j];
                arestas[j] = arestas[j + 1];
                arestas[j + 1] = temp;
            }
        }
    }

    // Constrói a AGM usando as arestas ordenadas
    printf("\nÁrvore Geradora Mínima:\n");
    float pesoTotal = 0;
    for (int i = 0; i < numArestas; i++) {
        int conjuntoOrigem = encontrarConjunto(conjuntos, arestas[i].origem);
        int conjuntoDestino = encontrarConjunto(conjuntos, arestas[i].destino);

        if (conjuntoOrigem != conjuntoDestino) { // Verifica se forma um ciclo
            printf("%s - %s (%.1f)\n", matrizNomes[arestas[i].origem], matrizNomes[arestas[i].destino], arestas[i].peso);
            pesoTotal += arestas[i].peso;
            unirConjuntos(conjuntos, conjuntoOrigem, conjuntoDestino);
        }
    }

    printf("Peso total da AGM: %.1f\n", pesoTotal);
}
