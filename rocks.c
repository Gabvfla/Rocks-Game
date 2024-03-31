#include <stdio.h>
#include <stdlib.h>

// Função para Apresentar as Instruções do jogo Caso o Usuário queira vê-las
void instrucoes(char inst) { 
    if (inst == 'S' || inst == 's') {
        printf("\n\033[1;34mRegras Básicas \033[0m\n\n");
        printf("\033[1;36mO jogo apresenta 48 casas com pedras (*) e 16 casas vazias (-), Totalizando 64 casas;\n");
        printf("·Só é possível fazer movimentos na diagonal;\n");
        printf("·Não há possibilidade de fazer saltos duplos;\n");
        printf("·Não é possível fazer saltos sobre casas vazias;\n\033[0m");
        printf("\n\033[1;34mO Jogo se encerra quando \033[0m\n\n");
        printf("\033[1;36m·o jogador decide desistir ou quando não é mais possível fazer saltos.\033[0m\n");
        printf("\033[1;36m·Para Desistir basta digitar pelo menos dois Zeros (0).\033[0m\n");
        printf("\n\033[1;34mO Jogo apresenta 4 dificuldades \033[0m\n\n");
        printf("\033[35ma) Extremamente fácil: eliminar até 29 pedras\033[0m\n");
        printf("\033[32mb) Fácil: eliminar de 30 a 39 pedras;\n\033[0m");
        printf("\033[33mc) Difícil: eliminar de 40 a 44 pedras;\n\033[0m");
        printf("\033[31md) Extremamente difícil: eliminar de 45 a 47 pedras;\n\033[0m");
    }
}

// Função para verificar se a casa de destino está na mesma diagonal da casa de origem
int verificarDiagonal(int lin_o, int col_o, int lin_d, int col_d) {
    int diff_lin = lin_d - lin_o;
    int diff_col = col_d - col_o;

    // Verifica se as diferenças entre as linhas e colunas são iguais (mesma diagonal)
    return (abs(diff_lin) == abs(diff_col));
}

// Função para verificar se todas as casas na diagonal entre a casa de origem e a casa de destino não estão vazias
int verificarCasasVazias(char tabuleiro[8][8], int lin_o, int col_o, int lin_d, int col_d) {
    int diff_lin = (lin_d > lin_o) ? 1 : -1; // Determina se o movimento é para cima ou para baixo na matriz
    int diff_col = (col_d > col_o) ? 1 : -1; // Determina se o movimento é para a esquerda ou para a direita na matriz

    // Percorre todas as casas na diagonal entre a casa de origem e a casa de destino
    int lin = lin_o + diff_lin;
    int col = col_o + diff_col;
    while (lin != lin_d || col != col_d) {
        if (tabuleiro[lin][col] == '-')
            return 0; // Encontrou uma casa vazia na diagonal
        lin += diff_lin;
        col += diff_col;
    }

    return 1; // Caso todas as casas na diagonal estão ocupadas
}

// Função para verificar se uma posição é válida
int posicaoValida(int linha, int coluna) {
    return (linha >= 0 && linha < 8 && coluna >= 0 && coluna < 8);
}

// Função para Verificar se o Movimento é válido
int movimentoValido(char tabuleiro[8][8], int lin_o, int col_o, int lin_d, int col_d) {
    // Verifica se as posições de origem e destino estão dentro do tabuleiro
    if (!posicaoValida(lin_o, col_o) || !posicaoValida(lin_d, col_d))
        return 0;

    // Verifica se a posição de destino está vazia
    if (tabuleiro[lin_d][col_d] != '-')
        return 0;

    // Verifica se o movimento é diagonal
    int diff_lin = lin_d - lin_o;
    int diff_col = col_d - col_o;
    if (abs(diff_lin) != abs(diff_col))
        return 0;

    // Verifica se há uma peça intermediária para ser eliminada
    int lin_intermediaria = lin_o + (diff_lin / 2);
    int col_intermediaria = col_o + (diff_col / 2);
    if (tabuleiro[lin_intermediaria][col_intermediaria] != '*')
        return 0;

    return 1;
}

// Função para mover a pedrinha de Lugar
void moverPedra(char tabuleiro[8][8], int lin_o, int col_o, int lin_d, int col_d) {
    tabuleiro[lin_o][col_o] = '-'; // Remove a peça da posição de origem
    tabuleiro[lin_d][col_d] = '*'; // Coloca a peça na posição de destino
}

// Função para realizar um salto em uma direção específica
void fazerSalto(char tabuleiro[8][8], int lin_o, int col_o, int lin_d, int col_d, int *pedras_eliminadas_salto) {
    if (posicaoValida(lin_o, col_o) && posicaoValida(lin_d, col_d) &&
        tabuleiro[lin_o][col_o] == '*' && tabuleiro[lin_d][col_d] == '-') {

        int dirLinha = lin_d - lin_o;
        int dirColuna = col_d - col_o;

        if ((dirLinha == -2 || dirLinha == 2) && (dirColuna == -2 || dirColuna == 2)) {
            int lin_s = lin_o + dirLinha / 2;
            int col_s = col_o + dirColuna / 2;

            if (tabuleiro[lin_s][col_s] == '*') {
                // Realizar o salto
                tabuleiro[lin_o][col_o] = '-';
                tabuleiro[lin_d][col_d] = '*';
                tabuleiro[lin_s][col_s] = '-';

                *pedras_eliminadas_salto += 1;
            }
        }
    }
}

// Função para eliminar as pedras na diagonal entre a casa de origem e a casa de destino
int eliminarPedras(char tabuleiro[8][8], int lin_o, int col_o, int lin_d, int col_d) {
    int dir_lin = (lin_d - lin_o) / abs(lin_d - lin_o);
    int dir_col = (col_d - col_o) / abs(col_d - col_o);

    int lin = lin_o + dir_lin;
    int col = col_o + dir_col;
    int pedras_eliminadas = 0;

    while (lin != lin_d || col != col_d) {
        if (tabuleiro[lin][col] == '*') {
            tabuleiro[lin][col] = '-'; // Eliminar a pedra
            pedras_eliminadas++;
        }
        lin += dir_lin;
        col += dir_col;
    }

    return pedras_eliminadas;
}

// Inicializar tabuleiro
void iniciar_tabuleiro(char m[8][8]) {
    int i,
        j;
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 8; j++) {
            m[i][j] = '*';
        }
    }
    
    for(i = 2; i <= 5; i++) {
        for(j = 2; j <= 5; j++) {
            m[i][j] = '-';
        }
    }
}

// Apresentar o tabuleiro
void imprimirTabuleiro(char tabuleiro[8][8]) {
    int i, j;
    printf("  ");
    for (i = 0; i < 8; i++) {
        printf("%d ", i + 1);
    }
    printf("\n");

    for (i = 0; i < 8; i++) {
        printf("%d ", i + 1);
        for (j = 0; j < 8; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char tabuleiro[8][8]; // Tabuleiro 8 por 8
    int lin_o, col_o, lin_d, col_d; // Coordenadas de origem e destino
    int pedras_eliminadas_salto = 0; // Pedras elimiandas por salto
    int pedras_eliminadas_total = 0; // Pedras eliminadas totais
    int pedras_restantes = 48; // Pedras Restantes
    char inst; // Resposta se quer ver o manual
    char jogar_novamente; // Resposta se quer jogar novamente
    int contasalto = 0; // Contador de Saltos

    // Inicio do Jogo
    printf("\033[1;34mJogo De Eliminação das Pedras\033[0m\n\n");
    printf("\033[36mO objetivo do jogo é eliminar o máximo de peças possíveis.\033[0m\n");

    do { // Looping Principal 
        printf("\033[36mDeseja ver as instruções? (S/N)\n\033[0m");
        scanf(" %c", &inst);
        if (inst != 'S' && inst != 's' && inst != 'N' && inst != 'n') {
            printf("\033[31mDigite uma resposta válida\033[0m\n");
        }
    } while (inst != 's' && inst != 'S' && inst != 'n' && inst != 'N');

    instrucoes(inst); // Chamada das Instruções
    printf("\n");
  do{
    iniciar_tabuleiro(tabuleiro); // Inicia o Jogo do zero
    imprimirTabuleiro(tabuleiro); // Imprime o Tabuleiro

    do {
        // Solicitar a casa de origem
        do {
            printf("\033[36mInforme a posição de \033[1;33mOrigem\033[0m\033[36m da pedra que deseja mover \033[31m[Digite dois zeros '0' Para Desistir]\033[0m \033[0m");
            printf("\033[36m\nLinha: \033[0m");
            scanf("%d", &lin_o);
            printf("\033[36mColuna: \033[0m");
            scanf("%d", &col_o);
            if(lin_o == 0 && col_o == 0){
              break; // Caso Valor digitado seja zero finaliza o jogo
            }    
            if (!posicaoValida(lin_o - 1, col_o - 1) || tabuleiro[lin_o - 1][col_o - 1] != '*') {
                printf("\033[31mCasa de origem inválida! Tente novamente.\n\033[0m");
            }
        } while (!posicaoValida(lin_o - 1, col_o - 1) || tabuleiro[lin_o - 1][col_o - 1] != '*');

        // Solicitar a casa de destino
        do {
            printf("\033[36mInforme a posição de \033[1;33mDestino\033[0m\033[36m da pedra que deseja mover \033[31m[Digite dois zeros '0' Para Desistir]\033[0m \033[0m");
            printf("\033[36m\nLinha: \033[0m");
            scanf("%d", &lin_d);
            printf("\033[36mColuna: \033[0m");
            scanf("%d", &col_d);
            if(lin_d == 0 && col_d ==0){
              break; // Caso Valor digitado seja zero finaliza o jogo
            } 
            if (!posicaoValida(lin_d - 1, col_d - 1) || tabuleiro[lin_d - 1][col_d - 1] != '-') {
                printf("\033[31mCasa de destino inválida! Tente novamente.\n\033[0m");
            }
        } while (!posicaoValida(lin_d - 1, col_d - 1) || tabuleiro[lin_d - 1][col_d - 1] != '-');

        // Movendo a pedra e eliminando pedras intermediárias
        if (movimentoValido(tabuleiro, lin_o - 1, col_o - 1, lin_d - 1, col_d - 1)) {
            moverPedra(tabuleiro, lin_o - 1, col_o - 1, lin_d - 1, col_d - 1);
            pedras_eliminadas_salto = eliminarPedras(tabuleiro, lin_o - 1, col_o - 1, lin_d - 1, col_d - 1);

            if (pedras_eliminadas_salto > 0) { // Caso a Jogada elimine uma Pedra
                printf("\n");
                imprimirTabuleiro(tabuleiro); // Mostra o Tabuleiro Atualizado
                pedras_eliminadas_total += pedras_eliminadas_salto; // Contador de Pedras eliminadas
                pedras_restantes -= pedras_eliminadas_salto; // Contador de Pedras restantes
                contasalto++; // Contador de Saltos
                printf("\033\n[32mSalto de (%d, %d) para (%d, %d)\033[0m", lin_o, col_o, lin_d, col_d);
                printf("\n\033[32mPedras eliminadas neste salto: %d\033[0m\n", pedras_eliminadas_salto);
                printf("\033[32mTotal de pedras eliminadas: %d\033[0m\n", pedras_eliminadas_total);
                printf("\033[32mTotal de pedras restantes: %d\033[0m\n\n", pedras_restantes);
            } else {
                printf("\033[31mNão há pedras para serem eliminadas neste salto!\033[0m\n\n");
            }
        } else if ( lin_o > 0 && col_o > 0 && lin_d > 0 && col_d > 0){
            printf("\033[31mMovimento inválido! Tente novamente.\n\033[0m");
        } else{
          break;
        }

    } while (1);
        printf("\n");
        printf("\033[1;33mFim de jogo!\033[0m\n\n");
        printf("\033[32mTotal de pedras eliminadas: %d\033[0m\n", pedras_eliminadas_total);
        printf("\033[32mTotal de saltos realizados: %d\033[0m\n", contasalto);
        printf("\033[32mTotal de pedras restantes: %d\033[0m\n\n", pedras_restantes);

        // Atualização da dificuldade com base nas ações do jogador
        if (pedras_eliminadas_total >= 45) {
            printf("\033[31md) Nível de Dificuldade atingido: Extremamente difícil\n\033[0m");
        } else if (pedras_eliminadas_total >= 40) {
            printf("\033[33mc) Nível de Dificuldade atingido: Difícil\n\033[0m");
        } else if (pedras_eliminadas_total >= 30) {
            printf("\033[32md) Nível de Dificuldade atingido: Fácil\n\033[0m");
        } else if (pedras_eliminadas_total <= 29) {
            printf("\033[35md) Nível de Dificuldade atingido: Extremamente fácil\n\033[0m");
        }

    printf("\033[32mDeseja jogar novamente? (S/N)\033[0m\n");
    scanf(" %c", &jogar_novamente);
    pedras_eliminadas_total = 0;
    pedras_restantes = 48;
    contasalto = 0;
  } while (jogar_novamente == 'S' || jogar_novamente == 's');
    printf("\n\033[1;30;42mJogo Finalizado!\033[0m\n");
    printf("\n\033[1;30;42mObrigado por Jogar\033[0m\n");
    printf("\n\033[1;30;42mFeito Por Gabriel Victor!!\033[0m\n");
    return 0;
}
