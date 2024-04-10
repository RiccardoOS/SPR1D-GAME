#include "Program.h"

#define FINAL 2

SaveGame Continue (SaveGame save){

    int count = 0, index = -2, rand_game, choice;
    int* pt_count = NULL,*tot_players = NULL;
    bool flag;

    pt_count = &count;//variabile che aggiorno tramite passaggio per parametri

    //conta i giocatori rimasti in partita in base allo stato dei giocatori totali
    for (int i = 0; i < save.ntot_player; ++i) {
        if(save.player_state[i] == 1){
            count++;
        }
    }

    if ((tot_players = (int *) calloc(count, sizeof(int))) == NULL) {
        printf("Errore: allocazione fallita\n");
        exit(EXIT_FAILURE);
    }

    //ogni qualvolta la funzione 'Continue' viene chiamata genera un nuovo array totale giocatori e crea degli id
    for (int i = 0; i < count; ++i) {
        tot_players[i] = rand() % count;
        for (int j = i; j >= 0; --j) {//Questo ciclo permette di non creare id uguali
            if (tot_players[i] == tot_players[j]) {
                tot_players[i] = rand() % count;
                j = i;
            }
        }
    }

    if(save.n_profile > 0){//se esistono profili salvati controlla quali sono ancora in gioco
        save = Check_Profile(save, count);
    }


    index = Check_Frontman(save, tot_players, count, index);//controlla se Riccardo Scateni e' in partita

    while(count > FINAL){
        if(index != -2){// nel caso Riccardo Scateni sia in partita gli e' permesso scegliere il gioco
            printf("Riccardo Scateni ha attivato la modalita' frontman, puo' scegliere il gioco a cui partecipare\n");
            printf("1 = Pari o dispari\n2 = Dadi\n3 = Murra\n4 = Morra cinese\n5 = Tris\n6 = Biglie\n7 = Ponte di vetro\n"
                   "8 = Impiccato\n");
            if(count < N_GAME){ //non permette di inserire i giochi da 4 giocatori se i giocatori totali sono meno di 8
                do {
                    scanf("%d", &rand_game);
                    if(rand_game < 1 || rand_game > N_GAME - 2){
                        printf("Non ci sono abbastanza partecipanti per questo gioco, scegline un altro\n");
                    }
                }while(rand_game < 1 || rand_game > N_GAME - 2);
            }else{//se i giocatori totali sono 8 o piu' possono essere scelti tutti i giochi
                do {
                    scanf("%d", &rand_game);
                    if(rand_game < 1 || rand_game > N_GAME){
                        printf("Questo gioco non esiste, scegline un altro\n");
                    }
                }while(rand_game < 1 || rand_game > N_GAME);
            }

        }else{
            if(count >= N_GAME){
                rand_game = 1 + rand()%8; //genera casualmente uno dei giochi possibili
            }else{
                rand_game = 1 + rand()%6;
            }

        }

        /*switch a cascata che richiama la funzione per creare 2 giocatori o 4 giocatori in base al gioco,
        * i giochi vengono chiamati all'interno dell funzioni 'Create_2/4Party' */
        switch (rand_game) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:

                /*funzione che crea il gruppo di giocatori che parteciperanno alla partita e termina
                 * quando tutti i giocatori presenti hanno partecipato*/
                tot_players = Create_2Party(save, tot_players, count, rand_game);

                //rialloca l'array dei giocatori totali con quelli presenti ancora in partita
                tot_players = Realloc_Player(tot_players, pt_count, count);

                //aggiorna lo stato dei giocatori totali
                for (int i = 0; i < count; ++i) {
                    save.player_state[i] = 1;
                }
                for (int i = count; i < save.ntot_player; ++i) {
                    save.player_state[i] = 0;
                }

                flag = true;
                /*doppio ciclo che assegna ai profili ancora in gioco i giochi vinti in totale,
                 * in caso un giocatore sia stato eliminato modifica l'id con -1*/
                for (int i = 0; i < save.n_profile; ++i) {
                    for (int j = 0; j < count; ++j) {
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

                //l'utente puo' scegliere se tornare al menu' o proseguire la partita
                printf("Vuoi tornare al menu'?\nSeleziona 1 per tornare al menu', altrimenti continua\n");
                scanf("%d", &choice);

                if(choice == 1){
                    free(tot_players);
                    return save;
                }
                break;

            case 7:
            case 8:
                tot_players = Create_4Party(save, tot_players, count, rand_game);

                //rialloca l'array dei giocatori totali con quelli presenti ancora in partita
                tot_players = Realloc_Player(tot_players, pt_count, count);

                //aggiorna lo stato dei giocatori totali
                for (int i = 0; i < count; ++i) {
                    save.player_state[i] = 1;
                }
                for (int i = count; i < save.ntot_player; ++i) {
                    save.player_state[i] = 0;
                }

                flag = true;
                /*doppio ciclo che assegna ai profili ancora in gioco i giochi vinti in totale,
                 * in caso un giocatore sia stato eliminato modifica l'id con -1*/
                for (int i = 0; i < save.n_profile; ++i) {
                    for (int j = 0; j < count; ++j) {
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

                //l'utente puo' scegliere se tornare al menu' o proseguire la partita
                printf("Vuoi tornare al menu'?\nSeleziona 1 per tornare al menu', altrimenti continua\n");
                scanf("%d", &choice);

                if(choice == 1){
                    free(tot_players);
                    return save;
                }
                break;
        }
    }

    if(count == FINAL){//solo nel caso il totale di giocatori rimasti sia 2 viene giocata la finale

        //aggiorna le statistiche 'totale giochi giocati' e 'SPR1D GAME giocati' ai profili che partecipano alla finale
        for (int i = 0; i < count; ++i) {
            for (int j = 0; j < save.n_profile; ++j) {
                if(tot_players[i] == save.profile[j].id){
                    save.profile[j].played_final++;
                    save.profile[j].totplayed_game++;
                }
            }
        }

        save = BlackJack(save,tot_players);

        //dopo la finale i dati che devono essere salvati vengono azzerati, e gli id di tutti i profili torna -1
        for (int i = 0; i < save.n_profile; ++i) {
            save.profile[i].id = -1;
        }

        save.game_stat = 0;
        save.ntot_player = 0;
        if(save.n_users != 0){
            free(save.index_user);
        }
        save.n_users = 0;
        free(save.player_state);
    }

    free(tot_players);

    return save;
}