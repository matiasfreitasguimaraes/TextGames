#include <iostream>
#include <string>
#include <ctype.h>
#include <sstream>
#include <unistd.h>
#include <string_view>

using namespace std;
// Constantes definidas pro programa. Majoritariamente mensagens. Algumas strings para trabalhar em cima. E uma constante de casas no jogo.
#define RED "\033[0;31m"
#define WHITE "\033[1;37m"
#define openMessage "Ola, voces gostariam de jogar Oware? 1 para sim, 0 para N"
#define repeatOpenMessage "Nao consegui entender sua resposta. Sim ou nao? 1 para sim, 0 para N"
#define repeatOpenMessage2 "Nao consegui entender sua resposta."
#define gameAgainMessage "Gostariam de jogar de novo? 1 para sim, 0 para N"
#define player1AskToGame "Jogador 1, qual casa você quer mover? Tu pode escolher as casas da a a f"
#define player2AskToGame "E ti,jogador 2, qual casa você vai mover? Tu pode escolher as casas da A a F"
#define confirmAction "Certeza? 1 para sim, 0 para N"
#define scoreMessage "Jogador, seu score é"
#define houseDrawn0and11  " __________"
#define houseDrawnL1_10 "|   "
#define houseDrawnR1_10 "   |"
#define NHouses 12
#define winPlayer1 "Jogador 1, parabéns pela vitória!"
#define winPlayer2 "Jogador 2! Você ganhou o jogo."
#define drawn "Empate! Bem jogado por ambos"
string housesNames = "abcdefABCDEF";
string houseP1 = "abcdef";
string houseP2 = "ABCDEF";

// Função para trocar a cor do terminal. Pega do professor.
void setcolor(string color){
    cout << color;
}
// Função para onde se imprime os valores no terminal. Pega do professor.
void gotoxy(int x, int y){
    ostringstream oss;
    oss << "\033["<< y << ";"<< x << "H";
    cout << oss.str();
}
// Função para limpar o terminal. Pega do professor.
void clrscr(void){
    cout << "\033[2J";
    gotoxy(0, 0);
}
// Função para imprimir 1 casa do tabuleiro de Oware.
void HousePrint(int pieces, char house, int x, int y){
    printf("      %c\n", house);
    gotoxy(x,y+2);
    printf("%s\n", houseDrawn0and11);
    for (int i = 3; i < 8; ++i) {
        gotoxy(x,y+i);
        printf("%s", houseDrawnL1_10);
        if(i == 5){
            // Um if pra lidar com o problema de printa numeros com mais de 1 digito.
            if(pieces>9){
                printf(" %i ", pieces);
            }
            else{
                printf("  %i ", pieces);
            }
        }else{
            printf("    ");
        }
        printf("%s\n", houseDrawnR1_10);
    }
    gotoxy(x,y+8);
    printf("%s\n\n",houseDrawn0and11 );
}
// Função para imprimir o tabuleiro inteiro com duas cores. Aproveito da troca de localização da função gotoxy para imprimir a função HousePrint em 12 lugares diferentes 
void ShowBoard (int housesPieces[NHouses]) {
    int x = 60;
    int y = 0;
    int dir = -1;
    for (int i = 0; i < NHouses; ++i) { //altera o print da casa
        gotoxy(x,y);
        HousePrint(housesPieces[i], housesNames[i], x, y);
        if (i == 5) { //troca de linha e inverta a ordem que as casas vão sendo printadas pra lidar com um tabuleiro circular.
            setcolor(RED);
            y = y + 10;
            dir = 1;
        } else{
           x = x + 12*dir;
        }
    }
    setcolor(WHITE);
}
// Função para pedir e tratar o comando do jogador.
int askMoveHouse(int jogador){
    int confirm  = 0;
    int charconfirm = 0;
    char houseChar;
    while (charconfirm == 0 || confirm == 0 ){ // um ciclo while que recomeça sempre se o input não tver correto. 
        if (jogador == 0){  // Busca e coloca o index da casa do jogador. Fiz dessa forma, com um if e else do jogador 1 e 2, pois consegui assim validar e coloca o index ao mesmo tempo em 1 processo. 
            printf("%s\n", player1AskToGame);
            cin >> houseChar;
            houseChar = houseP1.find(houseChar);
        } else{
            printf("%s\n", player2AskToGame);
            cin >> houseChar;
            houseChar = houseP2.find(houseChar);
        }    
        printf("%s\n", confirmAction);
        cin >> confirm;  
        if (houseChar != -1){
            charconfirm = 1;
        } else{
            printf("%s\n", repeatOpenMessage2);
        }
    }  
    if (jogador == 0){
        return houseChar;
    } else{
        return 6+houseChar;
    }
}
// Função que trata das mecanicas do jogo em si. Cria um array para as peças e coloca a lógica dos movimentos e regras permitdas. Cria as mecanicas do jogo e atualiza os scores. Também analiza um empate todo ciclo nas ultimas linhas
void Play() {
    clrscr();
    int housesPieces[NHouses] = {4,4,4,4,4,4,4,4,4,4,4,4};
    int score[2] = {0,0};
    int player = 0;
    int index;
    int nHouse;
    while (true){
        ShowBoard(housesPieces); //funçáo de mostrar o tabuleiro
        printf("%s ", scoreMessage); //mostra o score no começo da jogada
        printf("%i\n", score[player]);
        index = askMoveHouse(player); // função de perguntar a jogada
        for (int i = 0; i < housesPieces[index]; i++){ //move as peças da casa escolhida
            nHouse = (index+i+1)%NHouses; // Torna o tabuleiro ciclico
            while (housesPieces[nHouse] > 11){
                nHouse++;
            }
            housesPieces[nHouse]++;
        }
        housesPieces[index] = 0;
        while (housesPieces[nHouse]>1 && housesPieces[nHouse]<4){ // atualiza o score e limpa as peças capturadas
            score[player] = score[player] + housesPieces[nHouse];
            housesPieces[nHouse] = 0;
            nHouse--;
            if(nHouse<0){ // Torna o tabuleiro ciclico
                nHouse = nHouse+12;
            }
        }
        if (score[0] == 24 && score[0] == score[1]){ // analiza um empate
            printf("%s\n", drawn);
            break;
        } else if (score[0]>24){ // Uma vitoria do player 1
            printf("%s\n", winPlayer1);
            break;
        } else if (score[1]>24){ // uma vitoria do player 2
            printf("%s\n", winPlayer2);
            break;
        }
        player = player*-1+1;
        clrscr();
    }
}
// Função main que pergunta se o jogador quer jogar de novo ou não e chama a função play se quiser, e fecha o executavel se não quiser.
int main() {
    int beginVar;
    printf("%s\n",openMessage);
    cin >> beginVar;
    while (true){
        if (beginVar == 1){
            clrscr();
            Play();
            printf("\n\n%s\n",gameAgainMessage);
            cin >> beginVar;
        } else if (beginVar == 0){
            break;
        } else{
            printf("%s\n",repeatOpenMessage);
            cin >> beginVar;
        }
    }
}