#include "Program.h"

#define RAND_NUM 1000

int Player (SaveGame, int);

int* Skimming (SaveGame save, int* tot_players){

    int* party;
    int power = 4, remaining_player,n_party1, count = 0, n_party, count_player = 0;

    /*in base al numero totale dei giocatori valuta la potenza di 2 piu' vicina*/
    while (pow(BASE_POWER, power) < save.ntot_player){
        power++;
    }


    remaining_player = pow(BASE_POWER, power - BASE_POWER); //salvo il numero di giocatori che rimarranno alla fine del gioco
    n_party = save.ntot_player / remaining_player; //numero di giocatori per gruppo
    n_party1 = save.ntot_player % remaining_player; //numero dei gruppi che avranno 1 giocatore in piu'

    /*nel caso il modoulo tra giocatori totali e i giocatori che dovranno rimanere sia diverso da 0, vengono creati tot gruppi
     *con tot giocatori*/
    if(n_party1 != 0){

        /*creo l'array di supporto che utilizzero' per mandare al gioco il gruppo di giocatori*/
        if((party = (int*) calloc (n_party+1,sizeof (int))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }

        /*ciclo while continua fintanto che count arriva al numero di giocatori rimanenti*/
        while(count < remaining_player){
            /*se count e' piu' piccolo del numero di giocatori che avranno un giocatore in piu', i gruppi vengono
             * mandati con il numero di giocatori per gruppo + 1*/
            if(count < n_party1){
                for (int i = 0; i < n_party + 1; ++i) { //assegno gli id al gruppo che partecipa al gioco
                    party[i] = tot_players[count_player];
                    count_player++; //tiene conto dei giocatori partecipanti per gruppo
                }
                tot_players = Guess_the_Number(party, save, n_party + 1, tot_players); //richiama il gioco col gruppo creato
            }else{
                /*se count arriva al numero di giocatori che avranno un giocatore in piu' rialloco la memoria del gruppo
                 * con numero di giocatori per gruppo*/
                if(count == n_party1 ){
                    if((party = (int*) realloc (party,n_party*sizeof (int))) == NULL){
                        printf("Errore: allocazione fallita\n");
                        exit (EXIT_FAILURE);
                    }
                }

                for (int i = 0; i < n_party; ++i) { //assegno gli id al gruppo che partecipa al gioco
                    party[i] = tot_players[count_player];
                    count_player++; //tiene conto dei giocatori partecipanti per gruppo
                }

                tot_players = Guess_the_Number(party, save, n_party, tot_players); //richiama il gioco col gruppo creato

            }
            count++;
        }
        /*nel caso il modoulo tra giocatori totali e i giocatori che dovranno rimanere sia uguale a 0, i gruppi saranno
         * tutti del numer di giocatori per gruppo*/
    }else{
        /*creo l'array di supporto che utilizzero' per mandare al gioco il gruppo di giocatori*/
        if((party = (int*) calloc (n_party,sizeof (int))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }

        while(count < remaining_player){
            for (int i = 0; i < n_party; ++i) {//assegno gli id al gruppo che partecipa al gioco
                party[i] = tot_players[count_player];
                count_player++; //tiene conto dei giocatori partecipanti per gruppo
            }
            tot_players = Guess_the_Number(party, save, n_party, tot_players); //richiama il gioco col gruppo creato
            count++;
        }
    }

    free(party);

    return tot_players;
}

int* Guess_the_Number(int* party, SaveGame save, int n_party, int* tot_players){

    int rand_num, player = -1, cpu = -1, count = 0;
    int check_winner;
    bool condition = true;

    rand_num = 1;//rand()%RAND_NUM;

    for (int i = 0; i < n_party; ++i) { //con questo algoritmo vedo se ci sono utenti in partita
        for (int j = 0; j < save.n_users; ++j) {
            if(party[i] == save.profile[save.index_user[j]].id){
                count++;
            }
        }
    }

    if(count > 0){
        printf("\nBenvenuti al gioco: INDOVINA IL NUMERO!\nVerra' generato un numero da 0 a 999, "
               "provate ad indovinare qual e'.\nBuona fortuna\n");
    }


    /*il gioco non finisce fintanto che un partecipante non indovina il numero generato*/
    while(player != rand_num && cpu != rand_num){
        for (int i = 0; i < n_party; ++i) { //ciclo per la giocata o dell'utente o della cpu
            /*check in base all'id per riconoscere se la giocata e' da parte dell'utente*/
            for (int j = 0; j < save.n_users; ++j) {
                if(party[i] == save.profile[save.index_user[j]].id){
                    player = Player(save, party[i]);
                    condition = false; //se la giocata e' dell'utente non permette la giocata della cpu
                    j = save.n_users;
                    if(player == rand_num){
                        check_winner = i; //assegna l'indice dell'id vincitore
                        i = n_party;
                    }
                }else{
                    condition = true;
                }
                if(player > rand_num && condition == false){
                    printf("Il numero da indovinare e' minore\n");

                }else if(player < rand_num && condition == false){
                    printf("Il numero da indovinare e' maggiore\n");
                }
            }
            if(condition){
                cpu = rand()%RAND_NUM;
            }
            if(cpu == rand_num){
                check_winner = i;
                i = n_party;
            }
        }
    }

    check_winner = Check_Frontman (save, party, n_party, check_winner);
    condition = true;

    if(count > 0){
        printf("\n_____FINE DEL GIOCO_____\n");

        /*stampa l'ID del giocatore vincitore, ciclo che verifica se e' un profilo allora stampa anche il nome,
         * altrimenti solo l'ID se e' una cpu*/
        for (int i = 0; i < save.n_profile; ++i) {
            if(party[check_winner] == save.profile[i].id){
                printf("Il vincitore e': %s ID: %d\n\n",save.profile[i].name, party[check_winner]);
                condition = false;
            }
        }
        if(condition){
            printf("Il vincitore e': ID: %d\n\n",party[check_winner]);
        }
    }

    /*modifico gli id dei giocatori eliminati assegnandoli -1*/
    for (int i = 0; i < n_party; ++i) {
        for (int j = 0; j < save.ntot_player; ++j) {
            if (party[check_winner] != tot_players[j] && party[i] == tot_players[j]){
                tot_players[j] = - 1;
            }
        }
    }

    return tot_players;
}

int Player(SaveGame save, int id){

    int player;

    /*stampa del giocatore utente partecipante*/
    for (int i = 0; i < save.n_profile; ++i) {
        if (id == save.profile[i].id){
            printf("\nE' il turno di: %s\n",save.profile[i].name);
        }
    }
    /*faccio inserire il numero della giocata*/
    printf("Indovina il numero, inserisci il valore che pensi sia giusto (da 0 a 999)\n");
    do {
        scanf("%d", &player);
        if (player < 0 || player > RAND_NUM-1){
            printf("Reinserisci\n");
        }
    } while (player < 0 || player > RAND_NUM-1);

    return player;
}