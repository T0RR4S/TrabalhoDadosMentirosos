#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#define tamanho_maximo_dos_nomes_dos_jogadores 100
using namespace std;
void troca_jogador(int tempo_troca_jogador,int nome_do_jogador[],int vez)
{
    cout<<"JOGADOR "<<vez-1<<"SAIA IMEDIATAMENTE"<<endl;
    for(int i=tempo_de_troca_do_jogador;i>0;i--)
    {    
        cout<<i<<" SEGUNDOS ATÉ A VEZ DO JOGADOR "<<jogador[vez]<<endl;
        this_thread::sleep_for(chrono::seconds(i));
    }
}
main()
{
    ifstream arq("config.txt");
    if(!arq) 
    {
        cout<<"ERRO! ARQUIVO: CONFIG.TXT"<<endl;
        return 1;
    }
    int numero_de_jogadores,numeros_de_dados_por_jogador,tempo_de_troca_do_jogador;
    arq>>numero_de_jogadores>>numero_de_dados_por_jogador>>tempo_de_troca_do_jogador;
    if(numero_de_jogadores<2)
    {
        cout<<"ERRO! NÚMERO DE JOGADORES MÍNIMO: 2."<<endl;
        return 1;
    }
    if(numero_de_dados_por_jogador<1)
    {
        cout << "ERRO! NUMERO DE DADOS POR JOGADOR MÍNIMO: 1" << endl;
        return 1;
    }
    if(tempo_troca_jogador<0)
    {
        cout << "ERRO! TEMPO TROCA JOGADOR MÍNIMO: 0" << endl;
        return 1;
    }
    vector<string>nome_do_jogador(numero_de_jogadores);
    for(int i = 0; i < numero_de_jogadores; i++)
    {
        cout << "JOGADOR " <<i+1<<", INSIRA SEU NOME: ";
        getline(cin,nome_do_jogador[i]);
        if(nome_do_jogador[i].size()>tamanho_maximo_dos_nomes_dos_jogadores)
        {
            cout<<"TAMANHO MAXIMO DOS NOMES DOS JOGADORES: "<<tamanho_maximo_dos_nomes_dos_jogadores<<endl;
            i--;
        }
    }
}