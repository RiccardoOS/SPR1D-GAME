#include "Program.h"

#define HAND 5

int Player1(SaveGame,int);

typedef enum {PARI,DISPARI} OoE;

int* Odd_or_Even (SaveGame save, int* party, int* tot_players, int count_players){

    int count = 0, player_move, sum = 0, move, check_winner, index, id;
    bool condition = true;

    for (int i = 0; i < N_2PARTY; ++i) { //con questo algoritmo vedo se ci sono utenti in partita
        for (int j = 0; j < save.n_users; ++j) {
            if(party[i] == save.profile[save.index_user[j]].id){
                count++;
                index = i;
            }
        }
    }

    if(count > 0){
        printf("Benvenuti al gioco pari o dispari,\nchi tra i giocatori indovinera' se la somma tra i numeri giocati"
               " dai concorrenti stesso e' pari o dispari vincera'.\nBuona fortuna!\n");
    }

    switch (count) {// in base al numero di utenti presenti in partita il gioco si svolge differente
        case 0:// nessun utente
            check_winner = rand()%N_2PARTY;// crea un vincitore casuale se nessun utente e' in partita
            break;
        case 1:// utente contro cpu
            if(index == 1){// se c'e' solo un utente inizia sempre per primo
                id = party[index];
                party[1] = party[0];
                party[0] = id;
            }

            player_move = Player1(save, party[0]);// richiama la funzione per far giocare l'utente

            for (int i = 0; i < save.n_profile; ++i) {// fa inserire all'utente la sua mossa
                if(party[0] == save.profile[i].id){
                    printf("%s: Inserisci un numero tra 0 e 5\n",save.profile[i].name);
                    do {
                        scanf("%d", &move);
                        if(move < 0 || move > HAND){
                            printf("Reinserisci\n");
                        }
                    } while (move < 0 || move > HAND);
                }
            }

            sum = move;
            sum += rand()%(HAND+1);// la cpu genera un mossa casuale

            break;
        case 2:// utente contro utente
            player_move = Player1 (save, party[0]);// 1 giocatore sceglie pari o dispari, il secondo prende automaticamente l'inverso

            for (int i = 0; i < N_2PARTY; ++i) {// ogni utente inserisce la sua mossa
                for (int j = 0; j < save.n_profile; ++j) {
                    if(party[i] == save.profile[j].id){
                        printf("%s: Inserisci un numero tra 0 e 5\n",save.profile[j].name);
                        do {
                            scanf("%d", &move);
                            if(move < 0 || move > HAND){
                                printf("Reinserisci\n");
                            }
                        } while (move < 0 || move > HAND);
                    }
                }
                sum += move;// vengono sommate le mosse degli utenti
            }
            break;
    }



    if(count > 0){// controllo vittoria
        switch (player_move) {
            case PARI:
                if(sum%2 == 0){// se l'utente ha scelto pari e la somma e' pari vince, altrimenti vince l'avversario
                    check_winner = 0;
                }else{
                    check_winner = 1;
                }
                break;
            case DISPARI:
                if(sum%2 != 0){// se l'utente ha scelto dispari e la somma e' dispari vince, altrimenti vince l'avversario
                    check_winner = 0;
                }else{
                    check_winner = 1;
                }
                break;
        }
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

int Player1 (SaveGame save, int id){

    int choice;

    //l'utente sceglie pari o dispari
    for (int i = 0; i < save.n_profile; ++i) {
        if(id == save.profile[i].id){
            printf("\n%s: Scegli pari o dispari\n0 = Pari\n1 = Dispari\n",save.profile[i].name);
        }
    }
    do {
        scanf("%d", &choice);
        if(choice < 0 || choice > 1){
            printf("Reinserisci\n");
        }
    } while (choice < 0 || choice > 1);

    return choice;
}
