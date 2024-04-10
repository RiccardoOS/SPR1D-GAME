#include "Program.h"

int Player4(SaveGame, int);

typedef enum {CARTA = 1, SASSO, FORBICE} ScF;

int* Morra_Cinese(SaveGame save, int* party, int* tot_players, int count_players){

    int count = 0, index, check_winner, move1 = 0, move2 = 0, id;
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
        printf("Benvenuti alla morra cinese, (carta, sasso, forbice) chi riesce a battere l'avversario vince (cit. mr.ovvio)\n"
               "Buona fortuna!!\n");
    }

    switch (count) {
        case 0:// nessun utente
            check_winner = rand()%N_2PARTY;
            break;
        case 1:// utente contro cpu

            if(index == 1){
                id = party[index];
                party[1] = party[0];
                party[0] = id;
            }

            //ciclo che termina quando le mosse dei giocatori sono differenti
            while (move1 == move2){
                move1 = Player4(save,party[0]);// fa inserire la mossa all'utente
                move2 = 1 + rand()%FORBICE;// la cpu fa una mossa casuale

                if(move1 == move2){//stampa in caso di pareggio
                    printf("Pareggio!!\n");
                }
            }
            break;
        case 2:// utente contro utente

            //ciclo che termina quando le mosse dei giocatori sono differenti
            while (move1 == move2){
                move1 = Player4(save,party[0]);//utente 1 inserisce la sua mossa
                move2 = Player4(save,party[1]);//utente 2 inserisce la sua mossa

                if(move1 == move2){//stampa in caso di pareggio
                    printf("Pareggio!!\n");
                }
            }
            break;
    }

    if(count > 0){
        //controlla il vincitore della partita in base alla mossa dell'utente 1
        switch (move1) {
            case CARTA: //in caso di carta l'utente 1 perde se l'avversario ha forbice, altrimenti vince
                if(move2 == FORBICE){
                    check_winner = 1;
                }else{
                    check_winner = 0;
                }
                break;
            case SASSO: //in caso di sasso l'utente 1 perde se l'avversario ha carta, altrimenti vince
                if(move2 == CARTA){
                    check_winner = 1;
                }else{
                    check_winner = 0;
                }
                break;
            case FORBICE: //in caso di forbice l'utente 1 perde se l'avversario ha sasso, altrimenti vince
                if(move2 == SASSO){
                    check_winner = 1;
                }else{
                    check_winner = 0;
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

int Player4(SaveGame save, int id){

    int move;

    for (int i = 0; i < save.n_profile; ++i) {
        if(id == save.profile[i].id){
            printf("%s: Scegli carta, sasso o forbice\n1 = Carta\n2 = Sasso\n3 = Forbice\n",save.profile[i].name);
        }
    }

    do {
        scanf("%d", &move);
        if(move < CARTA || move > FORBICE){
            printf("Mossa non disponibile, reinserisci\n");
        }
    } while (move < CARTA || move > FORBICE);


    return move;
}