#include "Program.h"

int main() {

    /*variabile SaveGame che portero' a spasso per tenere conto ed aggiornare i dati relativi ad ogni giocatore*/
    SaveGame save;

    srand(time(NULL));

    //inizializzo i dati della struttura
    save.n_profile = 0;
    save.game_stat = 0;
    save.n_users = 0;
    save.ntot_player = 0;
    PlayerProfile* profile1 = NULL;
    int user_choice, count_profiles = 0;
    bool condition = true;

    printf("Benvenuto al gioco dello SPR1D GAME, competi con altri giocatori o contro la cpu per superare "
           "l'esame di PR1.\n\n");

    printf("Per iniziare crea dei profili giocatore, carica una partita o inizia una nuova "
           "partita,\ncon l'ultima opzione giochera' esclusivamente la cpu per tutta la competizione.\n\n");

    do{

        printf("Scegli tra le varie opzioni:\n1 = Crea un profilo giocatore\n2 = Inizia nuova partita\n"
               "3 = Salva partita\n4 = Carica partita\n5 = Continua partita\n"
               "6 = Visualizza i profili giocatori\n0 = Uscire dal gioco\n");

        do{

            scanf("%d", &user_choice);
            getchar();
            if (user_choice < 0 || user_choice > 6){
                printf("Questa opzione non e' valida, riprova\n");
            }

        }while(user_choice < 0 || user_choice > 6);

        switch (user_choice) {
            case 0:
                break;
            case 1:
                if(save.game_stat == 0 && condition){
                    count_profiles++;
                    profile1 = Create_Player(count_profiles, profile1);
                    save.n_profile = count_profiles;
                    save.profile = profile1;
                }else{
                    printf("E' in corso una partita o e' stato caricato un file, impossibile creare nuovi profili\n");
                }

                break;

            case 2:
                if(save.game_stat == 0){
                    save = New_Game(save);
                }else{
                    printf("Hai una partita gia' in corso, finire la partita prima di avviarne una nuova\n");
                }

                break;

            case 3: //salva la partita sia in corso che non
                Save_Game(save);
                break;

            case 4: //carica la partita sia in corso che non
                save = Load_Game();
                condition = false;// in caso venga caricata una partita, non permette di creare profili utente
                break;

            case 5://Continua partita
                if(save.game_stat == 1){
                    save = Continue(save);
                }else{
                    printf("Nessuna partita e' in corso, impossibile continuare\n");
                }

                break;

            case 6: //etichetta che manda alla stampa dati dei profili
                if(save.n_profile != 0){
                    Print_Profile(save);
                }else{
                    printf("Se non ci sono profili non puoi vederli.\n");
                }
                break;

            default: //etichetta default non utile ma leva il warning dello switch
                exit (EXIT_FAILURE);
        }
    } while (user_choice >= 1 && user_choice <= 6);

    return 0;
}