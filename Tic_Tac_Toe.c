#include "Program.h"

#define DIM 3
#define WIN 3
#define MAX_TURN 5

int Tris (SaveGame, int table[][DIM], int*, int);
int Check_Win (int table[][DIM], char);

int* Tic_Tac_Toe (SaveGame save, int* party, int* tot_players, int count_players){

    int table [DIM][DIM], count = 0, index, check_winner, n = 1, id;
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
        printf("Benvenuti al gioco del tris, il primo che riesce a mettere 3 pedine in fila vince\n"
               "Buona fortuna!!\n");

        printf("Considera la tabella in questo modo, il numero all'interno della tabella indica la posizione\n");

        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                printf("|  %d  |", n++);
            }
            printf("\n");
        }
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
            check_winner = Tris(save, table, party, count); //inizia la partita di tris
            break;
        case 2:// utente contro utente
            check_winner = Tris(save, table, party, count); //inizia la partita di tris
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

int Tris (SaveGame save, int table[][DIM], int* party, int count){

    int move, win, check_winner, turn, cpu_r, cpu_c;
    char symbol;
    int* pointer = NULL;
    bool flag = true;

    //ciclo 'do while' che termina solo quando uno dei 2 giocatori fa tris
    do {
        turn = 0;// reinizializzo i turni in caso di pareggio

        //Ogni volta che rinizia una nuova partita viene reinizializzata la matrice
        for (int i = 0; i < DIM ; ++i) {
            for (int j = 0; j < DIM ; ++j) {
                table[i][j] = ' ';
            }
        }

        //ciclo 'do while' che termina o quando c'e' un vincitore o la tabella e' piena senza vincitore
        do{
            turn++;// tiene conto dei turni

            printf("\n_____TURNO %d_____\n", turn);

            for (int i = 0; i < N_2PARTY; ++i) {// ciclo che permette la giocata dei giocatori
                for (int j = 0; j < save.n_users; ++j) { // valuta se ci sono utenti in partita
                    if(party[i] == save.profile[save.index_user[j]].id){
                        printf("E' il turno di: %s\n",save.profile[save.index_user[j]].name);
                        //indica in base all'indice la pedina utilizzata
                        if(i == 0){
                            printf("La tua pedina e': 'x'\n");
                        }else{
                            printf("La tua pedina e': 'o'\n");
                        }

                        //l'utente inserisce la sua mossa che vine fatta rifare in caso di numero sbagliato o casella gia' piena
                        do {
                            scanf("%d", &move);

                            switch (move) {// la mossa viene inserita con un puntatore all'interno della matrice in base alla scelta dell' utente
                                case 1:
                                    pointer = &table[0][0];
                                    break;
                                case 2:
                                    pointer = &table[0][1];
                                    break;
                                case 3:
                                    pointer = &table[0][2];
                                    break;
                                case 4:
                                    pointer = &table[1][0];
                                    break;
                                case 5:
                                    pointer = &table[1][1];
                                    break;
                                case 6:
                                    pointer = &table[1][2];
                                    break;
                                case 7:
                                    pointer = &table[2][0];
                                    break;
                                case 8:
                                    pointer = &table[2][1];
                                    break;
                                case 9:
                                    pointer = &table[2][2];
                                    break;
                            }

                            if ((move < 1 || move > 9) || *pointer != ' ') {
                                printf("Reinserisci\n");
                            }

                        } while ((move < 1 || move > 9) || *pointer != ' ');

                        if(i == 0){// in base all'indice viene inserito nella tabella il simbolo del giocatore
                            symbol = 'x';
                            *pointer =  symbol;
                        }else{
                            symbol = 'o';
                            *pointer = symbol;
                        }

                        //Richiamo la funzione per verificare se il giocatore ha vinto
                        win = Check_Win (table, symbol);

                        //se vi e' un vincitore esce dal ciclo
                        if(win == WIN){
                            check_winner = i;
                            j = save.n_users;
                            i = N_2PARTY;
                        }
                        flag = false;
                    }
                }

                //nel caso vi sia un solo utente e la cpu permette la giocata alla cpu
                if(count == 1 && flag && turn != MAX_TURN){
                    //Mossa della cpu, esce dal ciclo solo se la casella scelta e' vuota
                    do {
                        cpu_r = rand()%DIM;
                        cpu_c = rand()%DIM;
                    } while (table[cpu_r][cpu_c] != ' ');

                    if(i == 0){
                        symbol = 'x';
                        table[cpu_r][cpu_c] =  symbol;
                    }else{
                        symbol = 'o';
                        table[cpu_r][cpu_c] = symbol;
                    }

                    //Richiamo la funzione per verificare se il giocatore ha vinto
                    win = Check_Win (table, symbol);
                    if(win == WIN){
                        check_winner = i;
                        i = N_2PARTY;
                    }
                }
                flag = true;

            }

            //Stampa la tabella con le ultime giocate
            for (int i = 0; i < DIM; ++i) {
                for (int j = 0; j < DIM; ++j) {
                    printf("|  %c  |", table[i][j]);
                }
                printf("\n");
            }

            printf("\n");

        }while(turn != MAX_TURN && win != WIN);

    }while(win != WIN);

    return check_winner;
}

int Check_Win (int table[][DIM], char symbol) {

    int win = 0;

    //Check verticale
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM ; ++j) {
            //Verifico i simboli verticalmente, se sono uguali al simbolo del giocatore aumenta "win"
            if (table[j][i] == symbol) {
                win++;
            }
            if(win == WIN){ //Se "win" e' uguale a 3 esce dal ciclo "for"
                i = DIM;
            }
        }
        if (win != WIN){ //Se la colonna si e' conclusa e "win" non e' arrivato a 3, riparte il conteggio
            win = 0;
        }
    }

    //Nei prossimi check entra solo se "win" non ha raggiunto 3 nel check precedente

    //Check orizzontale
    if (win != WIN){
        for (int i = 0; i < DIM ; ++i) { //Uguale alla verticale, ma orizzontalmente
            for (int j = 0; j < DIM ; ++j) {
                if (table[i][j] == symbol){
                    win++;
                }
                if(win == WIN){
                    i = DIM;
                }
            }
            if (win != WIN){
                win = 0;
            }
        }
    }

    //Check diagonale
    if (win != WIN){
        for (int i = 0; i < DIM ; ++i) {
            if(table[i][i] == symbol){
                win++;
            } else{
                win = 0;
            }
        }
    }

    //Check diagonale inversa
    if (win != WIN){
        for (int i = 0; i < DIM ; ++i) {
            if(table[i][2-i] == symbol){
                win++;
            }else{
                win = 0;
            }
        }
    }

    return win;
}
