/**
 * @file   layout2.h
 * @brief  Interface para a arte ASCII/Braille da tela de derrota.
 *
 * Declara a funcao imprimirDerrota(), que exibe a animacao de eliminacao
 * de um jogador. Deve ser incluido em qualquer unidade de traducao que
 * precise exibir a tela de derrota (ex.: main.cpp).
 *
 * Dependencias: nenhuma alem de cabecalhos padrao (iostream, string, vector,
 * chrono, thread), incluidos internamente em layout2.cpp.
 */

#ifndef LAYOUT2_H
#define LAYOUT2_H

/**
 * @brief Imprime a arte Braille/ASCII da tela de derrota.
 *
 * @param i Se diferente de 0, adiciona delay de 100 ms entre linhas
 *          para efeito de aparecimento progressivo. Se 0 (padrao),
 *          imprime instantaneamente.
 */
void imprimirDerrota(int i = 0);

#endif // LAYOUT2_H