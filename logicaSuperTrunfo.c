#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
    char estado[50];
    char codigocard[50];
    char cidade[50];
    unsigned long int populacao;
    float area;
    float pib; /* em bilhões */
    int pontos_turistico;
    float densidade_populacional;
    float pib_per_capita;
    float super_poder;
} Carta;

/* Retorna:
   1 -> carta 1 vence
   2 -> carta 2 vence
   0 -> empate
*/
int comparar_atributo(const char* nome, double valor1, double valor2, int menor_vence) {
    const double EPS = 1e-6;
    printf("%s: ", nome);
    if (fabs(valor1 - valor2) < EPS) {
        printf("Empate (%.2f vs %.2f)\n", valor1, valor2);
        return 0;
    } else if ((menor_vence && valor1 < valor2) || (!menor_vence && valor1 > valor2)) {
        printf("Carta 1 vence (%.2f vs %.2f)\n", valor1, valor2);
        return 1;
    } else {
        printf("Carta 2 vence (%.2f vs %.2f)\n", valor1, valor2);
        return 2;
    }
}

int main () {
    const int n = 2;
    Carta cartas[n];

    // Cadastro das cartas
    for (int i = 0; i < n; i++) {
        printf("\n--- Carta %d ---\n", i+1);

        printf("Digite o estado: \n");
        fgets(cartas[i].estado, sizeof(cartas[i].estado), stdin);
        cartas[i].estado[strcspn(cartas[i].estado, "\n")] = 0;

        printf("Digite o codigo da carta: \n");
        fgets(cartas[i].codigocard, sizeof(cartas[i].codigocard), stdin);
        cartas[i].codigocard[strcspn(cartas[i].codigocard, "\n")] = 0;

        printf("Digite o nome da cidade: \n");
        fgets(cartas[i].cidade, sizeof(cartas[i].cidade), stdin);
        cartas[i].cidade[strcspn(cartas[i].cidade, "\n")] = 0;

        printf("Digite a população: \n");
        if (scanf("%lu", &cartas[i].populacao) != 1) {
            fprintf(stderr, "Entrada inválida para população\n");
            return 1;
        }

        printf("Digite a área: \n");
        if (scanf("%f", &cartas[i].area) != 1) {
            fprintf(stderr, "Entrada inválida para área\n");
            return 1;
        }

        printf("Digite o PIB (em bilhões): \n");
        if (scanf("%f", &cartas[i].pib) != 1) {
            fprintf(stderr, "Entrada inválida para PIB\n");
            return 1;
        }

        printf("Digite o número de pontos turísticos: \n");
        if (scanf("%d", &cartas[i].pontos_turistico) != 1) {
            fprintf(stderr, "Entrada inválida para pontos turísticos\n");
            return 1;
        }

        /* Limpa o buffer após scanf para preparar o próximo fgets */
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { /* descarta */ }

        /* Cálculos derivados */
        if (cartas[i].area > 0.0f)
            cartas[i].densidade_populacional = (float)cartas[i].populacao / cartas[i].area;
        else
            cartas[i].densidade_populacional = 0.0f;

        if (cartas[i].populacao > 0)
            cartas[i].pib_per_capita = (cartas[i].pib * 1000000000.0f) / (float)cartas[i].populacao;
        else
            cartas[i].pib_per_capita = 0.0f;

        float inverso_densidade = (cartas[i].densidade_populacional > 0.0f) ? 1.0f / cartas[i].densidade_populacional : 0.0f;
        cartas[i].super_poder = (float)cartas[i].populacao + cartas[i].area + cartas[i].pib +
                                (float)cartas[i].pontos_turistico + cartas[i].pib_per_capita + inverso_densidade;
    }

    // Impressão das cartas cadastradas
    printf("\n=== Fichas das Cartas ===\n");
    for (int i = 0; i < n; i++) {
        printf("\nCarta %d:\n", i+1);
        printf("Estado: %s\n", cartas[i].estado);
        printf("Código: %s\n", cartas[i].codigocard);
        printf("Cidade: %s\n", cartas[i].cidade);
        printf("População: %lu\n", cartas[i].populacao);
        printf("Área: %.2f km²\n", cartas[i].area);
        printf("PIB: %.2f bilhões de reais\n", cartas[i].pib);
        printf("Número de pontos turísticos: %d\n", cartas[i].pontos_turistico);
        printf("Densidade populacional: %.2f hab/km²\n", cartas[i].densidade_populacional);
        printf("PIB per Capita: %.2f reais\n", cartas[i].pib_per_capita);
        printf("Super Poder: %.2f\n", cartas[i].super_poder);
    }

    // Comparação dos atributos e pontuação
    printf("\n=== Comparação das Cartas ===\n");
    int score1 = 0, score2 = 0, res;

    res = comparar_atributo("População", (double)cartas[0].populacao, (double)cartas[1].populacao, 0);
    if (res == 1) score1++; else if (res == 2) score2++;

    res = comparar_atributo("Área", cartas[0].area, cartas[1].area, 0);
    if (res == 1) score1++; else if (res == 2) score2++;

    res = comparar_atributo("PIB (bilhões)", cartas[0].pib, cartas[1].pib, 0);
    if (res == 1) score1++; else if (res == 2) score2++;

    res = comparar_atributo("Pontos Turísticos", (double)cartas[0].pontos_turistico, (double)cartas[1].pontos_turistico, 0);
    if (res == 1) score1++; else if (res == 2) score2++;

    res = comparar_atributo("PIB per Capita", cartas[0].pib_per_capita, cartas[1].pib_per_capita, 0);
    if (res == 1) score1++; else if (res == 2) score2++;

    /* Para densidade populacional, menor é melhor (cidade menos densa ganha) */
    res = comparar_atributo("Densidade Populacional (menor vence)", cartas[0].densidade_populacional, cartas[1].densidade_populacional, 1);
    if (res == 1) score1++; else if (res == 2) score2++;

    res = comparar_atributo("Super Poder", cartas[0].super_poder, cartas[1].super_poder, 0);
    if (res == 1) score1++; else if (res == 2) score2++;

    // Resultado final
    printf("\n=== Resultado do Jogo ===\n");
    printf("Pontuação final -> Carta 1: %d | Carta 2: %d\n", score1, score2);
    if (score1 > score2) {
        printf("Resultado: Carta 1 vence o jogo!\n");
        printf("Vencedora: %s (%s - %s)\n", cartas[0].cidade, cartas[0].estado, cartas[0].codigocard);
    } else if (score2 > score1) {
        printf("Resultado: Carta 2 vence o jogo!\n");
        printf("Vencedora: %s (%s - %s)\n", cartas[1].cidade, cartas[1].estado, cartas[1].codigocard);
    } else {
        printf("Resultado: Empate no jogo!\n");
    }

    return 0;
}
