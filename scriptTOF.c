#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int i0[] = {0,
-4,
-18,
-27,
11,
497,
1753,
3062,
3720,
3672,
3038,
2296,
1782,
1415,
1128,
893,
676,
497,
367,
272,
218,
185,
148,
84,
31,
52,
52,
4,
11,
28,
-3,
-23,
-8,
-14,
-66,
-72,
-22,
-14,
-27,
-26,
-28,
-10,
5,
8,
33,
32,
5,
20,
41,
36,
22,
8,
17,
34,
22,
-12,
-12,
18,
10,
-17,
0,
5,
-2,0
-27,
-234,
-1112,
-3189,
-6768,
-11213,
-14837,
-16035,
-14499,
-11188,
-7285,
-3691,
-1266,
-501,
-1046,
-2040,
-2765,
-2959,
-2682,
-2105,
-1415,
-857,
-719,
-1139,
-2083,
-3336,
-4419,
-4909,
-4748,
-4127,
-3254,
-2183,
-926,
257,
938,
1028,
857,
620,
333,
102};

int q0[] = {0,
7,
47,
86,
133,
309,
445,
446,
664,
986,
976,
734,
479,
248,
123,
100,
73,
24,
-34,
-75,
-49,
6,
5,
-37,
-42,
-16,
-15,
-44,
-42,
-19,
-33,
-51,
-21,
33,
37,
4,
8,
26,
20,
14,
-7,
-24,
7,
9,
-22,
3,
23,
-1,
-13,
3,
35,
34,
8,
8,
14,
17,
6,
-23,
-23,
-1,
-8,
-16,
14,
2,
-178,
-790,
-2251,
-4574,
-7019,
-8514,
-8366,
-6697,
-4073,
-910,
2410,
5210,
6790,
6992,
6248,
5230,
4399,
3828,
3484,
3276,
3062,
2799,
2458,
1980,
1371,
716,
57,
-563,
-961,
-960,
-536,
228,
1103,
1753,
2019,
1993,
1808,
1544,
1297};

int main (){


    int dimensione = 102; //lunghezza degli array di dati di i e q su cui effetturare il ciclo for 
 double a[dimensione]; //inizializzazione dell'array delle ampiezze 


for ( int i = 0; i < dimensione; i++ ) {
    a[i] = sqrt (i0[i] * i0[i] + q0[i] * q0[i]);
    // printf("vettore delle ampiezze: %f\n", a[i]);
} //array con i valori di ampiezza del segnale 


    double massimo = a[0];
    
    int indice_max;

    // Itera attraverso gli elementi del vettore per trovare il massimo
    for (int i = 1; i < dimensione; i++) {
        if (a[i] > massimo) {
            massimo = a[i];
            indice_max = i; //la numerazione parte da 0 quindi il campione sarà i+1
        }
    } 
    // return indice_max;
     printf("l'indice del max:%d\n", indice_max);//troviamo il massimo tra i valori di ampiezza
    double target = massimo / 2; //il nostro punto target è la metà del massimo 
    
    printf("target: %f\n", target);

    

   
    
    double differenze[indice_max];
    for (int j = 0; j < indice_max; j++){
        differenze[j] = ( a[j] - target);
        // printf("vettore differenze: %f\n", differenze[j]); 

    }
    int indice_inf = 0; //indice inferiore dell'intervallo su cui effettuare l'interpolazione 
    int indice_sup = 0; //indice superiore dell'intervallo su cui effetturare l'interpolazione

    for ( int j = 0; j < indice_max; j++){
        if (differenze[j] * differenze[j+1] < 0){
            indice_inf = j;
            indice_sup = j+1;

        }
        
    }

// printf("gli indici dell' intervallo sono %d e %d\n", indice_inf, indice_sup);


//   differenze = abs(vettore(1:indice_massimo) - meta_massimo);

//             % Trova l'indice dell'elemento con la minima differenza assoluta
//             [~, indice_punto_vicino] = min(differenze);
            
//             % Valore corrispondente al punto più vicino
//             valore_punto_vicino = vettore(indice_punto_vicino);
            
//             if(valore_punto_vicino<meta_massimo)
//                punto_prima=indice_punto_vicino;
//                punto_dopo=indice_punto_vicino+1;
//             else
//                punto_prima=indice_punto_vicino-1;
//                punto_dopo=indice_punto_vicino;
//             end
     
double num = 256; //numero di punti di interpolazione 

double xp[256]; //vettore contenente i punti di interpolazione 
double yp[256]; 
double step = (indice_sup - indice_inf) / (num - 1);
    for (int i = 0; i < num; i++) {
        xp[i] = indice_inf + i * step;
           
        yp[i]= a[indice_inf] + (xp[i] - indice_inf) * (a[indice_sup]- a[indice_inf]) / (indice_sup - indice_inf); //interpolazione lineare 
        printf("intervallo di interpolazione: %f\n", yp[i]);  
       

    }
printf("%f\n %f\n %f\n %f\n", a[indice_inf], yp[0], a[indice_sup], yp[255]);


return 0; 
}