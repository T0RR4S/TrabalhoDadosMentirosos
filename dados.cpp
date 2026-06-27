/**
 * @file   dados.cpp
 * @brief  Renderizacao de dados (cubos) em arte ASCII 3D no terminal.
 *
 * Cada dado e representado como um bloco de 9 linhas simulando perspectiva
 * isometrica com tres faces visiveis. A face superior e destacada em vermelho
 * (\033[91m) e contem os pontos (●) do valor sorteado. As faces laterais
 * exibem circulos vazios (◯) decorativos.
 *
 * Uso principal: imprimirDados() recebe um vetor de valores e os imprime
 * lado a lado na tela, seguidos do valor numerico entre colchetes.
 */

#include "dados.h"

/**
 * @brief Retorna a arte ASCII de um dado com o valor especificado.
 *
 * Cada string do vetor retornado representa uma linha do bloco do dado.
 * O dado tem 9 linhas de altura e ocupa aproximadamente 19 caracteres de largura.
 * A face superior (vermelha) mostra os pontos do valor; as demais faces
 * exibem circulos decorativos (◯).
 *
 * @param valor Valor do dado (1 a 6). Qualquer outro valor exibe "?".
 * @return Vetor de 9 strings representando o dado linha a linha.
 */
vector<string> getDado(int valor)
{
    switch (valor)
    {
    case 1:
        return {
            "    \033[91mo--------o\033[0m     ",
            "   / \033[91m\\        \\\033[0m    ",
            "  /   \033[91m\\    ●   \\\033[0m   ", // 1 ponto central na face superior
            " /     \033[91m\\        \\\033[0m  ",
            "o◯     ◯\033[91mo--------o\033[0m ",
            " \\     /       ◯/  ",
            "  \\   /    ◯   /   ",
            "   \\ /◯       /    ",
            "    o--------o     ",
        };
    case 2:
        return {
            "    \033[91mo--------o\033[0m     ",
            "   / \033[91m\\       ●\\\033[0m    ",
            "  /   \033[91m\\        \\\033[0m   ", // 2 pontos diagonais na face superior
            " /     \033[91m\\●       \\\033[0m  ",
            "o   ◯   \033[91mo--------o\033[0m ",
            " \\     /◯      ◯/  ",
            "  \\   /        /   ",
            "   \\ /◯      ◯/    ",
            "    o--------o     ",
        };
    case 3:
        return {
            "    \033[91mo--------o\033[0m     ",
            "   /◯\033[91m\\       ●\\\033[0m    ",
            "  /  ◯\033[91m\\    ●   \\\033[0m   ", // 3 pontos diagonais na face superior
            " /     \033[91m\\●       \\\033[0m  ",
            "o◯     ◯\033[91mo--------o\033[0m ",
            " \\◯    /◯      ◯/  ",
            "  \\   /    ◯   /   ",
            "   \\◯/◯      ◯/    ",
            "    o--------o     ",
        };
    case 4:
        return {
            "    \033[91mo--------o\033[0m     ",
            "   /◯\033[91m\\●      ●\\\033[0m    ",
            "  /   \033[91m\\        \\\033[0m   ", // 4 pontos nos cantos da face superior
            " /     \033[91m\\●      ●\\\033[0m  ",
            "o◯  ◯  ◯\033[91mo--------o\033[0m ",
            " \\     /◯      ◯/  ",
            "  \\   /◯      ◯/   ",
            "   \\◯/◯      ◯/    ",
            "    o--------o     ",
        };
    case 5:
        return {
            "    \033[91mo--------o\033[0m     ",
            "   /◯\033[91m\\●      ●\\\033[0m    ",
            "  /   \033[91m\\    ●   \\\033[0m   ", // 5 pontos: 4 cantos + 1 central
            " /     \033[91m\\●      ●\\\033[0m  ",
            "o◯     ◯\033[91mo--------o\033[0m ",
            " \\     /        /  ",
            "  \\   /    ◯   /   ",
            "   \\◯/        /    ",
            "    o--------o     ",
        };
    case 6:
        return {
            "    \033[91mo--------o\033[0m     ",
            "   / \033[91m\\●      ●\\\033[0m    ",
            "  /   \033[91m\\●      ●\\\033[0m   ", // 6 pontos em duas colunas
            " /     \033[91m\\●      ●\\\033[0m  ",
            "o◯  ◯  ◯\033[91mo--------o\033[0m ",
            " \\     /       ◯/  ",
            "  \\   /        /   ",
            "   \\ /◯       /    ",
            "    o--------o     ",
        };
    default: // valor fora do intervalo 1-6: exibe "?" nas posicoes dos pontos
        return {
            "    \033[91m\033[91mo--------o\033[0m\033[0m     ",
            "   / \033[91m\\        \\\033[0m    ",
            "  /   \033[91m\\    ?   \\\033[0m   ",
            " /     \033[91m\\        \\\033[0m  ",
            "o   ?   \033[91m\033[91mo--------o\033[0m\033[0m ",
            " \\     /        /  ",
            "  \\   /    ?   /   ",
            "   \\ /        /    ",
            "    o--------o     ",
        };
    }
}

/**
 * @brief Imprime todos os dados de uma jogada lado a lado no terminal.
 *
 * Monta os blocos de cada dado e os imprime sincronizados linha por linha,
 * criando a ilusao de dados dispostos horizontalmente. Abaixo de cada dado
 * exibe o valor numerico entre colchetes em vermelho.
 *
 * Nao centraliza a saida; o alinhamento depende do contexto de chamada.
 *
 * @param dados Vetor de valores inteiros (1-6) a serem exibidos como dados.
 */
void imprimirDados(const vector<int> &dados)
{
    // Monta o bloco ASCII de cada dado
    vector<vector<string>> blocos;
    for (int d : dados)
        blocos.push_back(getDado(d));

    // Imprime os blocos sincronizados linha por linha (9 linhas por dado)
    for (int linha = 0; linha < 9; linha++)
    {
        for (int i = 0; i < (int)blocos.size(); i++)
        {
            cout << blocos[i][linha];
        }
        cout << "\n";
    }

    // Exibe o valor numerico abaixo de cada dado
    for (int d : dados)
    {
        if (d > 0 && d < 7)
            cout << "\033[91m       [" << d << "]         \033[0m"; // valor valido em vermelho
        else
            cout << "\033[91m       [?]         \033[0m"; // valor invalido
    }
    cout << "\n";
}