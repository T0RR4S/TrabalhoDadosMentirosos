/**
 * @file   layout.h
 * @brief  Interface para a arte ASCII/Braille do titulo do jogo.
 *
 * Declara a funcao imprimirTitulo(), que exibe o logotipo animado do jogo
 * na tela de abertura e como cabecalho durante a partida. Deve ser incluido
 * em qualquer unidade de traducao que precise exibir o titulo
 * (ex.: main.cpp).
 *
 * Dependencias: nenhuma alem de cabecalhos padrao (iostream, string, vector,
 * chrono, thread), incluidos internamente em layout.cpp.
 */

#ifndef LAYOUT_H
#define LAYOUT_H

/**
 * @brief Imprime o titulo do jogo em arte Braille/ASCII colorida.
 *
 * @param i Se diferente de 0, adiciona delay de 100 ms entre linhas
 *          para efeito de aparecimento progressivo na tela de abertura.
 *          Se 0 (padrao), imprime instantaneamente.
 */
void imprimirTitulo(int i = 0);

#endif // LAYOUT_H