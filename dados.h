/**
 * @file   dados.h
 * @brief  Interface para renderizacao de dados em arte ASCII 3D no terminal.
 *
 * Declara as funcoes responsaveis por gerar e imprimir representacoes visuais
 * de dados (cubos de 6 faces) no terminal. Cada dado e exibido como um bloco
 * de 9 linhas simulando perspectiva isometrica, com a face superior destacada
 * em vermelho (\033[91m) e os pontos (●) do valor sorteado.
 *
 * Funcoes exportadas:
 *  - getDado()      : retorna o bloco ASCII de um unico dado.
 *  - imprimirDados(): imprime um vetor de dados lado a lado com seus valores.
 *
 * Dependencias: <iostream>, <vector>, <string> — incluidos em dados.cpp.
 */

#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

/**
 * @brief Retorna a arte ASCII de um dado com o valor especificado.
 *
 * Cada string do vetor retornado representa uma linha do bloco do dado.
 * O bloco tem 9 linhas de altura e aproximadamente 19 caracteres de largura.
 * A face superior (vermelha) exibe os pontos correspondentes ao valor;
 * as faces laterais exibem circulos decorativos (◯).
 *
 * @param valor Valor do dado (1 a 6). Valores fora do intervalo exibem "?".
 * @return Vetor de 9 strings representando o dado linha a linha.
 */
vector<string> getDado(int valor);

/**
 * @brief Imprime todos os dados de uma jogada lado a lado no terminal.
 *
 * Monta os blocos de cada dado e os imprime sincronizados linha por linha,
 * criando a ilusao de dados dispostos horizontalmente. Abaixo de cada dado
 * exibe o valor numerico entre colchetes em vermelho.
 *
 * @param dados Vetor de valores inteiros (1–6) a serem exibidos como dados.
 */
void imprimirDados(const vector<int> &dados);