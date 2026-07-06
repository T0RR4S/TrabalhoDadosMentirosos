apagar dps:

"[                                                    ]"
"[                      história                      ]"
"[                                                    ]"

# DadosMentirosos

Jogo de dados estilo **Perudo** para múltiplos jogadores no terminal Windows,
com arte ASCII/Braille, efeitos de digitação e suporte a cores ANSI.

---

## Compilação e execução

```
clear
chcp 65001
g++ -std=c++17 main.cpp dados.cpp layout.cpp layout2.cpp historia.cpp -o jogo.exe
./jogo.exe
```

- `chcp 65001` define a página de código UTF-8 no terminal Windows, necessária
  para renderizar corretamente os caracteres Braille e os símbolos dos dados.
- Requer **g++ com suporte a C++17** (MinGW-w64 ou equivalente).
- O executável gerado é `jogo.exe` e deve ser rodado no mesmo diretório que
  o arquivo `config.txt`.

---

## Configuração (`config.txt`)

O arquivo `config.txt` deve conter três inteiros separados por espaço:

```
<numJogadores> <numDados> <tempoTroca>
```

| Campo          | Descrição                                              | Mínimo |
|----------------|--------------------------------------------------------|--------|
| `numJogadores` | Número de jogadores na partida                         | 2      |
| `numDados`     | Quantidade de dados inicial por jogador                | 1      |
| `tempoTroca`   | Segundos de espera na tela de troca de jogador         | 0      |

Exemplo:

```
2 1 3
```

---

## Regras do jogo

1. Cada jogador rola seus dados **em segredo** no início de cada rodada.
2. Os jogadores se revezam fazendo **chutes crescentes** sobre a **soma total**
   de todos os dados na mesa (valores de 1 a 6 por dado).
3. O jogador da vez pode:
   - **Chutar** um valor maior que o chute anterior.
   - **Chamar mentira** no chute do jogador anterior.
4. Ao chamar mentira, todos os dados são revelados:
   - Se a soma real **≥ chute**: quem chamou mentira perde um dado.
   - Se a soma real **< chute**: quem fez o chute perde um dado.
5. Sem dados, o jogador é **eliminado**.
6. Vence o **último jogador ativo**.

---

## Estrutura do projeto

```
DadosMentirosos/
├── main.cpp      — Lógica principal, loop de rodadas e interface com o usuário
├── dados.cpp     — Renderização dos dados em arte ASCII 3D
├── dados.h       — Interface de dados.cpp
├── layout.cpp    — Arte Braille/ASCII do título do jogo
├── layout.h      — Interface de layout.cpp
├── layout2.cpp   — Arte Braille/ASCII da tela de derrota
├── layout2.h     — Interface de layout2.cpp
├── cores.h       — Macros de escape ANSI para cores e estilos
├── config.txt    — Configuração da partida (jogadores, dados, tempo)
└── README.md     — Este arquivo
```

---

## Requisitos

- Windows 10 ou superior (necessário para suporte a VT100/ANSI no console).
- Terminal com fonte que suporte **Unicode** e **caracteres Braille**
  (recomendado: Windows Terminal com fonte Cascadia Code ou similar).
- Compilador C++17: MinGW-w64 (`g++`) ou MSVC.