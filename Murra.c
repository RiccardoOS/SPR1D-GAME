#include "Program.h"

#define MAX_MOVE 5
#define MIN_SUM 2
#define MAX_SUM 10

int Player3 (SaveGame,int);

int* Murra (SaveGame save, int* party, int* tot_players, int count_players){

    int count = 0, sum0 = 1, sum1 = 1, move = 0, check_winner, index, id;
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
        printf("Benvenuti al gioco della murra, i giocatori si scontreranno a colpi di dita,"
               "i due giocatori mostrano le dita della mano e dovranno indovinare la somma,\nBuona fortuna!\n");
    }

    switch (count) {
        case 0:// nessun giocatore
            check_winner = rand()%N_2PARTY;
            break;
        case 1:// utente contro cpu
            if(index == 1){
                id = party[index];
                party[1] = party[0];
                party[0] = id;
            }

            //ciclo che termina quando uno dei giocatori indovina la somma
            while (sum0 != move && sum1 != move){

                move = Player3(save, party[0]);// l'utente inserisce la sua mossa

                printf("Indovina la somma tra il tuo numero e quello del tuo avversario\n");

                //l'utente scommette la somma dei due giocatori
                do {
                    scanf("%d", &sum0);
                    if(sum0 < MIN_SUM || sum0 > MAX_SUM){
                        printf("Reinserisci\n");
                    }
                }while(sum0 < MIN_SUM || sum0 > MAX_SUM);

                move += 1 + rand()%MAX_MOVE;// somma la mossa dell'utente con quella della cpu
                sum1 = MIN_SUM + rand()%(MAX_SUM - MIN_SUM + 1); // la cpu genera la sua scommessa
            }
            break;
        case 2:// utente contro utente

            //ciclo che termina quando uno dei giocatori indovina la somma
            while (sum0 != move && sum1 != move){

                move = 0;// rinizializza la mossa dell'utente

                for (int i = 0; i < N_2PARTY; ++i) {

                    move += Player3 (save,party[i]);// permette di inserire la mossa degli utenti e le somma

                    printf("Indovina la somma tra il tuo numero e quello del tuo avversario\n");
                    if(i == 0){// permette di inserire la scommessa degli utenti
                        do {
                            scanf("%d", &sum0);
                            if(sum0 < MIN_SUM || sum0 > MAX_SUM){
                                printf("Reinserisci\n");
                            }
                        }while(sum0 < MIN_SUM || sum0 > MAX_SUM);
                    }else{
                        do {
                            scanf("%d", &sum1);
                            if(sum1 < MIN_SUM || sum1 > MAX_SUM){
                                printf("Reinserisci\n");
                            }
                        }while(sum1 < MIN_SUM || sum1 > MAX_SUM);
                    }

                }

            }
            break;
    }

    //controlla il vincitore della partita
    if(count < 0){
        if(sum0 == move){
            check_winner = 0;
        }else{
            check_winner = 1;
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

int Player3 (SaveGame save, int id){

    int player;

    for (int i = 0; i < save.n_profile; ++i) {
        if (id == save.profile[i].id){
            printf("%s inserisci un numero tra 1 e 5\n",save.profile[i].name);
        }
    }

    do {
        scanf("%d",&player);
        if(player < 1 || player > MAX_MOVE){
            printf("Reinserisci\n");
        }

    } while (player < 1 || player > MAX_MOVE);

    return player;
}