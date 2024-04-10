#include "Program.h"

void Game1 (SaveGame, char*, char**, int, int );

int* Impiccato (SaveGame save, int* party, int* tot_players, int count_players){

    int n_row, rand_frase, count = 0, check_winner;
    int* pt_row = NULL;
    char** dictionary = NULL;
    char* frase = NULL;
    bool condition = true, flag = true;

    pt_row = &n_row;// tramite il puntatore tengo conto delle righe all'interno del dizionario
    dictionary = Dictionary (dictionary, pt_row);// legge dal file 'default.txt' il dizionario

    rand_frase = rand()%n_row;// prende casualmente una frase all'interno del dizionario

    //stringa in cui viene inserita la frase casuale
    if((frase = (char*) calloc (strlen(dictionary[rand_frase]),sizeof (char))) == NULL){
        printf("Errore: allocazione fallita\n");
        exit(EXIT_FAILURE);
    }

    strcpy(frase,dictionary[rand_frase]);// copio la frase casuale nella stringa

    //trasformo le lettere della frase con gli underscore
    for (int i = 0; i < strlen(frase); ++i) {
        if(frase[i] == ' '|| frase[i] == ',' || frase[i] == '.' || frase[i] == 39) {

        }else{
            frase[i] = '_';
        }
    }

    for (int i = 0; i < N_4PARTY; ++i) { //con questo algoritmo vedo se ci sono utenti in partita
        for (int j = 0; j < save.n_users; ++j) {
            if(party[i] == save.profile[save.index_user[j]].id){
                count++;
            }
        }
    }

    if(count == 0){ // se non ci sono utenti genera un vincitore casuale
        check_winner = rand()%N_4PARTY;
    }else{

        printf("Benvenuti al gioco dell'impiccato, il primo che riesce ad indovinare la parola vince!\n"
               "Buona fortuna!!\n");

        puts(frase);//stampa la frase con gli underscore

        while(strcmp(frase,dictionary[rand_frase]) && flag){// ciclo che si conclude quando la frase e' stata completata
            for (int i = 0; i < N_4PARTY; ++i) {

                Game1(save, frase, dictionary, rand_frase, party[i]);// richiama la funzione per far giocare gli utenti

                if(strcmp(frase,dictionary[rand_frase])){
                }else{ //l'ultima lettera mancante della frase determina il vincitore della partita
                    check_winner = i;
                    flag = false;
                    i = N_4PARTY;
                }
            }
        }

        puts(frase); //stampa la frase completa
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

void Game1(SaveGame save, char* frase, char** dictionary, int rand_frase, int id){

    char move;
    bool flag = false;

    for (int i = 0; i < save.n_users; ++i) {
        if(id == save.profile[save.index_user[i]].id){
            printf("%s: indovina la lettera che pensi ci sia all'interno della frase\n"
                   "(solo lettere minuscole)\n",save.profile[save.index_user[i]].name);
            flag = true;// permette di far giocare l'utente
            i = save.n_users;
        }
    }

    if(flag){// se flag e' true gioca l'utente
        do{
            scanf("%c", &move);
            if (move < 'a' || move > 'z'){
                printf("Reinserisci\n");
            }
        }while(move < 'a' || move > 'z');

        getchar();
    }else{// se flag e' false gioca la cpu
        move = 'a' + rand()%('z' - 'a' + 1);
    }

    /*ricerca all'interno della frase la lettera inserita dal giocatore, in caso la lettera esiste nella frase
      trasforma l'underscore nella lettera selezionata */
    for (int i = 0; i < strlen(frase); ++i) {
        if(move == dictionary[rand_frase][i] || move - CHAR_DIST == dictionary[rand_frase][i]){
            if(dictionary[rand_frase][i] == move - CHAR_DIST){
                frase[i] = move - CHAR_DIST;
            }else{
                frase[i] = move;
            }
        }
    }

    if(flag){// se nel turno ha giocato l'utente, stampa la frase riempita con le lettere giocate
        puts(frase);
    }

}