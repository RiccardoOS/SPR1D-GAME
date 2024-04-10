#include "Program.h"

SaveGame New_Game(SaveGame save) {

    int *tot_players = NULL, *ptr_player_save = NULL;
    int player_save;
    bool flag;

    ptr_player_save = &player_save;

    save.game_stat = 1;

    /*chiedo e faccio inserire il numero di partecipanti dello SPR1D GAME*/
    printf("Quanti giocatori vuoi far partecipare? (il numero minimo di giocatori e' 16)\n");

    do {
        scanf("%d", &save.ntot_player);
        if (save.ntot_player < MIN_PLAYER) {
            printf("Numero di giocatori insufficiente. Reinserisci\n");
        }
    } while (save.ntot_player < MIN_PLAYER);

    if (save.n_profile != 0) {
        save = User_Choice(save); //chiamo la funzione solo in caso esistano profili giocatore
    }


    /*crea l'array di giocatori totali*/
    if ((tot_players = (int *) calloc(save.ntot_player, sizeof(int))) == NULL) {
        printf("Errore: allocazione fallita\n");
        exit(EXIT_FAILURE);
    }

    if ((save.player_state = (int*) calloc(save.ntot_player, sizeof(int))) == NULL) {
        printf("Errore: allocazione fallita\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < save.ntot_player; ++i) {
        save.player_state[i] = 1;
    }

    //Assegno casualmente gli id all'interno dell'array
    for (int i = 0; i < save.ntot_player; ++i) {
        tot_players[i] = rand() % save.ntot_player;
        for (int j = i; j >= 0; --j) {//Questo ciclo permette di non creare id uguali
            if (tot_players[i] == tot_players[j]) {
                tot_players[i] = rand() % save.ntot_player;
                j = i;
            }
        }
    }

    tot_players = Skimming(save, tot_players); //primo gioco obbligatorio scrematura

    /*per ogni giocatore eliminato lo stato diventa false*/
    for (int i = 0; i < save.ntot_player; ++i) {
        if (tot_players[i] == -1) {
            save.player_state[i] = 0;
        }
    }

    /*rialloco l'array di giocatori totali  che in questo momento contiene gli id vincitori e tutti gli eliminati -1,
     * per avere solamente i giocatori rimanenti*/
    tot_players = Realloc_Player(tot_players, ptr_player_save, save.ntot_player);

    flag = true;
    /*doppio ciclo che assegna ai profili ancora in gioco i giochi vinti in totale,
     * in caso un giocatore sia stato eliminato modifica l'id con -1*/
    for (int i = 0; i < save.n_profile; ++i) {
        for (int j = 0; j < player_save; ++j) {
            //la condizione si basa sull'array dei giocatori totali che viene modificato ogni fine partita
            if(tot_players[j] == save.profile[i].id){
                save.profile[i].totgame_win++;
                flag = false;
            }
        }
        if(flag){
            save.profile[i].id = -1; //assegna -1 all'id del profilo giocatore che e' stato eliminato
        }
        flag = true;
    }

    //aggiorna gli utenti rimasti
    if (save.n_users > 0){
        save = Update_User(save);
    }

    free(tot_players);

    return save;
}