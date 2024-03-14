
#include <stdio.h>
#include <stdlib.h>

// Apri il file in modalità di lettura binaria
int main () {

int ROWS, COLS;

FILE *file = fopen("i1.bin", "rb");
if (file == NULL) {
    fprintf(stderr, "Errore nell'apertura del file.\n");
    exit(1);
}

// Leggi le dimensioni ROWS e COLS dal file
if (fread(&ROWS, sizeof(int), 1, file) != 1) {
    fprintf(stderr, "Errore nella lettura di ROWS.\n");
    fclose(file);
    exit(1);
}

if (fread(&COLS, sizeof(int), 1, file) != 1) {
    fprintf(stderr, "Errore nella lettura di COLS.\n");
    fclose(file);
    exit(1);
}// Leggi le dimensioni ROWS e COLS dal file
if (fread(&ROWS, sizeof(int), 1, file) != 1) {
    fprintf(stderr, "Errore nella lettura di ROWS.\n");
    fclose(file);
    exit(1);
}

if (fread(&COLS, sizeof(int), 1, file) != 1) {
    fprintf(stderr, "Errore nella lettura di COLS.\n");
    fclose(file);
    exit(1);
}

// Ora hai le dimensioni ROWS e COLS, chiudi il file
fclose(file);


//allocazione dinamica della memoria cdopo aver letto le dimensioni del file 
double **matrice;
matrice = (double **)malloc(ROWS * sizeof(double *));
if (matrice == NULL) {
    fprintf(stderr, "Errore nell'allocazione di memoria per le righe.\n");
    exit(1);
}

for (int i = 0; i < ROWS; i++) {
    matrice[i] = (double *)malloc(COLS * sizeof(double));
    if (matrice[i] == NULL) {
        fprintf(stderr, "Errore nell'allocazione di memoria per le colonne.\n");
        exit(1);
    }
}

//lettura dei dati della matrice 
file = fopen("matrice.bin", "rb");
if (file == NULL) {
    fprintf(stderr, "Errore nell'apertura del file.\n");
    exit(1);
}

// Salta le prime due variabili (ROWS e COLS)
fseek(file, 2 * sizeof(int), SEEK_SET);

// Leggi i dati nella matrice
for (int i = 0; i < ROWS; i++) {
    fread(matrice[i], sizeof(double), COLS, file);
}

fclose(file);

}