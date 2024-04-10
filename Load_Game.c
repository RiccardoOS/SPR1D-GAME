#include "Program.h"

SaveGame Load_Game(){

    FILE *fp = NULL;
    SaveGame save;
    PlayerProfile* profiles = NULL;
    int*ptr = NULL;
    int n_row = 0, choice = 0;
    int n_profiles;
    int *user_index3 = NULL;
    int *player_state2 = NULL;
    char** file_name = NULL;


    ptr = &n_row; //utilizzo il passaggio per riferimento per ottenere il numero di file salvati dalla funzione che richiamo successivamente

    file_name = Load_File_Name(file_name,ptr); //ottengo i nomi dei file salvati in precedenza

    //stampa i nomi dei file esistenti
    printf("Scegli un salvataggio:\n");
    for (int i = 0; i < n_row; ++i) {
        printf("%d = %s\n", i + 1,file_name[i]);
    }

    //permette la scelta tra i file salvati in precedenza
    do {
        scanf("%d",&choice);
        if(choice < 1 || choice > n_row){
            printf("Opzione non disponibile, Riprova");
        }
    } while (choice < 1 || choice > n_row);

    if ((fp = fopen(file_name[choice -1],"rb")) == NULL){ //apre il file scelto dall'utente
        exit (EXIT_FAILURE);
    }

    fread(&n_profiles,sizeof(int),1,fp); //prende il numero di profili contenuti nel file

    if ((profiles = (PlayerProfile*) malloc(n_profiles*sizeof (PlayerProfile))) == NULL){
        exit (EXIT_FAILURE);
    }

    fread(profiles,sizeof(PlayerProfile),n_profiles,fp); //prende l'array di profili

    fread(&save.game_stat,sizeof (int), 1, fp); //prende lo stato della partita

    if(save.game_stat == 1){
        fread(&save.ntot_player,sizeof (int),1, fp); //prende il numero totale di giocatori nella partita, se la partita non e' in corso il dato e' 0

        fread(&save.n_users,sizeof (int),1, fp); //prende il numero degli utenti nella partita, se la partita non e' in corso il dato e' 0

        user_index3 = (int*) calloc (save.n_users, sizeof (int));

        player_state2 = (int*) calloc (save.ntot_player, sizeof (int));

        fread(user_index3, sizeof(int), save.n_users, fp);

        fread(player_state2, sizeof (int), save.ntot_player, fp);
    }

    fclose(fp);

    if ((save.profile = (PlayerProfile*) malloc(n_profiles*sizeof (PlayerProfile))) == NULL){
        exit (EXIT_FAILURE);
    }

    //salva le variabili prese dal file nella struttura SaveGame
    save.n_profile = n_profiles;
    save.profile = profiles;

    if(save.game_stat == 1){
        save.player_state = (int*) calloc (save.ntot_player, sizeof (int));
        save.index_user = user_index3;
        save.player_state = player_state2;
    }


    return save;
}
