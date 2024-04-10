#include "Program.h"

/*Funzione che modifica il nome dei giocatori o dei file binari salvati*/
void Modify_Name(char* str){

    for (int i = 0; i < strlen(str); ++i) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] + CHAR_DIST;
    }

    str[0] = str[0] - CHAR_DIST;

    for (int i = 0; i < strlen(str); ++i) {

        if(str[i] == ' '){
            str[i+1] = str[i+1] - CHAR_DIST;
        }
    }

}

/*funzione che permette di riallocare l'array dei giocatori totali tenendo gli id dei giocatori
 * presenti in esso*/
int* Realloc_Player (int* tot_players, int* ptr, int count_players){

    int count = 0;
    int* id = NULL; //array di supporto

    if((id = (int*) calloc (count_players, sizeof (int))) == NULL){
        printf("Errore: allocazione fallita\n");
        exit (EXIT_FAILURE);
    }

    /*assegno gli id rimasti all'array di supporto*/
    for (int i = 0; i < count_players; ++i) {
        if(tot_players[i] != -1){
            id[count++] = tot_players[i];
        }
    }

    /*rialloco la memoria dei giocatori totali*/
    if((tot_players = (int*) realloc (tot_players,count*sizeof (int))) == NULL){
        printf("Errore: allocazione fallita\n");
        exit (EXIT_FAILURE);
    }

    for (int i = 0; i < count; ++i) {
        tot_players[i] = id[i]; //riassegno gli id all'array di giocatori totali
    }

    *ptr = count;//tiene il conto dei giocatori rimasti nella funzione chiamante

    free(id);

    return tot_players;
}

/*funzione che aggiorna i giocatori utente*/
SaveGame Update_User (SaveGame save){

    int* index_user1 = NULL; //array di supporto per aggiornare l'array degli indici utenti
    int count = 0, index = 0;

    //tiene conto degli utenti ancora in gioco
    for (int i = 0; i < save.n_users; ++i) {
        if(save.profile[save.index_user[i]].id != -1){
            count++;
        }
    }

    if(count > 0 && count != save.n_users){

        if((index_user1 = calloc(count, sizeof (int))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }

        //con un ciclo salvo gli indici dei giocatori utente ancora in gioco
        for (int i = 0; i < save.n_users; ++i) {
            if(save.profile[save.index_user[i]].id != -1){
                index_user1[index++] = save.index_user[i];
            }
        }

        //rialloca l'array degli indici utente in base a quanti sono rimasti in gioco
        if((save.index_user = realloc(save.index_user,count*sizeof (int))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }

        save.n_users = count; //aggiorna il conteggio dei giocatori utente rimasti in partita

        //tramite l'array di supporto riassegno gli indici ancora in gioco
        for (int i = 0; i < save.n_users; ++i) {
            save.index_user[i] = index_user1[i];
        }

        free(index_user1);

    }else if(count == 0 && save.n_users > 0){
        // in caso non sia rimasto nessun giocatore utente libero la memoria dell'array indici e il numero di utenti a 0
        save.n_users = 0;
        free(save.index_user);
    }

    return save;
}

/*stampa le statistiche di ogni profilo presente nel file*/
void Print_Profile (SaveGame save) {

    printf("Dati giocatori:\n\n");
    for (int i = 0; i < save.n_profile; ++i) {

        printf("Nome: %s\n", save.profile[i].name);
        printf("ID: %d\n", save.profile[i].id);
        printf("SPR1D GAME giocati: %d\n", save.profile[i].spr1d_game_played);
        printf("SPR1D GAME vinti: %d\n", save.profile[i].spr1d_game_win);
        printf("Totale giochi giocati: %d\n", save.profile[i].totplayed_game);
        printf("Totale giochi vinti: %d\n", save.profile[i].totgame_win);
        printf("Finali giocate: %d\n\n", save.profile[i].played_final);

    }

    /*printf("Stato partita: %d\n",save.game_stat);
    printf("Totale giocatori: %d\n", save.ntot_player);
    printf("Numero utenti: %d\n", save.n_users);
    for (int i = 0; i < save.n_users; ++i) {
        printf("%d\n", save.index_user[i]);
    }
    for (int i = 0; i < save.ntot_player; ++i) {
        printf("%4d", save.player_state[i]);
    }
    printf("\n");*/
}

/*funzione per il giocatore Riccardo Scateni, ogni qualvolta Riccardo Scateni e' presente gli e' permesso scegliere
 * il gioco e vince tutte le partite*/
int Check_Frontman (SaveGame save, int* party, int n_party, int index){

    char str[] = {"Riccardo Scateni"};

    // nel caso Riccardo Scateni sia presente in partita ritorna l'indice del profilo
    for (int i = 0; i < save.n_profile; ++i) {
        for (int j = 0; j < n_party; ++j) {
            if(party[j] == save.profile[i].id){
                if(strcmp(str,save.profile[i].name)){
                }else{
                    index = j;
                }
            }
        }
    }

    return index;
}

SaveGame Check_Profile (SaveGame save, int count_players){

    int* id1 = NULL; //array di supporto
    int count = 0;

    //conta quanti profili sono ancora in gioco
    for (int i = 0; i < save.n_profile; ++i) {
        if(save.profile[i].id != -1){
            count++;
        }
    }


    if(count > 0){//solo se almeno un profilo e' ancora in partita
        if((id1 = (int*) calloc (count, sizeof (int))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }

        //genero id casualmente all'interno dell'array di supporto
        for (int i = 0; i < count; ++i) {
            id1[i] = rand() % count_players;
            for (int j = i; j >= 0 ; j--) {
                if(id1[i] == id1[j]){
                    id1[i] = rand() % count_players;
                    j=i;
                }
            }
        }

        count = 0;

        //per ogni giocatore ancora in partita assegno l'id univoco a ciascun giocatore
        for (int i = 0; i < save.n_profile; ++i) {
            if(save.profile[i].id != -1){
                save.profile[i].id = id1[count++];
            }
        }

        free(id1);
    }

    return save;
}

int* Create_2Party (SaveGame save, int* tot_players, int count_players, int rand_game){

    int* party = NULL; //array gruppo
    int count = 0;

    if((party = (int*) calloc (N_2PARTY,sizeof (int))) == NULL){
        printf("Errore: allocazione fallita\n");
        exit(EXIT_FAILURE);
    }

    while(count < count_players){
        //array gruppo viene aggiornato ogni volta che finisce un gioco ricevendo i giocatori a 2 a 2
        for (int i = 0; i < N_2PARTY; ++i) {
            party[i] = tot_players[count++];
        }

        //aggiorno la statistica 'totale giochi giocati' ai profili presenti in partita
        for (int i = 0; i < N_2PARTY; ++i) {
            for (int j = 0; j < save.n_profile; ++j) {
                if(party[i] == save.profile[j].id){
                    save.profile[j].totplayed_game++;
                }
            }
        }

        //dalla funzione 'Continue' viene passata la scelta di tale gioco il quale viene richiamato da questa funzione
        switch (rand_game) {
            case 1:
                tot_players = Odd_or_Even(save, party, tot_players, count_players);
                break;
            case 2:
                tot_players = Dice_Game(save, party, tot_players, count_players);
                break;
            case 3:
                tot_players = Murra(save, party, tot_players, count_players);
                break;
            case 4:
                tot_players = Morra_Cinese(save, party, tot_players, count_players);
                break;
            case 5:
                tot_players = Tic_Tac_Toe(save, party, tot_players, count_players);

                break;
            case 6:
                tot_players = Marble(save, party, tot_players, count_players);
                break;
        }

    }

    free(party);

    return tot_players;
}

int* Create_4Party (SaveGame save, int* tot_players, int count_players, int rand_game){

    int* party = NULL; //array gruppo
    int count = 0;


    if((party = (int*) calloc (N_4PARTY,sizeof (int))) == NULL){
        printf("Errore: allocazione fallita\n");
        exit(EXIT_FAILURE);
    }

    while(count < count_players) {

        //array gruppo viene aggiornato ogni volta che finisce un gioco ricevendo i giocatori a 2 a 2
        for (int i = 0; i < N_4PARTY; ++i) {
            party[i] = tot_players[count++];
        }

        //aggiorno la statistica 'totale giochi giocati' ai profili presenti in partita
        for (int i = 0; i < N_4PARTY; ++i) {
            for (int j = 0; j < save.n_profile; ++j) {
                if (party[i] == save.profile[j].id) {
                    save.profile[j].totplayed_game++;
                }
            }
        }

        //dalla funzione 'Continue' viene passata la scelta di tale gioco il quale viene richiamato da questa funzione
        switch (rand_game) {
            case 7:
                tot_players = Ponte_di_Vetro(save, party, tot_players, count_players);
                break;
            case 8:
                tot_players = Impiccato(save, party, tot_players, count_players);
                break;
        }
    }

    free(party);

    return tot_players;
}

char** Dictionary (char** dictionary, int* pt){

    int n_row, count;
    char eol;
    FILE *fp = NULL;

    if((fp = fopen("default.txt","r")) == NULL){
        printf("Errore apertura file\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fp,"%d ", &n_row);// prende da file il numero di righe del dizionario

    *pt = n_row;// passo il numero di righe alla funzione chiamante

    //crea una matrice col numero di righe del dizionario
    if((dictionary = (char**) calloc (n_row,sizeof(char*))) == NULL){
        printf("Errore: allocazione fallita\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n_row; ++i) {
        if((dictionary[i] = (char*) calloc (MAX_FRASE,sizeof (char))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit(EXIT_FAILURE);
        }
    }

    //all'interno della matrice inserisco le frasi del dizionario
    for (int i = 0; i < n_row; ++i) {
        fgets(dictionary[i],MAX_FRASE,fp);
    }

    /*questo algoritmo mi permette di determinare la corretta lunghezza della stringa, reallocare le righe
     * della matrice e scambiare ogni fine rigna il '\n' che viene preso dal file con il terminatore di stringa '\0' */
    for (int i = 0; i < n_row; ++i) {
        count = 0;
        eol = 'a';

        while (eol != '\n'){
            eol = dictionary[i][count++];
        }

        if ((dictionary[i] = (char*) realloc (dictionary[i], count*sizeof(char))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }
        dictionary[i][count-1] = '\0';
    }

    fclose(fp);

    return dictionary;
}