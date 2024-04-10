#include "Program.h"

#define MAX_MARBLES 20

int Player5(SaveGame, int *, int, bool);

int *Marble(SaveGame save, int *party, int *tot_players, int count_players) {

    int count = 0, index, check_winner, id, marble_1 = 10, marble_2 = 10, move_1, move_2, bet;
    int *pt_marble = NULL;
    bool condition = true, flag;

    for (int i = 0; i < N_2PARTY; ++i) { //con questo algoritmo vedo se ci sono utenti in partita
        for (int j = 0; j < save.n_users; ++j) {
            if (party[i] == save.profile[save.index_user[j]].id) {
                count++;
                index = i;
            }
        }
    }

    if (count > 0) {
        printf("Benvenuti al gioco delle biglie, ogni giocatore inizia con 10 biglie in mano\n"
               "scommettete le biglie e indovinate le biglie scommesse dall'avversario\n"
               "per vincerle, il primo che riesce ad ottenere tutte le 20 biglie vince\n"
               "Buona fortuna!!\n");
    }

    switch (count) {
        case 0:// nessun utente
            check_winner = rand() % N_2PARTY;
            break;
        case 1:// utente contro cpu

            if (index == 1) {
                id = party[index];
                party[1] = party[0];
                party[0] = id;
            }

            pt_marble = &marble_1;

            //ciclo 'do while' termina quando uno dei giocatori arriva a 20 biglie
            do {
                for (int i = 0; i < N_2PARTY; ++i) {

                    flag = false;
                    move_1 = Player5(save, pt_marble, party[0], flag); //permette all'utente di inserire la sua scommessa
                    move_2 = 1 + rand() % marble_2; //scommessa cpu

                    //in base all'indice cambia il giocatore che deve indovinare le biglie dell'avversario
                    if (i == 0) {
                        flag = true;
                        bet = Player5(save, pt_marble, party[0], flag);
                        //valuta la scommessa ed assegna al giocatore che ha vinto il numero di biglie dell'avversario
                        if (move_2 % 2 == bet) {
                            marble_1 += move_2; //player
                            marble_2 -= move_2; //cpu
                        } else {
                            marble_1 -= move_1; //player
                            marble_2 += move_1; //cpu
                        }
                    } else {
                        bet = rand() % N_2PARTY;
                        if (move_1 % 2 == bet) {
                            marble_1 += move_2; //player
                            marble_2 -= move_2; //cpu
                        } else {
                            marble_1 -= move_1; //player
                            marble_2 += move_1; //cpu
                        }
                    }

                    //in caso uno dei 2 giocatori raggiunge 20 biglie esce dal ciclo
                    if(marble_1 == MAX_MARBLES || marble_2 == MAX_MARBLES ){
                        i = N_2PARTY;
                    }
                    printf("Biglie giocatore 1: %d\nBiglie giocatore 2: %d\n", marble_1, marble_2);
                }


            } while (marble_2 < MAX_MARBLES && marble_1 < MAX_MARBLES);

            //valuta il vincitore della partita
            if (marble_1 == MAX_MARBLES) {
                check_winner = 0;
            } else {
                check_winner = 1;
            }
            break;
        case 2: //utente contro utente

            //ciclo 'do while' termina quando uno dei giocatori arriva a 20 biglie
            do {
                for (int i = 0; i < N_2PARTY; ++i) {

                    flag = false; //booleano che mando alla funzione 'Player5', permette di far indovinare le biglie dell'avversario all'utente corrente
                    pt_marble = &marble_1;// tramite un puntatore che passo alla funzione 'Player5' tengo conto dell biglie del giocatore
                    move_1 = Player5(save, pt_marble, party[0], flag);// permette la scommessa dll'utente 1
                    pt_marble = &marble_2;
                    move_2 = Player5(save, pt_marble, party[1], flag);// permette la scommessa dll'utente 2

                    //in base all'indice cambia il giocatore che deve indovinare le biglie dell'avversario
                    if(i == 0){
                        flag = true;
                        bet = Player5(save, pt_marble, party[0], flag);
                        //valuta la scommessa ed assegna al giocatore che ha vinto il numero di biglie dell'avversario
                        if (move_2 % 2 == bet) {
                            marble_1 += move_2;
                            marble_2 -= move_2;
                        } else {
                            marble_1 -= move_1;
                            marble_2 += move_1;
                        }
                    }else{
                        flag = true;
                        bet = Player5(save, pt_marble, party[1], flag);
                        if (move_1 % 2 == bet) {
                            marble_1 += move_2;
                            marble_2 -= move_2;
                        } else {
                            marble_1 -= move_1;
                            marble_2 += move_1;
                        }
                    }
                    //in caso uno dei 2 giocatori raggiunge 20 biglie esce dal ciclo
                    if(marble_1 == MAX_MARBLES || marble_2 == MAX_MARBLES ){
                        i = N_2PARTY;
                    }
                    printf("Biglie giocatore 1: %d\nBiglie giocatore 2: %d\n", marble_1, marble_2);
                }


            } while (marble_2 < MAX_MARBLES && marble_1 < MAX_MARBLES);

            //valuta il vincitore della partita
            if (marble_1 == MAX_MARBLES) {
                check_winner = 0;
            } else {
                check_winner = 1;
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

int Player5(SaveGame save, int *pt_marbles, int id, bool flag) {

    int move;

    if(flag == false){// in caso il booleano sia false i giocatori devono scommettere le loro biglie
        for (int i = 0; i < save.n_users; ++i) {
            if (id == save.profile[save.index_user[i]].id) {
                printf("%s: scommetti le tue biglie\n", save.profile[save.index_user[i]].name);
            }
        }

        do {
            scanf("%d", &move);
            if (move < 1 || move > *pt_marbles) {
                printf("Non hai abbastanza biglie in mano, scommetti di nuovo\n");
            }
        } while (move < 1 || move > *pt_marbles);
    }

    if(flag){// in caso il booleano sia true il giocatore corrente deve indovinare se le biglie dell'avversario sono pari o dispari
        for (int i = 0; i < save.n_users; ++i) {
            if (id == save.profile[save.index_user[i]].id) {
                printf("%s: indovina se le biglie scommesse dall'avversario "
                       "sono pari o dispari:\n0 = Pari\n1 = Dispari\n", save.profile[save.index_user[i]].name);
            }
        }
        do {
            scanf("%d", &move);
            if (move < 0 || move > 1) {
                printf("Opzione non disponibile, riprova\n");
            }
        } while (move < 0 || move > 1);
    }

    return move;
}