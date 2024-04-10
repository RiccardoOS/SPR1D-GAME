#include "Program.h"

#define DICE 6

int Player2 (SaveGame, int);

int* Dice_Game(SaveGame save, int* party, int* tot_players, int count_players){

    int count = 0, index, coin, dice0 = 0, dice1 = 0, prev_dice0 = 0, prev_dice1 = 0, check_winner;
    int cpu;
    bool condition = true;

    for (int i = 0; i < N_2PARTY; ++i) {//con questo algoritmo vedo se ci sono utenti in partita
        for (int j = 0; j < save.n_users; ++j) {
            if(party[i] == save.profile[save.index_user[j]].id){
                count++;
                index = i;// salvo l'indice dell'utente
            }else{
                cpu = i;// salvo l'indice della cpu
            }
        }
    }

    if(count > 0){
        printf("Benvenuti al gioco dei dadi,\nogni giocatore tirera' due dadi, il primo che fa un tiro la cui somma"
               " e' minore al tiro precedente perde.\nBuona fortuna!\n");
    }

    coin = rand()%N_2PARTY;//genera il primo giocatore casualmente

    switch (count) {
        case 0:// nessun utente
            check_winner = rand()%N_2PARTY;
            break;
        case 1:// utente contro cpu
        //ciclo che termina quando uno dei due giocatori fa un tiro la cui somma e' minore a quella precedente
            while (prev_dice0 <= dice0 && prev_dice1 <= dice1){
                //salvo le somme in una variabile d'appoggio che tiene conto del tiro precedente
                prev_dice1 = dice1;
                prev_dice0 = dice0;

                if(coin == index){// in base all'indice scambio chi deve giocare
                    dice0 = Player2(save, party[coin]);// permette la giocata dell'utente
                    printf("Somma: %d\n",dice0);
                    check_winner = cpu;
                    coin = cpu;

                }else{
                    dice1 = 1 + rand()%(DICE + DICE);
                    check_winner = index;
                    coin = index;
                }
            }
            break;
        case 2:// utente contro utente
            while (prev_dice0 <= dice0 && prev_dice1 <= dice1){
                //salvo le somme in una variabile d'appoggio che tiene conto del tiro precedente
                prev_dice1 = dice1;
                prev_dice0 = dice0;

                //scambio il turno dei giocatori
                if(coin == 0){
                    dice0 = Player2(save, party[coin]);
                    printf("Somma: %d\n",dice0);
                    coin = 1;
                    check_winner = 1;
                }else{
                    dice1 = Player2(save, party[coin]);
                    printf("Somma: %d\n",dice1);
                    coin = 0;
                    check_winner = 0;
                }
            }
            break;
    }

    // valuta se Riccardo Scateni e' in partita, se si vince sempre
    check_winner = Check_Frontman (save, party, N_2PARTY, check_winner);

    if(count > 0){

        printf("_____FINE PARTITA_____\n");

        for (int i = 0; i < save.n_profile; ++i) {//se il vincitore e' un profilo stampa il nome e l'id
            if(party[check_winner] == save.profile[i].id){
                printf("Il vincitore e': %s ID: %d\n\n",save.profile[i].name, party[check_winner]);
                condition = false;
            }
        }
        //altrimenti stampa solo l'id
        if(condition){
            printf("Il vincitore e': ID: %d\n\n",party[check_winner]);
        }
    }


    //aggiorna l'array totale giocatori assegnando come id -1 a chi e' stato eliminato
    for (int i = 0; i < N_2PARTY; ++i) {
        for (int j = 0; j < count_players; ++j) {
            if (party[check_winner] != tot_players[j] && party[i] == tot_players[j]){
                tot_players[j] = - 1;
            }
        }
    }

    return tot_players;
}

int Player2 (SaveGame save, int id){

    int dice = 0;
    for (int i = 0; i < save.n_profile; ++i) {
        if(id == save.profile[i].id){
            printf("/----------------------------------/\n");
            printf("%s premi invio per tirare i dadi\n",save.profile[i].name);
            getchar();
        }
    }

    for (int i = 0; i < N_2PARTY; ++i) {
        dice += 1 + rand()%DICE;// lancia il dado 2 volte facendo la somma
    }

    return dice;
}
