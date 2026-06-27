/**
 * @file   cores.h
 * @brief  Macros de sequencias de escape ANSI para estilizacao do terminal.
 *
 * Define constantes de pre-processador para reset, estilos de texto,
 * cores de primeiro plano (normais e bright) e cores de fundo (normais e bright),
 * seguindo o padrao VT100/ANSI (ISO 6429).
 *
 * Uso: inclua este cabecalho e concatene as macros diretamente em strings
 * enviadas para cout. Sempre finalize sequencias de cor com RESET para
 * evitar que o estilo "vaze" para o restante da saida.
 *
 * Exemplo:
 * @code
 *   cout << VERMELHO_B << "texto em vermelho brilhante" << RESET << "\n";
 * @endcode
 *
 * Requisito: o console deve suportar VT100. No Windows, habilite via
 * SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING).
 */

#pragma once

// ========================
// RESET
// ========================

#define RESET        "\033[0m"  ///< Restaura todos os atributos ao padrao do terminal.

// ========================
// ESTILOS DE TEXTO
// ========================

#define NEGRITO      "\033[1m"  ///< Texto em negrito (ou maior intensidade).
#define DIM          "\033[2m"  ///< Texto com intensidade reduzida.
#define ITALICO      "\033[3m"  ///< Texto em italico (suporte varia por terminal).
#define SUBLINHADO   "\033[4m"  ///< Texto sublinhado.

// ========================
// CORES DE PRIMEIRO PLANO — NORMAIS
// ========================

#define PRETO        "\033[30m" ///< Texto preto.
#define VERMELHO     "\033[31m" ///< Texto vermelho.
#define VERDE        "\033[32m" ///< Texto verde.
#define AMARELO      "\033[33m" ///< Texto amarelo.
#define AZUL         "\033[34m" ///< Texto azul.
#define MAGENTA      "\033[35m" ///< Texto magenta.
#define CIANO        "\033[36m" ///< Texto ciano.
#define BRANCO       "\033[37m" ///< Texto branco.

// ========================
// CORES DE PRIMEIRO PLANO — BRIGHT (alta intensidade)
// ========================

#define PRETO_B      "\033[90m" ///< Texto preto brilhante (cinza escuro).
#define VERMELHO_B   "\033[91m" ///< Texto vermelho brilhante.
#define VERDE_B      "\033[92m" ///< Texto verde brilhante.
#define AMARELO_B    "\033[93m" ///< Texto amarelo brilhante.
#define AZUL_B       "\033[94m" ///< Texto azul brilhante.
#define MAGENTA_B    "\033[95m" ///< Texto magenta brilhante.
#define CIANO_B      "\033[96m" ///< Texto ciano brilhante.
#define BRANCO_B     "\033[97m" ///< Texto branco brilhante.

// ========================
// CORES DE FUNDO — NORMAIS
// ========================

#define FUNDO_PRETO      "\033[40m"  ///< Fundo preto.
#define FUNDO_VERMELHO   "\033[41m"  ///< Fundo vermelho.
#define FUNDO_VERDE      "\033[42m"  ///< Fundo verde.
#define FUNDO_AMARELO    "\033[43m"  ///< Fundo amarelo.
#define FUNDO_AZUL       "\033[44m"  ///< Fundo azul.
#define FUNDO_MAGENTA    "\033[45m"  ///< Fundo magenta.
#define FUNDO_CIANO      "\033[46m"  ///< Fundo ciano.
#define FUNDO_BRANCO     "\033[47m"  ///< Fundo branco.

// ========================
// CORES DE FUNDO — BRIGHT (alta intensidade)
// ========================

#define FUNDO_PRETO_B    "\033[100m" ///< Fundo preto brilhante.
#define FUNDO_VERMELHO_B "\033[101m" ///< Fundo vermelho brilhante.
#define FUNDO_VERDE_B    "\033[102m" ///< Fundo verde brilhante.
#define FUNDO_AMARELO_B  "\033[103m" ///< Fundo amarelo brilhante.
#define FUNDO_AZUL_B     "\033[104m" ///< Fundo azul brilhante.
#define FUNDO_MAGENTA_B  "\033[105m" ///< Fundo magenta brilhante.
#define FUNDO_CIANO_B    "\033[106m" ///< Fundo ciano brilhante.
#define FUNDO_BRANCO_B   "\033[107m" ///< Fundo branco brilhante.