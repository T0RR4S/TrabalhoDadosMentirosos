/**
 * @file   layout3.h
 * @brief  Interface para a arte ASCII/Braille da tela de vitoria.
 *
 * Declara a funcao imprimirVitoria(), que exibe a animacao de vitoria
 * de um jogador. Deve ser incluido em qualquer unidade de traducao que
 * precise exibir a tela de vitoria (ex.: main.cpp).
 *
 * Dependencias: nenhuma alem de cabecalhos padrao (iostream, string, vector,
 * chrono, thread), incluidos internamente em layout3.cpp.
 */

#ifndef LAYOUT3_H
#define LAYOUT3_H

/**
 * @brief Imprime a arte Braille/ASCII da tela de vitoria.
 *
 * @param i Se diferente de 0, adiciona delay de 100 ms entre linhas
 *          para efeito de aparecimento progressivo. Se 0 (padrao),
 *          imprime instantaneamente.
 */
void imprimirVitoria(int i = 0);

#endif // LAYOUT3_H