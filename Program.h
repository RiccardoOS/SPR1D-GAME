#ifndef PR0G3770_F1N4L3_PROGRAM_H
#define PR0G3770_F1N4L3_PROGRAM_H

#endif //PR0G3770_F1N4L3_PROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define MIN_PLAYER 16 //numero minimo di giocatori per il gioco completo
#define BASE_POWER 2 //utile a calcolare la potenza di 2 dei giocatori che rimangono dopo la scrematura
#define CHAR_DIST 32 //distanza tra le lettere maiuscole e minuscole
#define N_GAME 8 //numero di giochi che possono capitare casualmente durante la partita
#define N_2PARTY 2 //numero di giocatori per gruppo nei giochi che richedono 2 giocatori
#define N_4PARTY 4 //numero di giocatori per gruppo nei giochi che richedono 4 giocatori
#define MAX_FRASE 120

typedef struct{
    int id;
    char name[64];
    int spr1d_game_win;
    int spr1d_game_played;
    int played_final;
    int totplayed_game;
    int totgame_win;
}PlayerProfile;

/*struttura utile a tener conto di tutti i dati all'interno del programma*/
typedef struct{
    int n_profile;
    PlayerProfile *profile;
    int game_stat;
    int ntot_player;
    int n_users;
    int* index_user;
    int* player_state;
}SaveGame;

//Funzioni che hanno un proprio file.c
PlayerProfile* Create_Player (int, PlayerProfile*); //crea il profilo giocatore
void Save_File_Name (char*); //salva il nome dei file creati in un file testo
char** Load_File_Name (char**, int*); //carica il file testo contenente i nomi dei file salvati
SaveGame Load_Game (); //carica un file binario richiesto dall'utente
void Save_Game (SaveGame); //salva la partita o i profili giocatore
SaveGame New_Game (SaveGame); //funzione che viene richiamata dal menu,permette di iniziare una nuova partita
SaveGame User_Choice (SaveGame); //in caso di nuova partita permette la scelta di utilizzare uno o piu' profili
int* Skimming (SaveGame, int*); //scrematura contenente anche il gioco 'Guess the Number'
SaveGame Continue (SaveGame); //funzione che viene richiamata dal menu, permette di continuare la partita iniziata

//Funzioni utili (presenti nel file.c 'Useful_Function')
//sono definiti nell'ordine in cui sono dichiarati successivamente
void Modify_Name (char*);
int* Realloc_Player(int*, int*, int);
SaveGame Update_User(SaveGame);
void Print_Profile (SaveGame);
int Check_Frontman (SaveGame, int*, int, int);
SaveGame Check_Profile(SaveGame, int);
int* Create_2Party (SaveGame, int*, int, int );
int* Create_4Party (SaveGame, int*, int, int);
char** Dictionary (char**, int*);

//GIOCHI
int* Guess_the_Number (int*, SaveGame, int, int*); //all'interno del file 'Skimming'
int* Odd_or_Even (SaveGame, int*, int*, int);// pari o dispari
int* Dice_Game(SaveGame, int*, int*, int);// gioco dei dadi
int* Murra (SaveGame, int*, int*, int);
int* Morra_Cinese (SaveGame, int*, int*, int);// sasso, carta, forbice
int* Tic_Tac_Toe (SaveGame, int*, int*, int);// tris
int* Marble (SaveGame, int*, int*, int);// gioco delle biglie
int* Ponte_di_Vetro (SaveGame, int*, int*, int);
int* Impiccato (SaveGame, int*, int*, int);
SaveGame BlackJack (SaveGame, int*);


