#include <iostream>
#include <iomanip>
#include <fstream>
#include "layout.h"
#include "dados.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <limits>

using namespace std;

const int LARGURA = 120;

// ========================
// ESTRUTURAS
// ========================

struct Jogador
{
    int id;
    string nome;
    string nomeExibido;
    vector<int> dados;
    bool ativo;
};

// ========================
// CENTRALIZACAO
// ========================

void imprimirCentralizado(const string &texto, int ms_por_letra = 50)
{
    int espacos = (LARGURA - (int)texto.size()) / 2;
    if (espacos < 0)
        espacos = 0;
    cout << string(espacos, ' ');
    for (char c : texto)
    {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(ms_por_letra));
    }
    cout << "\n";
}

void imprimirCentralizadoDevagar(const string &texto, int ms_por_letra = 100)
{
    imprimirCentralizado(texto, ms_por_letra);
}

void imprimirSeparador(char c = '=')
{
    cout << string(LARGURA, c) << "\n";
}

void imprimirBlocoCentralizado(const vector<string> &linhas)
{
    int larguraBloco = 0;
    for (const auto &l : linhas)
        if ((int)l.size() > larguraBloco)
            larguraBloco = (int)l.size();
    int espacos = (LARGURA - larguraBloco) / 2;
    if (espacos < 0)
        espacos = 0;
    string pad = string(espacos, ' ');
    for (const auto &l : linhas)
        cout << pad << l << "\n";
}

// ========================
// NOME COM ID
// ========================

string formatarId(int n)
{
    return "(" + to_string(n) + ")";
}

// ========================
// FUNCOES AUXILIARES
// ========================

void aguardarTroca(const string &nomeExibido, int tempoEspera)
{
    imprimirTitulo();
    imprimirSeparador();
    imprimirCentralizado("TROCA DE JOGADOR");
    imprimirSeparador();
    imprimirCentralizado("Proximo jogador: " + nomeExibido);
    imprimirCentralizado("Jogador anterior, saia do computador.");
    imprimirCentralizado(nomeExibido + ", prepare-se para jogar.");
    cout << "\n";

    for (int t = tempoEspera; t >= 0; t--)
    {
        this_thread::sleep_for(chrono::seconds(1));
        if (t > 0)
        {
            string msg = "Aguardando " + to_string(t) + " segundo(s)...";
            imprimirCentralizado(msg, 0);
        }
    }
    imprimirSeparador();
}

bool lerConfiguracoes(const string &arquivo, int &numJogadores, int &numDados, int &tempoTroca)
{
    ifstream arqEntrada;
    arqEntrada.open(arquivo, ios::in);

    if (!arqEntrada)
    {
        cout << "Erro: nao foi possivel abrir o arquivo " << arquivo << ".\n";
        return false;
    }

    arqEntrada >> numJogadores >> numDados >> tempoTroca;
    arqEntrada.close();

    if (numJogadores < 2)
    {
        cout << "Erro: numero de jogadores invalido.\n";
        cout << "O jogo deve possuir pelo menos 2 jogadores.\n";
        return false;
    }
    if (numDados < 1)
    {
        cout << "Erro: numero de dados por jogador invalido.\n";
        cout << "Cada jogador deve possuir pelo menos 1 dado.\n";
        return false;
    }
    if (tempoTroca < 0)
    {
        cout << "Erro: tempo de troca invalido.\n";
        cout << "O tempo de troca deve ser maior ou igual a 0.\n";
        return false;
    }

    return true;
}

void lerNomesJogadores(vector<Jogador> &jogadores)
{
    imprimirSeparador('-');
    imprimirCentralizado("CADASTRO DE JOGADORES");
    imprimirSeparador('-');
    for (auto &j : jogadores)
    {
        string prompt = "Digite o nome do jogador " + to_string(j.id) + ": ";
        int espacos = (LARGURA - (int)prompt.size()) / 2;
        cout << string(espacos, ' ') << prompt;
        cin >> j.nome;
        j.nomeExibido = j.nome + " " + formatarId(j.id);
    }
    cout << "\n";
}

void rolarDados(vector<Jogador> &jogadores)
{
    for (auto &j : jogadores)
        if (j.ativo)
            for (auto &d : j.dados)
                d = (rand() % 6) + 1;
}

int contarDadosAtivos(const vector<Jogador> &jogadores)
{
    int total = 0;
    for (const auto &j : jogadores)
        if (j.ativo)
            total += (int)j.dados.size();
    return total;
}

int contarJogadoresAtivos(const vector<Jogador> &jogadores)
{
    int total = 0;
    for (const auto &j : jogadores)
        if (j.ativo)
            total++;
    return total;
}

void exibirDadosRestantes(const vector<Jogador> &jogadores)
{
    imprimirSeparador('-');
    imprimirCentralizado("DADOS RESTANTES");
    imprimirSeparador('-');
    for (const auto &j : jogadores)
    {
        if (j.ativo)
        {
            string linha = j.nomeExibido + ": " + to_string(j.dados.size()) + " dado(s)";
            imprimirCentralizado(linha);
        }
    }
    cout << "\n";
}

void exibirDadosJogador(const Jogador &j)
{
    imprimirCentralizado("Jogador atual: " + j.nomeExibido);
    imprimirCentralizado("Seus dados:");
    cout << "\n";
    imprimirDados(j.dados);
    cout << "\n";
}

void revelarDados(const vector<Jogador> &jogadores)
{
    imprimirSeparador('-');
    imprimirCentralizado("DADOS REVELADOS");
    imprimirSeparador('-');
    for (const auto &j : jogadores)
    {
        if (j.ativo)
        {
            this_thread::sleep_for(chrono::seconds(2));
            imprimirCentralizado(j.nomeExibido + ":");
            imprimirDados(j.dados);
            cout << "\n";
        }
    }
}

int calcularSomaReal(const vector<Jogador> &jogadores)
{
    int soma = 0;
    for (const auto &j : jogadores)
        if (j.ativo)
            for (int d : j.dados)
                soma += d;
    return soma;
}

int lerChute(int minVal, int maxVal)
{
    int chute;
    while (true)
    {
        string prompt = "Digite seu chute (" + to_string(minVal) + " a " + to_string(maxVal) + "): ";
        int espacos = (LARGURA - (int)prompt.size()) / 2;
        cout << string(espacos, ' ') << prompt;
        if (cin >> chute && chute >= minVal && chute <= maxVal)
            return chute;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        imprimirCentralizado("Chute invalido! Tente novamente.");
    }
}

bool lerOpcao()
{
    int opcao;
    while (true)
    {
        imprimirCentralizado("Escolha uma opcao:");
        imprimirCentralizado("1 - Dizer mentira");
        imprimirCentralizado("2 - Fazer novo chute");
        string prompt = "Opcao: ";
        int espacos = (LARGURA - (int)prompt.size()) / 2;
        cout << string(espacos, ' ') << prompt;
        if (cin >> opcao && (opcao == 1 || opcao == 2))
            return opcao == 1;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        imprimirCentralizado("Opcao invalida!");
    }
}

int proximoAtivo(const vector<Jogador> &jogadores, int indiceAtual)
{
    int n = (int)jogadores.size();
    int proximo = (indiceAtual + 1) % n;
    while (!jogadores[proximo].ativo)
        proximo = (proximo + 1) % n;
    return proximo;
}

// ========================
// LOGICA DE RODADA
// ========================

void executarRodada(vector<Jogador> &jogadores, int &indiceInicio, int tempoTroca, int rodada)
{
    int dadosAtivos = contarDadosAtivos(jogadores);
    int somaMaxima = dadosAtivos * 6;

    rolarDados(jogadores);

    int ultimoChute = -1;
    int indiceUltimoChutador = -1;
    int indiceAtual = indiceInicio;
    bool primeiroChute = true;
    bool mentiraDita = false;

    while (true)
    {
        // 1) tela de troca com timer
        aguardarTroca(jogadores[indiceAtual].nomeExibido, tempoTroca);

        // 2) apos timer: layout + cabecalho da rodada + dados restantes + dados do jogador
        imprimirTitulo();
        imprimirSeparador();
        imprimirCentralizado("RODADA " + to_string(rodada));
        imprimirSeparador();
        exibirDadosRestantes(jogadores);
        imprimirSeparador('-');
        exibirDadosJogador(jogadores[indiceAtual]);
        imprimirSeparador('-');

        if (primeiroChute)
        {
            imprimirCentralizado("Ainda nao existe chute nesta rodada.");
            int chute = lerChute(1, somaMaxima);
            ultimoChute = chute;
            indiceUltimoChutador = indiceAtual;
            imprimirCentralizado(jogadores[indiceAtual].nomeExibido + " chutou: " + to_string(chute));
            primeiroChute = false;
        }
        else
        {
            imprimirCentralizado("Ultimo chute: " + to_string(ultimoChute) +
                                 " (feito por " + jogadores[indiceUltimoChutador].nomeExibido + ")");
            imprimirCentralizado("Soma maxima possivel: " + to_string(somaMaxima));
            cout << "\n";

            if (ultimoChute >= somaMaxima)
            {
                imprimirCentralizado("O chute anterior ja e o maximo. Voce deve dizer mentira!");
                mentiraDita = true;
            }
            else
            {
                bool escolheuMentira = lerOpcao();
                if (escolheuMentira)
                {
                    mentiraDita = true;
                }
                else
                {
                    int chute = lerChute(ultimoChute + 1, somaMaxima);
                    indiceUltimoChutador = indiceAtual;
                    ultimoChute = chute;
                    imprimirCentralizado(jogadores[indiceAtual].nomeExibido + " chutou: " + to_string(chute));
                }
            }
        }

        if (mentiraDita)
        {
            this_thread::sleep_for(chrono::seconds(2));
            imprimirSeparador();
            imprimirCentralizado(jogadores[indiceAtual].nomeExibido + " disse: MENTIRA!");
            imprimirSeparador();

            revelarDados(jogadores);

            int somaReal = calcularSomaReal(jogadores);

            this_thread::sleep_for(chrono::seconds(2));
            imprimirCentralizado("Ultimo chute: " + to_string(ultimoChute));

            this_thread::sleep_for(chrono::seconds(2));
            imprimirCentralizadoDevagar("Soma real:    " + to_string(somaReal));
            cout << "\n";

            int perdedor;
            if (somaReal >= ultimoChute)
            {
                perdedor = indiceAtual;
                imprimirCentralizado("A soma real era maior ou igual ao chute.");
                imprimirCentralizado(jogadores[perdedor].nomeExibido + " perde um dado!");
            }
            else
            {
                perdedor = indiceUltimoChutador;
                imprimirCentralizado("A soma real era menor que o chute.");
                imprimirCentralizado(jogadores[perdedor].nomeExibido + " perde um dado!");
            }

            jogadores[perdedor].dados.pop_back();
            imprimirCentralizado(jogadores[perdedor].nomeExibido + " perdeu um dado.");

            if (jogadores[perdedor].dados.empty())
            {
                jogadores[perdedor].ativo = false;
                imprimirSeparador('*');
                imprimirCentralizado(jogadores[perdedor].nomeExibido + " ficou sem dados e foi ELIMINADO!");
                imprimirSeparador('*');
            }

            indiceInicio = proximoAtivo(jogadores, perdedor);
            break;
        }

        indiceAtual = proximoAtivo(jogadores, indiceAtual);
    }
}

// ========================
// MAIN
// ========================

int main()
{
    srand((unsigned int)time(nullptr));

    int numJogadores, numDados, tempoTroca;

    if (!lerConfiguracoes("config.txt", numJogadores, numDados, tempoTroca))
    {
        return 1;
    }

    vector<Jogador> jogadores(numJogadores);
    for (int i = 0; i < numJogadores; i++)
    {
        jogadores[i].id = i + 1;
        jogadores[i].nome = "";
        jogadores[i].nomeExibido = "";
        jogadores[i].dados.resize(numDados, 0);
        jogadores[i].ativo = true;
    }

    imprimirTitulo();
    
    imprimirCentralizado(
        "Jogadores: " + to_string(numJogadores) +
        "                                      Dados por jogador: " + to_string(numDados) +
        "                      Tempo de troca: " + to_string(tempoTroca) + " segundo(s)");

    imprimirSeparador();

    cout << "\n";

    lerNomesJogadores(jogadores);

    imprimirSeparador('-');
    imprimirCentralizado("JOGADORES CADASTRADOS");
    imprimirSeparador('-');
    for (const auto &j : jogadores)
        imprimirCentralizado(j.nomeExibido);
    cout << "\n";

    int indiceInicio = 0;
    int rodada = 1;

    while (contarJogadoresAtivos(jogadores) > 1)
    {
        executarRodada(jogadores, indiceInicio, tempoTroca, rodada);
        rodada++;
    }

    imprimirTitulo();
    imprimirSeparador('*');
    imprimirCentralizado("FIM DE JOGO!");
    for (const auto &j : jogadores)
    {
        if (j.ativo)
        {
            imprimirCentralizado("Vencedor: " + j.nomeExibido);
            break;
        }
    }
    imprimirSeparador('*');

    return 0;
}