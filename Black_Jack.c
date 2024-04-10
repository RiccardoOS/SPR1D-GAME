#include "Program.h"

#define MAZZO 52
#define BLACK_JACK 21
#define HAND_CARD 2

int* Create_Deck(int *);
int* Shuffle(int *, int, int);
int Distribute_Card(int *, int *);
int Point_Value(int *, int);

typedef enum {
    CUORI, QUADRI = 13, FIORI = 26, PICCHE = 39
} Seed;

typedef enum {
    ASSO = 1, FANTE = 11, REGINA, RE
} Figure;

SaveGame BlackJack (SaveGame save, int* party){

    int count = 0, index, check_winner = -1, point1, point2, id;
    int* deck = NULL, *hand = NULL;
    bool condition = true;

    printf("Benvenuti all'ultima gara dello SPR1D GAME,\nquest'ultima consiste nel gioco di BLACK JACK."
           " che i finalisti giochino.\nBuona fortuna!!\n");



    for (int i = 0; i < N_2PARTY; ++i) { //con questo algoritmo vedo se ci sono utenti in partita
        for (int j = 0; j < save.n_users; ++j) {
            if(party[i] == save.profile[save.index_user[j]].id){
                count++;
                index = i;
            }
        }
    }
    //crea il mazzo di carte
    deck = Create_Deck(deck);

    switch (count) {
        case 0://nessun utente
            check_winner = rand()%N_2PARTY;
            break;
        case 1://utente contro cpu
            if(index == 1){
                id = party[index];
                party[1] = party[0];
                party[0] = id;
            }

            //ciclo 'do while' che termina quando c'e' un vincitore
            do{

                //mischia il mazzo di carte
                for (int i = 0; i < MAZZO; ++i) {
                    deck = Shuffle(deck, rand()%MAZZO, rand()%MAZZO);
                }

                for (int i = 0; i < save.n_profile; ++i) {
                    if(party[0] == save.profile[i].id){
                        printf("%s ecco le tue carte\n",save.profile[i].name);
                    }
                }

                //tramite questo array valuto le carte in mano all'utente
                if((hand = (int*) calloc (HAND_CARD,sizeof (int))) == NULL){
                    printf("Errore: allocazione fallita\n");
                    exit(EXIT_FAILURE);
                }

                point1 = Distribute_Card (deck, hand);// richiama la funzione per far giocare l'utente

                point2 = 15 + rand()%(25 - 15 + 1);// la cpu genera una somma delle carte casuale tra 15 e 25

                //valuta il vincitore della partita
                if(point1 > BLACK_JACK){
                    check_winner = 1;
                }
                if(point2 > BLACK_JACK){
                    check_winner = 0;
                }

                if(point1 > point2 && point1 <= BLACK_JACK){
                    check_winner = 0;
                }
                if(point2 > point1 && point2 <= BLACK_JACK){
                    check_winner = 1;
                }

            } while (point1 == point2 && check_winner == -1);

            break;
        case 2:

            //ciclo 'do while' che termina quando c'e' un vincitore
            do {

                //somma delle carte dei giocatori
                point2 = 0;//viene rinizializzato in caso di pareggio
                point1 = 0;//viene rinizializzato in caso di pareggio

                for (int i = 0; i < N_2PARTY; i++) {
                    for (int j = 0; j < save.n_profile; j++) {
                        if(party[i] == save.profile[j].id){
                            printf("%s ecco le tue carte\n",save.profile[j].name);
                        }
                    }

                    //mischia il mazzo di carte
                    for (int j = 0; j < MAZZO; j++) {
                        deck = Shuffle(deck, rand()%MAZZO, rand()%MAZZO);
                    }

                    //tramite questo array valuto le carte in mano all'utente
                    if((hand = (int*) calloc (HAND_CARD,sizeof (int))) == NULL){
                        printf("Errore: allocazione fallita\n");
                        exit(EXIT_FAILURE);
                    }
                    if(i == 0){// in base all'indice gioca il giocatore 1 o il giocatore 2
                        point1 = Distribute_Card (deck, hand);
                    }else{
                        point2 = Distribute_Card (deck, hand);
                    }

                    //valuta il vincitore della partita
                    if(point1 > BLACK_JACK){
                        check_winner = 1;
                        i = N_2PARTY;
                    }
                    if(point2 > BLACK_JACK){
                        check_winner = 0;
                        i = N_2PARTY;
                    }

                    if(point1 > point2 && point1 <= BLACK_JACK && point2 != 0 && point1 != 0){
                        check_winner = 0;
                    }

                    if(point2 > point1 && point2 <= BLACK_JACK && point2 != 0 && point1 != 0){
                        check_winner = 1;
                    }

                    free(hand);

                }


            } while (point1 == point2 && check_winner == -1);

            break;
    }

    // valuta se Riccardo Scateni e' in partita, se si vince sempre
    check_winner = Check_Frontman (save, party, N_2PARTY, check_winner);

    for (int i = 0; i < save.n_profile; ++i) {//se il vincitore e' un profilo stampa il nome e l'id
        if(party[check_winner] == save.profile[i].id){
            printf("Il vincitore e': %s ID: %d\n\n",save.profile[i].name, party[check_winner]);
            save.profile[i].totgame_win++;// se il vincitore e' un profilo aumenta il totale di giochi vinti
            save.profile[i].spr1d_game_win++;// se il vincitore e' un profilo aumenta il le finali vinte
            condition = false;
        }
    }
    //altrimenti stampa solo l'id
    if(condition){
        printf("Il vincitore e': ID: %d\n\n",party[check_winner]);
    }

    free(deck);
    return save;
}

int* Create_Deck(int *deck) {

    int seme = CUORI, card = 1;

    if((deck = (int*) calloc (MAZZO, sizeof (int))) == NULL){
        printf("Errore: allocazione fallita\n");
        exit(EXIT_FAILURE);
    }

    //crea il mazzo di carte, vengono assegnate 13 carte per ogni seme
    switch (seme) {
        case CUORI:
            for (int i = CUORI; i < QUADRI; ++i) {
                deck[i] = card++;
            }
            card = 1;
        case QUADRI:
            for (int i = QUADRI; i < FIORI; ++i) {
                deck[i] = card++;
            }
            card = 1;
        case FIORI:
            for (int i = FIORI; i < PICCHE; ++i) {
                deck[i] = card++;
            }
            card = 1;
        case PICCHE:
            for (int i = PICCHE; i < MAZZO; ++i) {
                deck[i] = card++;
            }
            break;
    }

    return deck;
}

int* Shuffle(int *deck, int val1, int val2) {// funzione che scambia i valori all'interno del mazzo

    int temp = deck[val1];

    deck[val1] = deck[val2];
    deck[val2] = temp;

    return deck;
}

int Distribute_Card(int *deck, int *hand) {// funzione che permette la giocata del giocatore

    int point, hand_card = 2, condition;

    //ciclo 'do while' che termina quando il giocatore non vuole piu' carte o viene superato 21 con la somma delle carte
    do {
        printf("Carte in mano:  ");
        for (int i = 0; i < hand_card; ++i) {// stampa le carte in mano dell'utente
            hand[i] = deck[i];// la mano dell'utente si riempie con le prime carte del mazzo
            switch (hand[i]) {
                case ASSO:
                    printf("ASSO  ");
                    break;
                case FANTE:
                    printf("FANTE  ");
                    break;
                case REGINA:
                    printf("REGINA  ");
                    break;
                case RE:
                    printf("RE  ");
                    break;
                default:
                    printf("%d  ", hand[i]);
                    break;
            }

        }

        point = Point_Value(hand, hand_card);// chiamata alla funzione che calcola il punteggio
        printf("\nSomma: %d\n", point);// stampa la somma delle carte

        if(point < BLACK_JACK){// se il giocatore ha fatto black jack o ha superato il valore non permette di prendere altre carte
            printf("Vuoi un'altra carta?\n0 = no\n1 = si\n");

            //l'utente decide se avere altre carte oppure no
            do {
                scanf("%d", &condition);
                if(condition < 0 || condition > 1){
                    printf("Opzione non disponibile\n");
                }
            }while(condition < 0 || condition > 1);
            if(condition){// se l'utente decide di prendere un'altra carta rialloco la mano con una carta in piu'
                hand = (int *) realloc (hand, (++hand_card) * sizeof(int));
            }
        }

    } while (condition && point < BLACK_JACK);

    return point;
}

int Point_Value (int *hand, int hand_card) {//calcola la somma delle carte

    int point = 0, condition;

    //somma le carte in mano
    for (int i = 0; i < hand_card; ++i) {
        switch (hand[i]) {
            case ASSO:// l'utente puo' scegliere quanto far valere l'asso
                printf("\nQuanto vuoi far valere l'asso?\n1 = 1 punto\n2 = 11 punti\n");
                do {
                    scanf("%d", &condition);
                    if(condition < 1 || condition > 2){
                        printf("L'asso puo' valere solo 1 o 11\n");
                    }
                } while (condition < 1 || condition > 2);

                //in base alla scelta dell'utente il valore dell'asso cambia
                if(condition == 1){
                    point += 1;
                } else{
                    point += 11;
                }
                break;
            case FANTE:
            case REGINA:
            case RE:
                point += 10;// tutte le figure valgono 10 punti
                break;
            default:
                point += hand[i];
                break;
        }
    }

    return point;
}