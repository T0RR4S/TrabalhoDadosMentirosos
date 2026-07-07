#ifndef HISTORIA_H
#define HISTORIA_H

#include <vector>
#include <string>

/**
 * @brief Representa um slide da historia inicial.
 */
struct Slide
{
    std::vector<std::string> arte; ///< Linhas da arte ASCII/Braille do slide (largura ja pronta, pode conter escapes ANSI).
    std::string texto;             ///< Texto narrativo exibido abaixo da arte.
};

/**
 * @brief Exibe a sequencia de slides da historia inicial.
 *
 * Os slides (arte + texto) sao montados internamente em historia.cpp,
 * na funcao montarSlides() -- mesmo padrao usado por imprimirTitulo() em
 * layout.cpp, onde a arte fica embutida na propria unidade de traducao.
 *
 * Enter avanca para o proximo slide. Backspace volta ao slide anterior
 * (no primeiro slide, Backspace tambem sai). Espaco sai a qualquer
 * momento. No ultimo slide, o prompt vira "Voce(s) aceitam?": Enter
 * aceita e inicia o jogo, Backspace sai (mesmo comportamento do Espaco).
 *
 * @return true se aceitou (Enter no ultimo slide), false se saiu.
 */
bool exibirHistoria();

#endif // HISTORIA_H