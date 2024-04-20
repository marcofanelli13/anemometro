/* Funzione per trovare tempo di volo con input I, Q, la loro dimensione e la frequenza di
campionamento. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double CalcoloTOF0(int I[], int Q[], int n, double fs) {
    // Vettore ampiezza
    double A[n]; 

    // Vettore fase
    double Phi[n];

    for (int i = 0; i < n; i++) {
        A[i] = sqrt(I[i] * I[i] + Q[i] * Q[i]);
        Phi[i] = atan2(Q[i], I[i]);
    }

    // Calcolo del massimo valore dell'ampiezza
    double A_max = A[0];
    int indice_max;

    for (int i = 1; i < n; i++) {
        if (A[i] > A_max) {
            A_max = A[i];
            indice_max = i;
        }
    }

    // Calcolo del punto target
    double target = A_max / 2;

    /* Bisogna eseguire un'interpolazione fra i campioni precedente e successivo al valore target. 
    Questi vengono ricavati calcolando le differenze tra il valore target e gli altri valori 
    presenti in A. Quando si verifica un cambio di segno nelle differenze, si identifica un 
    punto di passaggio attraverso il punto target. La funzione è stata sviluppata in modo tale
    da avere questi punti solo sul fronte di salita del tratto di segnale con picco A_max. */
    double differenze[indice_max];

    // Vengono presi in considerazione solo i campioni prima del picco
    for (int j = 0; j < indice_max; j++) {
        differenze[j] = (A[j] - target);
    }

    int indice_inf = 0;
    int indice_sup = 0;

    for (int j = 0; j < indice_max; j++) {
        if (differenze[j] * differenze[j + 1] < 0) {
            indice_inf = j;
            indice_sup = j + 1;
        }
    }

    // Interpolazione lineare su 256 punti fra il valore precedente e successivo al target
    int interp = 256;
    double xp[interp];
    double yp[interp];
    double yphi[interp];
    double step = (double)(indice_sup - indice_inf) / (interp - 1);

    // Calcolo dei punti di interpolazione ampiezza e fase
    for (int i = 0; i < interp; i++) {
        xp[i] = indice_inf + i * step;
        yp[i] = A[indice_inf] + (xp[i] - indice_inf) * (A[indice_sup] - A[indice_inf]) / (indice_sup - indice_inf);
        yphi[i] = Phi[indice_inf] + (xp[i] - indice_inf) * (Phi[indice_sup] - Phi[indice_inf]) / (indice_sup - indice_inf);
    }

    /* Calcolo della differenza fra i punti interpolati e il target per trovare il valore
    interpolato più vicino al target stesso. */
    double diff_inter[interp];
    int indice_target; // Indice del target sulla linea di interpolazione (fra 0 e 256)

    for (int i = 0; i < interp; i++) {
        diff_inter[i] = fabs(yp[i] - target);
    }

    double target_y = diff_inter[0];

    for (int i = 1; i < interp; i++) {
        if (diff_inter[i] < target_y) {
            target_y = diff_inter[i];
            indice_target = i;
        }
    }

    double Phi0;
    Phi0 = yphi[indice_target];

    // Calcolo dell'indice del target all'interno della misurazione
    double indice_inf_double = (double)indice_inf;
    double indice_target_double = (double)indice_target;
    double campione = (double)(indice_inf_double + indice_target_double / (interp-1));

    const double ODR = 2;
    const double PRE_TRIGGER = 0.0006;

    // Calcolo del Tempo di Volo e della distanza
    double tof0 = (ODR / fs * (campione - 1) - PRE_TRIGGER); 
    double distanza = tof0 * 343;
    /* La distanza tra i sensori è nota quindi possiamo filtrare i valori di distanza 
    che non rientrano in un intervallo specifico dal valore noto. */
const double DIST = 0.052000; 

    // Calcolo dell'intervallo del 10%
    float intervallo = 0.1 * DIST;

    // Calcolo dei limiti dell'intervallo
    float limite_superiore = DIST + intervallo;
    float limite_inferiore = DIST - intervallo;

    // Controlla se il valore è all'interno dell'intervallo
    if (distanza >= limite_inferiore && distanza <= limite_superiore) {
        printf("La distanza tra i sensori è: %f,\n", distanza);  // Il valore è all'interno dell'intervallo
    } else {
        printf("La distanza risultante è errata"); // Il valore è al di fuori dell'intervallo
    }

    printf("Time of flight: %f\n", tof0);
    printf("Fase: %f\n", Phi0);

    return tof0;
}

int main() {
    int I[] = {0,5,11,10,-177,-629,-516,639,1636,1621,1198,905,437,-802,-3004,-5579,-7431,-7976,-7706,-7623,-8276,-9136,-9090,-7757,-5817,-4151,-3108,-2519,-2139,-1853,-1597,-1302,-940,-544,-179,79,190,154};
    int Q[] = {0,1,23,99,383,1320,3041,4560,4777,3976,2895,1261,-1694,-6133,-11071,-14687,-15748,-14553,-12213,-9592,-6899,-3987,-1080,1150,2288,2536,2337,2044,1840,1754,1741,1691,1537,1352,1109,747,404,155};
    int n = sizeof(I)/sizeof(I[0]);
    double fs = 175993.0;
    double tof0 = CalcoloTOF0(I, Q, n, fs);

    return 0;
}