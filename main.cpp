/**
 * @file   main.cpp
 * @brief  Jogo de dados estilo "Perudo" para multiplos jogadores no terminal.
 *
 * Cada jogador rola seus dados em segredo e, em turnos, faz chutes crescentes
 * sobre a soma total de todos os dados na mesa. Um jogador pode chamar "mentira"
 * no chute anterior; os dados sao revelados e o perdedor perde um dado.
 * Quem ficar sem dados e eliminado. Vence o ultimo jogador ativo.
 *
 * Configuracao lida de config.txt: <numJogadores> <numDados> <tempoTroca>
 */

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "layout.h"
#include "dados.h"
#include "cores.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <limits>

using namespace std;

const int LARGURA = 120; ///< Largura total do terminal em caracteres.

// ========================
// ESTRUTURAS
// ========================

/**
 * @brief Representa um jogador na partida.
 */
struct Jogador
{
    int id;              ///< Identificador numerico do jogador (1-based).
    string nome;         ///< Nome cru digitado pelo jogador.
    string nomeExibido;  ///< Nome formatado com cor ANSI e ID para exibicao.
    vector<int> dados;   ///< Valores atuais dos dados do jogador (1-6 cada).
    bool ativo;          ///< false quando o jogador e eliminado (sem dados).
};

// ========================
// TAMANHO VISIVEL (ignora sequencias ANSI)
// ========================

/**
 * @brief Calcula a largura visivel de uma string no terminal.
 *
 * Ignora sequencias de escape ANSI (ex.: \033[31m) e bytes de continuacao
 * UTF-8 (10xxxxxx), que nao ocupam coluna extra na tela.
 *
 * @param texto String possivelmente contendo escape ANSI e/ou UTF-8.
 * @return Numero de colunas que a string ocupa no terminal.
 */
int tamanhoVisivel(const string &texto)
{
    int tam = 0;
    bool emEscape = false;
    for (char c : texto)
    {
        if (c == '\033') { emEscape = true; continue; }  // inicio de sequencia ESC
        if (emEscape)    { if (c == 'm') emEscape = false; continue; } // fim em 'm'
        if ((unsigned char)c >= 0x80 && (unsigned char)c < 0xC0) continue; // byte de continuacao UTF-8
        tam++;
    }
    return tam;
}

// ========================
// CENTRALIZACAO
// ========================

/**
 * @brief Imprime texto centralizado na largura LARGURA, com efeito de digitacao.
 *
 * Calcula o padding com base na largura visivel (ignora escapes ANSI/UTF-8).
 * Aplica uma cor opcional antes do texto e reseta apos.
 *
 * @param texto        Texto a ser exibido.
 * @param ms_por_letra Delay em milissegundos entre cada caractere (0 = instantaneo).
 * @param cor          Sequencia ANSI de cor a aplicar (padrao: sem cor).
 */
void imprimirCentralizado(const string &texto, int ms_por_letra = 100, const string &cor = "")
{
    int espacos = (LARGURA - tamanhoVisivel(texto)) / 2;
    if (espacos < 0)
        espacos = 0;
    cout << string(espacos, ' ') << cor;
    for (char c : texto)
    {
        if (c != ' ')
            this_thread::sleep_for(chrono::milliseconds(ms_por_letra)); // pausa por letra (exceto espacos)
        cout << c << flush;
    }
    cout << RESET << "\n";
}

/**
 * @brief Atalho para imprimirCentralizado com delay padrao de 100 ms/letra.
 * @param texto        Texto a ser exibido.
 * @param ms_por_letra Delay em milissegundos entre cada caractere.
 */
void imprimirCentralizadoDevagar(const string &texto, int ms_por_letra = 100)
{
    imprimirCentralizado(texto, ms_por_letra);
}

/**
 * @brief Imprime uma linha separadora de LARGURA colunas.
 *
 * Repete @p simbolo (que pode ser UTF-8 multibyte como "⚀") @p colunas vezes.
 * Aguarda 500 ms antes de imprimir para dar ritmo visual.
 *
 * @param simbolo String (possivelmente UTF-8) usada como caractere de separacao.
 * @param colunas Numero de repeticoes do simbolo.
 */
void imprimirSeparador(const string &simbolo = "=", int colunas = LARGURA)
{
    this_thread::sleep_for(chrono::milliseconds(500));
    string linha = "";
    for (int i = 0; i < colunas; i++)
        linha += simbolo;
    cout << linha << "\n";
}

/**
 * @brief Imprime um bloco de linhas alinhado a esquerda, centrado como bloco.
 *
 * Calcula a largura da linha mais longa do bloco e usa esse valor
 * para determinar o padding esquerdo unico para todas as linhas.
 *
 * @param linhas Vetor de strings a serem impressas como bloco centrado.
 */
void imprimirBlocoCentralizado(const vector<string> &linhas)
{
    int larguraBloco = 0;
    for (const auto &l : linhas)
        if (tamanhoVisivel(l) > larguraBloco)
            larguraBloco = tamanhoVisivel(l);
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

/**
 * @brief Formata o ID numerico do jogador entre parenteses.
 * @param n Numero do jogador.
 * @return String no formato "(n)".
 */
string formatarId(int n)
{
    return "(" + to_string(n) + ")";
}

// ========================
// AGUARDAR TECLA
// ========================

/**
 * @brief Bloqueia a execucao ate o usuario pressionar qualquer tecla.
 *
 * Exibe um prompt "[enter]" piscante centralizado enquanto aguarda.
 * Usa _kbhit() para nao bloquear o loop de animacao.
 */
void aguardarTecla()
{
    cout << "\n";
    bool visivel = true;
    while (true)
    {
        if (_kbhit())
        {
            _getch(); // consome a tecla pressionada sem exibir
            break;
        }

        // Alterna entre exibir e apagar o prompt para criar efeito de piscar
        if (visivel)
        {
            imprimirCentralizado("[        ]", 0, VERMELHO_B);
            imprimirCentralizado("[  enter ]", 0, VERMELHO_B);
            imprimirCentralizado("[        ]", 0, VERMELHO_B);
            imprimirCentralizado("  [      ]", 0, VERMELHO_B);
            imprimirCentralizado("  [      ]", 0, VERMELHO_B);
            imprimirCentralizado("  [      ]", 0, VERMELHO_B);
            imprimirCentralizado("  [      ]", 0, VERMELHO_B);
        }
        else
        {
            imprimirCentralizado("          ", 0);
            imprimirCentralizado("          ", 0);
            imprimirCentralizado("          ", 0);
            imprimirCentralizado("          ", 0);
            imprimirCentralizado("          ", 0);
            imprimirCentralizado("          ", 0);
            imprimirCentralizado("          ", 0);
        }

        cout << "\033[A" << "\033[A" << "\033[A" << "\033[A" << "\033[A" << "\033[A" << "\033[A"; // sobe 7 linhas para sobrescrever na proxima iteracao
        visivel = !visivel;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << "\033[2K\n"; // limpa a linha do prompt ao sair
}

// ========================
// FUNCOES AUXILIARES
// ========================

/**
 * @brief Exibe a tela de troca de jogador e aguarda o tempo configurado.
 *
 * Limpa a tela, mostra o nome do proximo jogador e conta regressivamente
 * @p tempoEspera segundos para que o jogador anterior saia do teclado.
 *
 * @param nomeExibido Nome formatado (com cor) do proximo jogador.
 * @param tempoEspera Segundos de contagem regressiva antes de prosseguir.
 */
void aguardarTroca(const string &nomeExibido, int tempoEspera)
{
    this_thread::sleep_for(chrono::milliseconds(500));
    imprimirTitulo();
    imprimirCentralizado("TROCA DE JOGADOR");
    imprimirSeparador();
    cout << "\n";
    imprimirCentralizado("Proximo jogador: " + nomeExibido);
    imprimirCentralizado("Jogador anterior, saia do computador.");
    imprimirCentralizado(nomeExibido + ", prepare-se para jogar.");
    cout << "\n";

    for (int t = tempoEspera; t > 0; t--)
    {
        this_thread::sleep_for(chrono::seconds(1));
        string msg = "Aguardando " + to_string(t) + " segundo(s)...";
        imprimirCentralizado(msg, 0);
        cout << "\n";
    }
    imprimirSeparador();
    system("cls");
}

/**
 * @brief Le as configuracoes da partida a partir de um arquivo texto.
 *
 * O arquivo deve conter tres inteiros na ordem:
 * <numJogadores> <numDados> <tempoTroca>
 *
 * @param arquivo      Caminho para o arquivo de configuracao.
 * @param numJogadores [out] Numero de jogadores (minimo 2).
 * @param numDados     [out] Dados iniciais por jogador (minimo 1).
 * @param tempoTroca   [out] Segundos de espera na troca de jogador (>= 0).
 * @return true se a leitura e validacao foram bem-sucedidas, false caso contrario.
 */
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

/**
 * @brief Solicita e armazena o nome de cada jogador via entrada padrao.
 *
 * Preenche os campos nome e nomeExibido de cada Jogador no vetor.
 * Limpa a tela apos o cadastro.
 *
 * @param jogadores Vetor de jogadores com IDs ja inicializados.
 */
void lerNomesJogadores(vector<Jogador> &jogadores)
{
    imprimirSeparador("-");
    imprimirCentralizado("CADASTRO DE JOGADORES");
    imprimirSeparador("-");
    cout << "\n";
    for (auto &j : jogadores)
    {
        string prompt = "Digite o nome do jogador " + to_string(j.id) + ": ";
        int espacos = (LARGURA - (int)prompt.size()) / 2;
        cout << string(espacos, ' ') << prompt;
        cout << VERMELHO_B;
        cin >> j.nome;
        cout << RESET;
        j.nomeExibido = VERMELHO_B + j.nome + " " + formatarId(j.id) + RESET; // monta nome colorido com ID
    }
    system("cls");
}

/**
 * @brief Rola os dados de todos os jogadores ativos.
 *
 * Gera um valor aleatorio entre 1 e 6 para cada dado de cada jogador ativo.
 * Requer que srand() tenha sido chamado previamente.
 *
 * @param jogadores Vetor de jogadores a serem processados.
 */
void rolarDados(vector<Jogador> &jogadores)
{
    for (auto &j : jogadores)
        if (j.ativo)
            for (auto &d : j.dados)
                d = (rand() % 6) + 1;
}

/**
 * @brief Conta o total de dados ainda na mesa (jogadores ativos).
 * @param jogadores Vetor de jogadores.
 * @return Soma dos dados de todos os jogadores ativos.
 */
int contarDadosAtivos(const vector<Jogador> &jogadores)
{
    int total = 0;
    for (const auto &j : jogadores)
        if (j.ativo)
            total += (int)j.dados.size();
    return total;
}

/**
 * @brief Conta quantos jogadores ainda estao na partida.
 * @param jogadores Vetor de jogadores.
 * @return Numero de jogadores com ativo == true.
 */
int contarJogadoresAtivos(const vector<Jogador> &jogadores)
{
    int total = 0;
    for (const auto &j : jogadores)
        if (j.ativo)
            total++;
    return total;
}

/**
 * @brief Exibe o placar de dados restantes de cada jogador ativo.
 * @param jogadores Vetor de jogadores.
 */
void exibirDadosRestantes(const vector<Jogador> &jogadores)
{
    cout << "\n";
    imprimirSeparador("-");
    imprimirCentralizado("DADOS RESTANTES");
    imprimirSeparador("-");
    cout << "\n";
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

/**
 * @brief Exibe os dados do jogador atual em arte ASCII.
 * @param j Jogador cujos dados serao exibidos.
 */
void exibirDadosJogador(const Jogador &j)
{
    cout << "\n";
    imprimirCentralizado("Jogador atual: " + j.nomeExibido, 100, VERMELHO_B);
    cout << "\n";
    imprimirCentralizado("Seus dados:");
    cout << "\n";
    imprimirDados(j.dados); // renderiza os dados em 3D lado a lado
    cout << "\n";
}

/**
 * @brief Revela os dados de todos os jogadores ativos apos chamada de "mentira".
 *
 * Exibe os dados de cada jogador com um intervalo de 2 segundos entre eles
 * para criar suspense.
 *
 * @param jogadores Vetor de jogadores.
 */
void revelarDados(const vector<Jogador> &jogadores)
{
    imprimirSeparador("-");
    imprimirCentralizado("DADOS REVELADOS");
    imprimirSeparador("-");
    for (const auto &j : jogadores)
    {
        if (j.ativo)
        {
            this_thread::sleep_for(chrono::seconds(2)); // suspense antes de revelar cada jogador
            imprimirCentralizado(j.nomeExibido + ":");
            imprimirDados(j.dados);
            cout << "\n";
        }
    }
}

/**
 * @brief Calcula a soma real de todos os dados ativos na mesa.
 * @param jogadores Vetor de jogadores.
 * @return Soma de todos os valores de dados dos jogadores ativos.
 */
int calcularSomaReal(const vector<Jogador> &jogadores)
{
    int soma = 0;
    for (const auto &j : jogadores)
        if (j.ativo)
            for (int d : j.dados)
                soma += d;
    return soma;
}

/**
 * @brief Le e valida um chute inteiro dentro do intervalo [minVal, maxVal].
 *
 * Repete a leitura ate que o usuario informe um valor valido.
 *
 * @param minVal Valor minimo aceito (inclusive).
 * @param maxVal Valor maximo aceito (inclusive).
 * @return Chute valido digitado pelo usuario.
 */
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // descarta entrada invalida
        imprimirCentralizado("Chute invalido! Tente novamente.");
    }
}

/**
 * @brief Pergunta ao jogador se deseja dizer mentira ou fazer novo chute.
 *
 * Repete a leitura ate que o usuario escolha 1 ou 2.
 *
 * @return true se o jogador escolheu "dizer mentira", false para "novo chute".
 */
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // descarta entrada invalida
        imprimirCentralizado("Opcao invalida!");
    }
}

/**
 * @brief Retorna o indice do proximo jogador ativo apos @p indiceAtual.
 *
 * Percorre o vetor circularmente ate encontrar um jogador com ativo == true.
 *
 * @param jogadores   Vetor de jogadores.
 * @param indiceAtual Indice do jogador atual.
 * @return Indice do proximo jogador ativo.
 */
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

/**
 * @brief Executa uma rodada completa do jogo.
 *
 * Cada rodada consiste em:
 *  1. Rolar os dados de todos os jogadores ativos em segredo.
 *  2. Os jogadores se revezam fazendo chutes crescentes sobre a soma total.
 *  3. Um jogador pode chamar "mentira" no chute anterior.
 *  4. Os dados sao revelados; o perdedor perde um dado.
 *  5. Se o perdedor ficar sem dados, e eliminado.
 *
 * Apos a rodada, @p indiceInicio e atualizado para o jogador que sucede
 * o perdedor, que comeca a proxima rodada.
 *
 * @param jogadores    Vetor de jogadores (modificado: pode perder dados/ativo).
 * @param indiceInicio [in/out] Indice do jogador que comeca a rodada.
 * @param tempoTroca   Segundos de espera na tela de troca de jogador.
 * @param rodada       Numero da rodada atual (apenas para exibicao).
 */
void executarRodada(vector<Jogador> &jogadores, int &indiceInicio, int tempoTroca, int rodada)
{
    int dadosAtivos = contarDadosAtivos(jogadores);
    int somaMaxima  = dadosAtivos * 6; // maior chute possivel (todos os dados em 6)

    rolarDados(jogadores);

    int  ultimoChute          = -1;
    int  indiceUltimoChutador = -1;
    int  indiceAtual          = indiceInicio;
    bool primeiroChute        = true;  // true enquanto nenhum chute foi feito na rodada
    bool mentiraDita          = false;

    while (true)
    {
        aguardarTroca(jogadores[indiceAtual].nomeExibido, tempoTroca);

        this_thread::sleep_for(chrono::milliseconds(500));
        imprimirTitulo();
        imprimirCentralizado("RODADA " + to_string(rodada), 100, VERMELHO_B);
        cout << VERMELHO_B;
        imprimirSeparador();
        cout << RESET;
        exibirDadosRestantes(jogadores);
        imprimirSeparador("⚀");
        exibirDadosJogador(jogadores[indiceAtual]);
        imprimirSeparador("⚀");

        if (primeiroChute)
        {
            // Primeiro jogador da rodada: obrigado a fazer um chute inicial
            imprimirCentralizado("Ainda nao existe chute nesta rodada.");
            int chute = lerChute(1, somaMaxima);
            ultimoChute          = chute;
            indiceUltimoChutador = indiceAtual;
            imprimirCentralizado(jogadores[indiceAtual].nomeExibido + " chutou: " + to_string(chute));
            primeiroChute = false;
        }
        else
        {
            // Demais jogadores: podem aumentar o chute ou chamar mentira
            imprimirCentralizado("Ultimo chute: " + to_string(ultimoChute) +
                                 " (feito por " + jogadores[indiceUltimoChutador].nomeExibido + ")");
            imprimirCentralizado("Soma maxima possivel: " + to_string(somaMaxima));
            cout << "\n";

            if (ultimoChute >= somaMaxima)
            {
                // Chute no maximo: o jogador e forcado a chamar mentira
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
                    // Novo chute deve ser estritamente maior que o anterior
                    int chute = lerChute(ultimoChute + 1, somaMaxima);
                    indiceUltimoChutador = indiceAtual;
                    ultimoChute          = chute;
                    imprimirCentralizado(jogadores[indiceAtual].nomeExibido + " chutou: " + to_string(chute));
                }
            }
        }

        if (mentiraDita)
        {
            // Resolucao: revelar dados e determinar o perdedor
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
                // Chute era valido: quem chamou mentira perde
                perdedor = indiceAtual;
                imprimirCentralizado("A soma real era maior ou igual ao chute.");
                imprimirCentralizado(jogadores[perdedor].nomeExibido + " perde um dado!");
            }
            else
            {
                // Chute era invalido: quem chutou perde
                perdedor = indiceUltimoChutador;
                imprimirCentralizado("A soma real era menor que o chute.");
                imprimirCentralizado(jogadores[perdedor].nomeExibido + " perde um dado!");
            }

            jogadores[perdedor].dados.pop_back(); // remove um dado do perdedor
            imprimirCentralizado(jogadores[perdedor].nomeExibido + " perdeu um dado.");

            if (jogadores[perdedor].dados.empty())
            {
                // Sem dados: jogador e eliminado
                jogadores[perdedor].ativo = false;
                imprimirSeparador("*");
                imprimirCentralizado(jogadores[perdedor].nomeExibido + " ficou sem dados e foi ELIMINADO!");
                imprimirSeparador("*");
            }

            indiceInicio = proximoAtivo(jogadores, perdedor); // perdedor (ou proximo) abre a proxima rodada
            break;
        }

        indiceAtual = proximoAtivo(jogadores, indiceAtual);
    }
}

// ========================
// MAIN
// ========================

/**
 * @brief Ponto de entrada do programa.
 *
 * Inicializa o console Windows com suporte a VT100, le configuracoes,
 * cadastra jogadores e executa o loop principal de rodadas ate restar
 * um unico vencedor.
 *
 * @return 0 em sucesso, 1 se houver erro na leitura de configuracoes.
 */
int main()
{
    system("cls");

    // Habilita sequencias VT100/ANSI no console do Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    srand((unsigned int)time(nullptr)); // semente aleatoria baseada no tempo

    int numJogadores, numDados, tempoTroca;

    if (!lerConfiguracoes("config.txt", numJogadores, numDados, tempoTroca))
    {
        return 1;
    }

    // Inicializa vetor de jogadores com dados zerados e status ativo
    vector<Jogador> jogadores(numJogadores);
    for (int i = 0; i < numJogadores; i++)
    {
        jogadores[i].id          = i + 1;
        jogadores[i].nome        = "";
        jogadores[i].nomeExibido = "";
        jogadores[i].dados.resize(numDados, 0);
        jogadores[i].ativo       = true;
    }

    // Tela de boas-vindas com resumo da configuracao
    imprimirTitulo(1);
    imprimirCentralizado(
        "Jogadores: " + to_string(numJogadores) +
            "                                      Dados por jogador: " + to_string(numDados) +
            "                      Tempo de troca: " + to_string(tempoTroca) + " segundo(s)",
        100, VERMELHO_B);
    cout << "\n" << VERMELHO_B;
    imprimirSeparador();
    cout << RESET;
    aguardarTecla();
    system("cls");

    lerNomesJogadores(jogadores);

    // Confirmacao dos jogadores cadastrados
    imprimirSeparador();
    imprimirCentralizado("JOGADORES CADASTRADOS");
    imprimirSeparador();
    cout << "\n";
    for (const auto &j : jogadores)
        imprimirCentralizado(j.nomeExibido);
    system("cls");

    int indiceInicio = 0; // jogador que abre a primeira rodada
    int rodada       = 1;

    // Loop principal: continua ate restar apenas um jogador ativo
    while (contarJogadoresAtivos(jogadores) > 1)
    {
        executarRodada(jogadores, indiceInicio, tempoTroca, rodada);
        rodada++;
    }

    // Tela de fim de jogo com o vencedor
    imprimirTitulo();
    imprimirSeparador("*");
    imprimirCentralizado("FIM DE JOGO!");
    for (const auto &j : jogadores)
    {
        if (j.ativo)
        {
            imprimirCentralizado("Vencedor: " + j.nomeExibido);
            break;
        }
    }
    imprimirSeparador("*");

    return 0;
}