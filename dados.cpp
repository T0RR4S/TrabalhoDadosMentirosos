#include "dados.h"

vector<string> getDado(int valor)
{
    switch (valor)
    {
    case 1:
        return {
            "    o--------o     ",
            "   / \\        \\    ",
            "  /   \\    ●   \\   ",
            " /     \\        \\  ",
            "o◯     ◯o--------o ",
            " \\     /       ◯/  ",
            "  \\   /    ◯   /   ",
            "   \\ /◯       /    ",
            "    o--------o     ",
        };
    case 2:
        return {
            "    o--------o     ",
            "   / \\       ●\\    ",
            "  /   \\        \\   ",
            " /     \\●       \\  ",
            "o   ◯   o--------o ",
            " \\     /◯      ◯/  ",
            "  \\   /        /   ",
            "   \\ /◯      ◯/    ",
            "    o--------o     ",
        };
    case 3:
        return {
            "    o--------o     ",
            "   /◯\\       ●\\    ",
            "  /  ◯\\    ●   \\   ",
            " /     \\●       \\  ",
            "o◯     ◯o--------o ",
            " \\◯    /◯      ◯/  ",
            "  \\   /    ◯   /   ",
            "   \\◯/◯      ◯/    ",
            "    o--------o     ",
        };
    case 4:
        return {
            "    o--------o     ",
            "   /◯\\●      ●\\    ",
            "  /   \\        \\   ",
            " /     \\●      ●\\  ",
            "o◯  ◯  ◯o--------o ",
            " \\     /◯      ◯/  ",
            "  \\   /◯      ◯/   ",
            "   \\◯/◯      ◯/    ",
            "    o--------o     ",
        };
    case 5:
        return {
            "    o--------o     ",
            "   /◯\\●      ●\\    ",
            "  /   \\    ●   \\   ",
            " /     \\●      ●\\  ",
            "o◯     ◯o--------o ",
            " \\     /        /  ",
            "  \\   /    ◯   /   ",
            "   \\◯/        /    ",
            "    o--------o     ",
        };
    case 6:
        return {
            "    o--------o     ",
            "   / \\●      ●\\    ",
            "  /   \\●      ●\\   ",
            " /     \\●      ●\\  ",
            "o◯  ◯  ◯o--------o ",
            " \\     /       ◯/  ",
            "  \\   /        /   ",
            "   \\ /◯       /    ",
            "    o--------o     ",
        };
    default:
        return {
            "    o--------o     ",
            "   / \\        \\    ",
            "  /   \\    ?   \\   ",
            " /     \\        \\  ",
            "o   ?   o--------o ",
            " \\     /        /  ",
            "  \\   /    ?   /   ",
            "   \\ /        /    ",
            "    o--------o     ",
        };
    }
}

void imprimirDados(const vector<int> &dados)
{
    // Monta todos os blocos
    vector<vector<string>> blocos;
    for (int d : dados)
        blocos.push_back(getDado(d));

    // Imprime linha por linha lado a lado
    for (int linha = 0; linha < 9; linha++)
    {
        for (int i = 0; i < (int)blocos.size(); i++)
        {
            cout << blocos[i][linha];
        }
        cout << "\n";
    }

    // Numeros embaixo
    for (int d : dados)
    {
        if (d > 0 && d < 7)
            cout << "       [" << d << "]         ";
        else
            cout << "       [?]         ";
    }
    cout << "\n";
}