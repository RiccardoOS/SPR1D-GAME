#include "Program.h"

SaveGame User_Choice (SaveGame save){

    int choice;

    /*chedo e faccio inserire quanti dei profili disponibili vuole utilizzare l'utente*/
    printf("Quanti dei profili giocatore creati/caricati vuoi utilizzare?\n"
           "Numero di profili: %d\n",save.n_profile);
    do {
        scanf("%d",&save.n_users);
        if (save.n_users < 0 || save.n_users > save.n_profile){
            printf("Numero di giocatori scelti non valido. Reinserisci\n");
        }
    }while (save.n_users < 0 || save.n_users > save.n_profile);

    if(save.n_users != 0){

        if((save.index_user = (int*) calloc (save.n_users,sizeof(int))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }

        for (int i = 0; i < save.n_users; ++i) {
            save.index_user[i] = -1;
        }

        printf("Quale tra i seguenti giocatori vuoi utilizzare?\n");
        for (int i = 0; i < save.n_profile; ++i) {
            printf("%d = %s\n", i + 1, save.profile[i].name);
        }

        for (int i = 0; i < save.n_users; ++i) {
            do {
                scanf("%d",&choice);
                if (choice < 1 || choice > save.n_profile){
                    printf("Scelta giocatore non disponibile. Reinserisci\n");
                }
            } while (choice < 1 || choice > save.n_profile);

            save.index_user[i] = choice - 1; //assegna l'indice del profilo all'array

            for (int j = i-1; j >= 0; j--) {
                if(save.index_user[i] == save.index_user[j]){
                    printf("Il personaggio e' gia' stato selezionato, sceglierne un altro\n");
                    do {
                        scanf("%d",&choice);
                        if (choice < 1 || choice > save.n_profile){
                            printf("Scelta giocatore non disponibile. Reinserisci\n");
                        }
                    } while (choice < 1 || choice > save.n_profile);

                    save.index_user[i] = choice - 1; //assegna l'indice del profilo all'array
                    j = i;
                }
            }

            printf("Hai scelto: %s\n",save.profile[choice - 1].name);
            if(i < save.n_users - 1){
                printf("Scegli ancora\n");
            }
        }

        for (int i = 0; i < save.n_profile; ++i) {
            save.profile[i].id = rand()%save.ntot_player;
            for (int j = i; j >= 0; j--) {
                if(save.profile[i].id == save.profile[j].id){
                    save.profile[i].id = rand()%save.ntot_player;
                    j=i;
                }
            }
        }
    }

    for (int i = 0; i < save.n_profile; ++i) {
        save.profile[i].spr1d_game_played++;
        save.profile[i].totplayed_game++;
    }

    return save;
}