/*codice per estrarre i dati dalle variabili matlab*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main()
{
  
//   //ESTRAZIONE DELLE FS
//   int ROWF=1, COLF=2;
//   int **fs;
//   fs = (int**)malloc(ROWF * sizeof(int *));
//   if (fs == NULL) {
//     fprintf(stderr, "Errore nell'allocazione di memoria per le righe.\n");
//     exit(1);

//   }

//   for (int i = 0; i < ROWF; i++)   {
//     fs[i] = (int*)malloc(COLF * sizeof(int));
//     if (fs[i] == NULL) {
//         fprintf(stderr, "Errore nell'allocazione di memoria per le colonne. \n");
//         exit(1);
//     }

//   }

    // FILE *filefs;
    // filefs = fopen("fs.bin", "rb");
    // if (filefs == NULL) {
    //     fprintf(stderr, "Errore nell'apertura del file.\n");
    //     exit(1);
    // }

    // // Leggere la fs dal file
    // for (int i = 0; i < ROWF; i++) {
    //     fread(fs[i], sizeof(double), COLF, filefs);
    // }

    // fclose(filefs);

    //  //Stampare la fs per verifica
    // printf("fs letta da file:\n");
    // for (int i = 0; i < ROWF; i++) {
    //     for (int j = 0; j < COLF; j++) {
    //         printf("%lf ", fs[i][j]);
    //     }
    //     printf("\n");
    // }
//DICHIARAZIONE DEI VALORI DI FS
int fs0=177651, fs1=176148;
// printf("fs0=%d,fs1=%d.\n", fs0, fs1);

    //ESTRAZIONE DEI DATI DI I0
int ROWS=102, COLS0=31, COLS1=32 ;
    double **i0;
    i0 = (double **)malloc(ROWS * sizeof(double *));
    if (i0 == NULL) {
        fprintf(stderr, "Errore nell'allocazione di memoria per le righe.\n");
        exit(1);
    }

    for (int i = 0; i < ROWS; i++) {
        i0[i] = (double *)malloc(COLS0 * sizeof(double));
        if (i0[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione di memoria per le colonne.\n");
            exit(1);
        }
    }

    FILE *filei0;
    filei0 = fopen("i0.bin", "rb");
    if (filei0 == NULL) {
        fprintf(stderr, "Errore nell'apertura del file.\n");
        exit(1);
    }

    // Leggere la i0 dal file
    for (int i = 0; i < ROWS; i++) {
        fread(i0[i], sizeof(double), COLS0, filei0);
    }

    fclose(filei0);

    //Stampare la i0 per verifica
    printf("i0 letta da file:\n");
    for (int i = 0; i < COLS0; i++) {
        for (int j = 0; j < ROWS; j++) {
            printf("%lf\n ", i0[i][j]);
        }
        printf("\n");
    }



    
//ESTRAZIONE DEI DATI DI Q0
    double **q0;
    q0 = (double **)malloc(ROWS * sizeof(double *));
    if (q0 == NULL) {
        fprintf(stderr, "Errore nell'allocazione di memoria per le righe di q0.\n");
        exit(1);
    }

    for (int i = 0; i < ROWS; i++) {
        q0[i] = (double *)malloc(COLS0 * sizeof(double));
        if (q0[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione di memoria per le colonne di q0 .\n");
            exit(1);
        }
    }

    FILE *fileq0;
    fileq0 = fopen("q0.bin", "rb");
    if (fileq0 == NULL) {
        fprintf(stderr, "Errore nell'apertura del file.\n");
        exit(1);
    }

    // Leggere la q0 dal file
    for (int i = 0; i < ROWS; i++) {
        fread(q0[i], sizeof(double), COLS0, fileq0);
    }

    fclose(fileq0);

    // Stampare la i0 per verifica
    // printf("q0 letta da file:\n");
    // for (int i = 0; i < ROWS; i++) {
    //     for (int j = 0; j < COLS0; j++) {
    //         printf("%lf ", q0[i][j]);
    //     }
    //     printf("\n");
    // }

    
    //ESTRAZIONE DEI DATI DI I1

    double **i1;
    i1 = (double **)malloc(ROWS * sizeof(double *));
    if (i1 == NULL) {
        fprintf(stderr, "Errore nell'allocazione di memoria per le righe di i1.\n");
        exit(1);
    }

    for (int i = 0; i < ROWS; i++) {
        i1[i] = (double *)malloc(COLS0 * sizeof(double));
        if (i1[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione di memoria per le colonne di i1.\n");
            exit(1);
        }
    }

    FILE *filei1;
    filei1 = fopen("i1.bin", "rb");
    if (filei1 == NULL) {
        fprintf(stderr, "Errore nell'apertura del file di i1.\n");
        exit(1);
    }

    // Leggere la i1 dal file
    for (int i = 0; i < ROWS; i++) {
        fread(i1[i], sizeof(double), COLS1, filei1);
    }

    fclose(filei1);

    // Stampare la i1 per verifica
    // printf("i1 letta da file:\n");
    // for (int i = 0; i < ROWS; i++) {
    //     for (int j = 0; j < COLS1; j++) {
    //         printf("%lf ", i1[i][j]);
    //     }
    //     printf("\n");
    // }



    
//ESTRAZIONE DEI DATI DI Q1
    double **q1;
    q1 = (double **)malloc(ROWS * sizeof(double *));
    if (q1 == NULL) {
        fprintf(stderr, "Errore nell'allocazione di memoria per le righe di q1.\n");
        exit(1);
    }

    for (int i = 0; i < ROWS; i++) {
        q1[i] = (double *)malloc(COLS1 * sizeof(double));
        if (q1[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione di memoria per le colonne di q1 .\n");
            exit(1);
        }
    }

    FILE *fileq1;
    fileq1 = fopen("q0.bin", "rb");
    if (fileq1 == NULL) {
        fprintf(stderr, "Errore nell'apertura del file.\n");
        exit(1);
    }

    // Leggere la q1 dal file
    for (int i = 0; i < ROWS; i++) {
        fread(q1[i], sizeof(double), COLS1, fileq0);
    }

    fclose(fileq1);

    // Stampare la i1 per verifica
    // printf("q1 letta da file:\n");
    // for (int i = 0; i < ROWS; i++) {
    //     for (int j = 0; j < COLS0; j++) {
    //         printf("%lf ", q1[i][j]);
    //     }
    //     printf("\n");
    // }


   

/*CODICE DA METTERE DENTRO LA FUNZIONE*/
    

double a0[ROWS][COLS0];
double a1[ROWS][COLS1];

    for (int i =0; i < ROWS; i++) {
        for (int j = 0; j < COLS0; j++) {
            a0[i][j] = sqrt(i0[i][j] * i0[i][j] + q0[i][j] * q0[i][j]);
            a1[i][j] = sqrt(i1[i][j] * i1[i][j] +q1[i][j] * q1[i][j]);
        }
    }
printf("Matrice a0:\n");
for (int i = 0; i < COLS0; i++) {
    for (int j = 0; j < ROWS; j++) {
        printf("%lf\n", a0[i][j]);

    }
    printf("\n");
}

printf("Matrice a1:\n");
for (int i = 0; i < COLS0; i++) {
    for (int j = 0; j < ROWS; j++) {
        printf("%lf\n", a1[i][j]);
    }
    printf("\n");
}


 /*PARTE DA TENERE ALLA FINEEE PER LIBERARE LA MEMORIA !!!!*/
 //liberare la memoria allocata per ogni riga
    for (int i = 0; i < ROWS; i++)  {
        free(i0[i]);
    }

    for (int i = 0; i < ROWS; i++)  {
        free(q0[i]);
    }


// Liberare la memoria allocata per ogni riga
    for (int i = 0; i < ROWS; i++) {
        free(q1[i]);


    }

 // Liberare la memoria allocata per ogni riga
    for (int i = 0; i < ROWS; i++) {
        free(i1[i]);
    }
    // Liberare la memoria allocata per l'array di puntatori
    free(i0);
    free(q0);
    free(i1);
    free(q1);
   
    // free(fs);


    return 0;
    

}

/*metti nel main questo, q0 e i1 e q1 (poi prova a farci unna funzione per l'estrazione dei dati) e inizia a lavorare alla funzione per il TOF */