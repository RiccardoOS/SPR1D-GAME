#include "Program.h"

PlayerProfile* Create_Player (int count_profiles, PlayerProfile* profile2){

    bool flag = false;

    /*alloco la memoria all'array di profili, in caso non esiste nessun profilo lo creo, in caso contriario
     * rialloco la memoria*/
    if(count_profiles == 1){
        profile2 = (PlayerProfile*) malloc (count_profiles * sizeof(PlayerProfile));
    }else{
        profile2 = (PlayerProfile*) realloc (profile2, count_profiles * sizeof(PlayerProfile));
    }

    if(profile2 == NULL){
        exit -1;
    }

    printf("Scegli un nome per il personaggio, se hai creato altri personaggi\n"
           "non puoi creare un altro personaggio con lo stesso nome\n");

    do { //permette l'inserimento del nome del personaggio
        gets(profile2[count_profiles - 1].name);
        Modify_Name(profile2[count_profiles - 1].name);

        //in caso il nome esiste gia' lo fa reinserire
        for (int i = 0; i < count_profiles-1; ++i) {
            if (strcmp(profile2[count_profiles - 1].name, profile2[i].name)){
                flag = false;
            }else{
                flag = true;
                i = count_profiles; //in caso del nome gia' esistente esce dal ciclo
            }
        }

        if(flag){
            printf("il nome inserito e' gia' in uso, riprova\n");
        }
    } while (flag); //in caso di nome gia' esistente rinizia il ciclo

    //stampa il nome di tutti i giocatori ogni volta che viene creato 1 giocatore
    for (int i = 0; i < count_profiles; ++i) {
        printf("%s\n", profile2[i].name);
    }

    //assegno valori nulli ai dati di ogni giocatore non essendo ancora in partita
    profile2[count_profiles - 1].totplayed_game = 0;
    profile2[count_profiles - 1].totgame_win = 0;
    profile2[count_profiles - 1].id = -1;
    profile2[count_profiles - 1].spr1d_game_win = 0;
    profile2[count_profiles - 1].spr1d_game_played = 0;
    profile2[count_profiles - 1].played_final = 0;

    return profile2;
}