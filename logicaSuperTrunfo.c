#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
    char estado[50];
    char codigocard[50];
    char cidade[50];
    unsigned long int populacao;
    double area;
    double pib; /* em bilhões */
    int pontos_turistico;
    double densidade_populacional;
    double pib_per_capita;
    double super_poder;
} Carta;

const double EPS = 1e-9;

/* Retorna o nome do atributo a partir da opção */
const char* nome_atributo(int opc) {
    switch (opc) {
        case 1: return "População";
        case 2: return "Área (km²)";
        case 3: return "PIB (bilhões)";
        case 4: return "Pontos Turísticos";
        case 5: return "PIB per Capita (R$)";
        case 6: return "Densidade Populacional (hab/km²)";
        case 7: return "Super Poder";
        default: return "Atributo Desconhecido";
    }
}

/* Retorna 1 se menor valor vence (apenas para densidade), 0 caso contrário */
int menor_vence(int opc) {
    if (opc == 6) return 1; /* densidade: menor vence */
    return 0;
}

/* Obtém o valor do atributo como double para comparação e soma */
double valor_atributo(const Carta* c, int opc) {
    switch (opc) {
        case 1: return (double)c->populacao;
        case 2: return c->area;
        case 3: return c->pib;
        case 4: return (double)c->pontos_turistico;
        case 5: return c->pib_per_capita;
        case 6: return c->densidade_populacional;
        case 7: return c->super_poder;
        default: return 0.0;
    }
}

/* Compara um atributo entre duas cartas.
   Retorna: 1 -> carta1 vence, 2 -> carta2 vence, 0 -> empate */
int comparar_um_atributo(const char* nome, double v1, double v2, int menorWins) {
    printf("%s: ", nome);
    if (fabs(v1 - v2) < EPS) {
        printf("Empate (%.2f vs %.2f)\n", v1, v2);
        return 0;
    } else if ((menorWins && v1 < v2) || (!menorWins && v1 > v2)) {
        printf("Carta 1 vence (%.2f vs %.2f)\n", v1, v2);
        return 1;
    } else {
        printf("Carta 2 vence (%.2f vs %.2f)\n", v1, v2);
        return 2;
    }
}

/* Imprime menu de atributos. Se excluir >=1, esse índice será pulado. */
void imprimir_menu(int excluir) {
    printf("Escolha um atributo numérico para comparação:\n");
    for (int i = 1; i <= 7; ++i) {
        if (i == excluir) continue;
        /* use switch para montar o menu (requisito de usar switch) */
        switch (i) {
            case 1: printf("  1 - População\n"); break;
            case 2: printf("  2 - Área (km²)\n"); break;
            case 3: printf("  3 - PIB (bilhões)\n"); break;
            case 4: printf("  4 - Pontos Turísticos\n"); break;
            case 5: printf("  5 - PIB per Capita (R$)\n"); break;
            case 6: printf("  6 - Densidade Populacional (menor vence)\n"); break;
            case 7: printf("  7 - Super Poder\n"); break;
        }
    }
    printf("Digite o número correspondente: ");
}

/* Limpa o buffer de entrada até quebra de linha */
void limpar_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

int main() {
    const int n = 2;
    Carta cartas[n];

    /* Entrada das cartas */
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro da Carta %d ---\n", i+1);

        printf("Digite o estado: ");
        if (!fgets(cartas[i].estado, sizeof(cartas[i].estado), stdin)) {
            fprintf(stderr, "Erro ao ler estado.\n");
            return 1;
        }
        cartas[i].estado[strcspn(cartas[i].estado, "\n")] = 0;

        printf("Digite o código da carta: ");
        if (!fgets(cartas[i].codigocard, sizeof(cartas[i].codigocard), stdin)) {
            fprintf(stderr, "Erro ao ler código da carta.\n");
            return 1;
        }
        cartas[i].codigocard[strcspn(cartas[i].codigocard, "\n")] = 0;

        printf("Digite o nome da cidade: ");
        if (!fgets(cartas[i].cidade, sizeof(cartas[i].cidade), stdin)) {
            fprintf(stderr, "Erro ao ler cidade.\n");
            return 1;
        }
        cartas[i].cidade[strcspn(cartas[i].cidade, "\n")] = 0;

        printf("Digite a população (número inteiro): ");
        if (scanf("%lu", &cartas[i].populacao) != 1) { fprintf(stderr, "Entrada inválida.\n"); return 1; }

        printf("Digite a área (km²): ");
        if (scanf("%lf", &cartas[i].area) != 1) { fprintf(stderr, "Entrada inválida.\n"); return 1; }

        printf("Digite o PIB (em bilhões): ");
        if (scanf("%lf", &cartas[i].pib) != 1) { fprintf(stderr, "Entrada inválida.\n"); return 1; }

        printf("Digite o número de pontos turísticos: ");
        if (scanf("%d", &cartas[i].pontos_turistico) != 1) { fprintf(stderr, "Entrada inválida.\n"); return 1; }

        /* limpar restante do buffer para próximos fgets */
        limpar_buffer();

        /* cálculos derivados */
        if (cartas[i].area > 0.0)
            cartas[i].densidade_populacional = (double)cartas[i].populacao / cartas[i].area;
        else
            cartas[i].densidade_populacional = 0.0;

        if (cartas[i].populacao > 0)
            cartas[i].pib_per_capita = (cartas[i].pib * 1e9) / (double)cartas[i].populacao;
        else
            cartas[i].pib_per_capita = 0.0;

        double inverso_densidade = (cartas[i].densidade_populacional > 0.0) ? 1.0 / cartas[i].densidade_populacional : 0.0;
        cartas[i].super_poder = (double)cartas[i].populacao + cartas[i].area + cartas[i].pib +
                                (double)cartas[i].pontos_turistico + cartas[i].pib_per_capita + inverso_densidade;
    }

    /* Exibir as cartas cadastradas de forma clara */
    printf("\n=== Fichas das Cartas Cadastradas ===\n");
    for (int i = 0; i < n; ++i) {
        printf("\nCarta %d:\n", i+1);
        printf("  Estado: %s\n", cartas[i].estado);
        printf("  Código: %s\n", cartas[i].codigocard);
        printf("  Cidade: %s\n", cartas[i].cidade);
        printf("  População: %lu\n", cartas[i].populacao);
        printf("  Área: %.2f km²\n", cartas[i].area);
        printf("  PIB: %.2f bilhões\n", cartas[i].pib);
        printf("  Pontos Turísticos: %d\n", cartas[i].pontos_turistico);
        printf("  Densidade: %.2f hab/km²\n", cartas[i].densidade_populacional);
        printf("  PIB per Capita: %.2f R$\n", cartas[i].pib_per_capita);
        printf("  Super Poder: %.2f\n", cartas[i].super_poder);
    }

    /* Escolha interativa de dois atributos distintos */
    int escolha1 = 0, escolha2 = 0;
    while (1) {
        imprimir_menu(0);
        if (scanf("%d", &escolha1) != 1) { limpar_buffer(); printf("Entrada inválida. Tente novamente.\n"); continue; }
        if (escolha1 < 1 || escolha1 > 7) { printf("Opção inválida. Escolha entre 1 e 7.\n"); continue; }
        break;
    }
    limpar_buffer();

    while (1) {
        imprimir_menu(escolha1); /* menu dinâmico: não mostra a primeira escolha */
        if (scanf("%d", &escolha2) != 1) { limpar_buffer(); printf("Entrada inválida. Tente novamente.\n"); continue; }
        if (escolha2 < 1 || escolha2 > 7) { printf("Opção inválida. Escolha entre 1 e 7.\n"); continue; }
        if (escolha2 == escolha1) { printf("Você não pode escolher o mesmo atributo duas vezes. Escolha outro.\n"); continue; }
        break;
    }
    limpar_buffer();

    printf("\nAtributos escolhidos:\n  1) %s\n  2) %s\n\n", nome_atributo(escolha1), nome_atributo(escolha2));

    /* Comparações individuais e soma */
    double v1_card1 = valor_atributo(&cartas[0], escolha1);
    double v1_card2 = valor_atributo(&cartas[1], escolha1);
    double v2_card1 = valor_atributo(&cartas[0], escolha2);
    double v2_card2 = valor_atributo(&cartas[1], escolha2);

    printf("=== Comparações por Atributo ===\n");
    int res1 = comparar_um_atributo(nome_atributo(escolha1), v1_card1, v1_card2, menor_vence(escolha1));
    int res2 = comparar_um_atributo(nome_atributo(escolha2), v2_card1, v2_card2, menor_vence(escolha2));

    /* Soma dos valores dos atributos (usando os valores numéricos originais) */
    double soma_card1 = v1_card1 + v2_card1;
    double soma_card2 = v1_card2 + v2_card2;

    printf("\n=== Soma dos Atributos Selecionados ===\n");
    printf("Carta 1 -> %s: %.2f | %s: %.2f | Soma = %.2f\n",
           nome_atributo(escolha1), v1_card1, nome_atributo(escolha2), v2_card1, soma_card1);
    printf("Carta 2 -> %s: %.2f | %s: %.2f | Soma = %.2f\n",
           nome_atributo(escolha1), v1_card2, nome_atributo(escolha2), v2_card2, soma_card2);

    printf("\n=== Resultado da Rodada (por soma) ===\n");
    if (fabs(soma_card1 - soma_card2) < EPS) {
        printf("Empate!\n");
    } else if (soma_card1 > soma_card2) {
        printf("Carta 1 vence a rodada pela maior soma (%.2f vs %.2f)!\n", soma_card1, soma_card2);
        printf("Vencedora: %s (%s - %s)\n", cartas[0].cidade, cartas[0].estado, cartas[0].codigocard);
    } else {
        printf("Carta 2 vence a rodada pela maior soma (%.2f vs %.2f)!\n", soma_card2, soma_card1);
        printf("Vencedora: %s (%s - %s)\n", cartas[1].cidade, cartas[1].estado, cartas[1].codigocard);
    }

    (void)res1; /* suprime warning de variável não usada quando compilado com -Wall e sem uso posterior */
    (void)res2;

    return 0;
}