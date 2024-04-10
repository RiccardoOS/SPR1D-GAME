#include "Program.h"

#define ROW 3
#define COL 2

typedef enum {FRAGILE, RESISTENTE} Res;

int Game(SaveGame, int*, int bridge [][COL], int);



int* Ponte_di_Vetro (SaveGame save, int* party, int* tot_players, int count_players){

    int count = 0, check_winner = -1;
    int bridge[ROW][COL];
    bool condition = true;

    for (int i = 0; i < N_4PARTY; ++i) {// valuta quanti utenti ci sono in partita
        for (int j = 0; j < save.n_users; ++j) {
            if(party[i] == save.profile[save.index_user[j]].id){
                count++;
            }
        }
    }

    if (count > 0){
        printf("Benvenuti al gioco del ponte di vetro, ciascun giocatore dovra' riuscire ad attraversare il ponte\n"
               "saltando sul vetro resistente, il primo che ci riesce ha vinto, oppure l'ultimo"
               " sara' il piu' fortunato.\nBuona fortuna!!\n");
    }

    for (int i = 0; i < ROW; ++i) {// genera il ponte di vetro se il vetro e' 0 allora e' fragile, se e' 1 e' resistente
        bridge[i][0] = rand()%2;
        if (bridge[i][0] == FRAGILE){
            bridge[i][1] = RESISTENTE;
        }else{
            bridge[i][1] = FRAGILE;
        }
    }

    if(count == 0){// se non ci sono utenti genera un vincitore casuale
        check_winner = rand()%N_4PARTY;
    }else{
        for (int i = 0; i < N_4PARTY; ++i) {// i giocatori giocano a turno uno dopo l'altro
            check_winner = Game (save, party, bridge, i);// richiama la funzione 'Game' che restituisce un vincitore
            if(check_winner == i){
                i = N_4PARTY;
            }
        }
        if(check_winner == -1){// se tutti i giocatori sono stati eliminati il vincitore e' automaticamente l'ultimo
            check_winner = 3;
        }

    }



    // valuta se Riccardo Scateni e' in partita, se si vince sempre
    check_winner = Check_Frontman (save, party, N_4PARTY, check_winner);

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
    for (int i = 0; i < N_4PARTY; ++i) {
        for (int j = 0; j < count_players; ++j) {
            if (party[check_winner] != tot_players[j] && party[i] == tot_players[j]){
                tot_players[j] = - 1;
            }
        }
    }

    return tot_players;
}

int Game(SaveGame save, int* party, int bridge[ROW][COL], int player){

    int check_winner = -1, move, index;

    bool stat = true, condition;

    for (int i = 0; i < save.n_users; ++i) {
        if(party[player] == save.profile[save.index_user[i]].id){
            condition = true; // se il giocatore e' un utente permette di farlo giocare, altrimenti gioca la cpu
            index = save.index_user[i];
            i = save.n_users;

        }else{
            condition = false;
        }
    }

    if(condition){// se condition e' true gioca l'utente

        //il booleano stat permette di continuare o uscire dal ciclo in caso il giocatore venga eliminato
        for (int i = 0; i < ROW && stat == true; ++i) {// fa inserire all'utente dove passare finquando oltrepassa il ponte o viene eliminato
            printf("%s: Inserisci se vuoi passare sul vetro a destra oppure a sinistra\n"
                   "0 = destra\n1 = sinistra\n",save.profile[index].name);

            do {
                scanf("%d",&move);
                if (move < 0 || move > 1){
                    printf("Reinserisci\n");
                }
            } while (move < 0 || move > 1);

            switch (move) {// in base alla mossa del giocatore valuta se dove passa e' fragile o resistente
                case 0:
                    if(bridge[i][1] == RESISTENTE){
                        printf("Sei salvo\n");
                        stat = true;
                    }else{
                        printf("Sei morto (cit. Dark Souls)\n");
                        stat = false;
                    }
                    break;
                case 1:
                    if(bridge[i][0] == RESISTENTE){
                        printf("Sei salvo\n");
                        stat = true;
                    }else{
                        printf("Sei morto (cit. Dark Souls)\n");
                        stat = false;
                    }
                    break;
            }

        }

    }else{// se condition e' false gioca la cpu

        for (int i = 0; i < ROW && stat == true ; ++i) {
            move = rand()%COL;

            //la giocata della cpu permette all'utente di sapere dove e' il vetro resistente
            switch (move) {
                case 0:
                    if(bridge[i][1] == RESISTENTE){
                        printf("Vetro resistente in fila %d: destra\n", i + 1);
                        stat = true;
                    }else{
                        printf("Vetro resistente in fila %d: sinistra\n", i + 1);
                        stat = false;
                    }
                    break;
                case 1:
                    if(bridge[i][0] == RESISTENTE){
                        printf("Vetro resistente in fila %d: sinistra\n", i + 1);
                        stat = true;
                    }else{
                        printf("Vetro resistente in fila %d: destra\n", i + 1);
                        stat = false;

                    }
                    break;
            }

        }
    }

    //se stat rimane true il giocatore ha vinto la partita
    if (stat){
        check_winner = player;
    }


    return check_winner;
}
