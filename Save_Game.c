#include "Program.h"

void Save_Game (SaveGame save){

    int choice;
    char save_name[] = {"Saved_profiles.bin"};
    bool flag = false;
    FILE *save_fp = NULL;
    PlayerProfile* save_profiles = NULL;

    if(save.n_profile != 0){
        if((save_profiles = (PlayerProfile*) calloc (save.n_profile, sizeof(PlayerProfile))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }
    }


    //l'utente puo' scegliere il nome del file oppure il nome predefinito del file "Saved_profiles.bin"
    printf("Scegli una opzione:\n1 = Crea un nome per il salvataggio\n2 = Utilizza il nome prestabilito\n");
    do {
        scanf("%d", &choice);
        getchar();
        if (choice < 1 || choice > 2){
            printf("Opzione non disponibile, riprova\n");
        }
    }while (choice < 1 || choice > 2);

    //chiedo l'inserimento del nome del file
    if (choice == 1){
        printf("Inserisci il nome (il nome puo contenere solo lettere e spazi):\n");

        /*non permette di inserire altri caratteri oltre a lettere e spazi, nel caso ne vengano inseriti
         * permette di farlo reinserire*/
        do {
            gets(save_name); //acquisisce il nome del file
            Modify_Name(save_name); //modifica il nome con lettere maiuscole dopo ogni spazio, vedere la funzione Modify_Name

            /*cicla la stringa contenente il nome del file, e nel caso ci siano caratteri diversi da quelli richiesti
             * flag torna true e fa riniziare il while*/
            for (int i = 0; i < strlen(save_name); ++i) {
                if ((save_name[i] >= 'A' && save_name[i] <= 'Z') || (save_name[i] >= 'a' && save_name[i] <= 'z') || save_name[i] == ' ' ){
                    flag = false;
                }else{
                    flag = true;
                    i = (int) strlen(save_name); //in caso di carattere non valido termina il ciclo for
                }
            }

            if(flag){
                printf("Solo lettere e spazi... Riprova\n");
            }

        } while (flag);

        strcat(save_name,".bin"); //aggiunge alla stringa nome del file ".bin", per poter salvare il file correttamente

        Save_File_Name(save_name); //salva il nome del file in un file testo che contiene il nome di tutti i file salvati

    }

    if(save.n_profile != 0){
        save_profiles = save.profile; //utilizzo una variabile "PlayerProfile" per salvare i profili all'interno del file
    }

    save_fp = fopen(save_name,"wb"); //apro il file di salvataggio

    if (save_fp == NULL){
        printf("Errore apertura file\n");
        exit (EXIT_FAILURE);
    }

    /*inserisco i tutti i dati all'interno del file di salvataggio, nuovo o gia' esistente*/
    fwrite(&save.n_profile,sizeof (int),1,save_fp);

    fwrite(save_profiles,sizeof (PlayerProfile),save.n_profile,save_fp);

    if(save.game_stat == 0){
        save.ntot_player = 0;
        save.n_users = 0;
        fwrite(&save.game_stat, sizeof (int),1,save_fp);
        fwrite(&save.ntot_player, sizeof (int),1,save_fp);
        fwrite(&save.n_users, sizeof (int),1,save_fp);
    }else{
        fwrite(&save.game_stat, sizeof (int),1,save_fp);
        fwrite(&save.ntot_player, sizeof (int),1,save_fp);
        fwrite(&save.n_users, sizeof (int),1,save_fp);
        fwrite(save.index_user, sizeof (int), save.n_users, save_fp);
        fwrite(save.player_state, sizeof (int), save.ntot_player, save_fp);

    }

    fclose(save_fp);

}