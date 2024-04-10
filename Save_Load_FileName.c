#include "Program.h"

void Save_File_Name (char* str){

    FILE *name_file = NULL;
    int* ptr = NULL, n_row = 0;
    char** file = NULL;
    bool flag;

    ptr = &n_row;

    file = Load_File_Name(file,ptr); //apro il file contente i nomi dei file di salvataggio per poter essere modificato

    for (int i = 0; i < n_row; ++i) {
        if(strcmp(str,file[i])){
            flag = true;
        }else{
            flag = false;
            i = n_row;
        }
    }

    if(flag){
        n_row += 1; //aggiorna il numero di profili di salvataggio

        //alloca nuovo spazio alla matrice nomi per inserire il nuovo file di salvataggio
        if((file = (char**) realloc (file,n_row*sizeof(char*))) == NULL){
            exit(EXIT_FAILURE);
        }

        file[n_row - 1] = (char*) calloc (strlen(str),sizeof (char)); //alloca spazio all'ultima riga della matrice
        strcpy(file[n_row - 1],str); //copia il nome del nuovo nome del file all'interno della matrice

        for (int i = 0; i < n_row; ++i) {
            file[i][strlen(file[i])] = '\n';
        }

        /*apro il file testo "FileName" per aggiornarlo con il nuovo nome del file di salvataggio*/
        name_file = fopen("FileName.txt","w");

        fprintf(name_file,"%d\n",n_row); //inserisce il numero di file esistenti

        //inserisce il nome di tutti i file esistenti
        for (int i = 0; i < n_row; ++i) {
            fputs(file[i], name_file);
        }

        fclose(name_file); //chiudo il file

        //libero la memoria che occupa la matrice
        for (int i = 0; i < n_row; ++i) {
            free(file[i]);
        }

        free(file);
    }

}

char** Load_File_Name(char** file, int* ptr){

    FILE *name_file = NULL;
    int n_row, count;
    char eol;

    name_file = fopen("FileName.txt","r"); //apro il file testo

    if(name_file == NULL){
        printf("Errore: allocazione fallita\n");
        exit (EXIT_FAILURE);
    }

    fscanf(name_file,"%d ", &n_row); //prendo il numero di file esistenti

    *ptr = n_row; //con il passaggio per riferimento, modifico la variabile "n_row" nella funzione precedente

    /*alloco una matrice dinamica per contenere i nomi dei file*/
    if((file = (char**) calloc (n_row,sizeof(char*))) == NULL){
        printf("Errore: allocazione fallita\n");
        exit (EXIT_FAILURE);
    }

    //alloca spazio alla matrice che conterra' i nomi dei file di salvataggio
    for (int i = 0; i < n_row; ++i) {
        if ((file[i] = (char*) calloc(50,sizeof(char))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n_row; ++i) {
        fgets(file[i],50,name_file);
    }

    /*questo algoritmo mi permette di determinare la corretta lunghezza della stringa, reallocare le righe
     * della matrice e scambiare ogni fine rigna il '\n' che viene preso dal file con il terminatore di stringa '\0' */
    for (int i = 0; i < n_row; ++i) {
        count = 0;
        eol = 'a';

        while (eol != '\n'){
            eol = file[i][count++];
        }

        if ((file[i] = (char*) realloc (file[i], count*sizeof(char))) == NULL){
            printf("Errore: allocazione fallita\n");
            exit (EXIT_FAILURE);
        }
        file[i][count-1] = '\0';
    }

    fclose(name_file);

    return file;
}