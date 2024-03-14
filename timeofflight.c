 /*Time of Flight calculation */
 /*funzione per il calcolo del ToF del segnale
 Gli input sono i vettori I e Q e la frequenza dei campionamenti.
 PROCEDIMENTO LOGICO:
 Dai dati di I e Q calcolare l'ampiezza (presumibilmente fare un ciclo sugli array di dati che arrivano dal sensore ma ancora non è chiaro il tipo di dati che escono fuori ) 
 Su tutti i valori di ampiezza per ogni gruppo di campioni va trovato il massimo (picco), la funzione max non è presente su C, 
 sul valore picco/2 individuarne l'indice del campionamento.

 USARE METODO DI MARIO PER LA IDENTIFICAZIONE DI PICCO/2 CON LE DIFFERENZE DEI VALORI ASSOLUTI PER TROVARE GLI INDICI DELL'INTERVALLO SU CUI FARE L'INTERPOLAZIONE.
 I valori di I e Q dovrebbero essere input in formato array (esistono i dati in formato matrice? semplicemente array con dimensioni maggiorni di 1?) ,
  su cui effettuare il calcolo dell'Ampiezza su ogni valori valore, dai valori di ampiezza trovare il massimo PICCO.
  trovare il valore picco/2 ---> metodo delle differenze di Mario e trovare i campioni su cui fare interpolazione
  
  COME SI FA UN INTERPOLAZIONE IN C?
  
  IL NUMERO DI SAMPLE CHE HO FA PARTE DEGLI INPUT? non abbiamo ancora trovato un modo per collegare i samples alla frequenza di campionamento
  Il max_range non ci serve
  che cosa è ODR ? (ODR=output data range)
   */

/*DATI E PARTI DI CODICE MATLAB DA TRADURRE:

c=343.4; %velocità del suono [m/s] @ 25°C 
d=52; %distanza tra i sensori in mm

Calcolo dell'ampiezza 
(qui ho tutto in forma matriciale)
for i=1:ns
for j=1:dim 
    A0(i,j)=sqrt(I0mat(i,j)^2+Q0mat(i,j)^2);
    A1(i,j)=sqrt(I1mat(i,j)^2+Q1mat(i,j)^2);

end
end
IL CALCOLO DEL TARGET E DEL TOF VA ITERATO PER OGNI MISURAZIONE E POI RIPETUTO PER I DATI DI 1
SCRIPT DI LUDOVICO PER TROVARE IL CAMPIONE TARGET 
    % Trovare gli indici (ovvero il numero di campionamento), l'ampiezza, l'istante di tempo e la posizione di picchi e valli
    [picco0, indice_picco0] = max(ampiezza0);
    posizione_picco0 = spazio(indice_picco0);
    [valli0, indici_valli0] = findpeaks(-ampiezza0);
    indice_valle0 = max(indici_valli0(indici_valli0 < indice_picco0));
    valle0 = ampiezza0(indice_valle0);
    posizione_valle0 = spazio(indice_valle0);
    if isempty(indice_valle0)
        indice_valle0 = 0;
        valle0 = 0;
        posizione_valle0 =0;
    end

    %indici_intervallo = posizione_valle:posizione_picco;
    indici_intervallo0 = (spazio >= posizione_valle0) & (spazio <= posizione_picco0);

    target0 = picco0/2;
    target_inf0 = max(ampiezza0(indici_intervallo0 & ampiezza0 < target0));
    indice_target_inf0 = find(ampiezza0 == target_inf0);
    posizione_target_inf0 = spazio(indice_target_inf0);
    target_sup0 = min(ampiezza0(indici_intervallo0 & ampiezza0 > target0));
    indice_target_sup0 = find(ampiezza0 == target_sup0);
    posizione_target_sup0 = spazio(indice_target_sup0);
    
    x_intermedi0 = linspace(posizione_target_inf0, posizione_target_sup0, 256);
    
    % Calcola i valori interpolati delle ampiezze per i punti intermedi
    y_interpolati0 = interp1([posizione_target_inf0, posizione_target_sup0], [target_inf0, target_sup0], x_intermedi0, 'linear');
    
    % Trova il valore interpolato più vicino al valore desiderato
    [valore_vicino0, indice0] = min(abs(y_interpolati0 - target0));
    
    % Valore di x corrispondente al valore interpolato più vicino
    x_target0 = x_intermedi0(indice0);
    %posizione_target = spazio(x_intermedi);

SCRIPT DI MARIO 
% Calcola la metà del massimo valore
            meta_massimo = massimo_valore / 2;

            differenze = abs(vettore(1:indice_massimo) - meta_massimo);

            % Trova l'indice dell'elemento con la minima differenza assoluta
            [~, indice_punto_vicino] = min(differenze);
            
            % Valore corrispondente al punto più vicino
            valore_punto_vicino = vettore(indice_punto_vicino);
            
            if(valore_punto_vicino<meta_massimo)
               punto_prima=indice_punto_vicino;
               punto_dopo=indice_punto_vicino+1;
            else
               punto_prima=indice_punto_vicino-1;
               punto_dopo=indice_punto_vicino;
            end
 VA ANCORA FATTA L'INTERPOLAZIONE DEI VALORI 

 CALCOLO DEL TEMPO DI VOLO E DELLA VELOCITA DEL VENTO 
            tempo_di_volo01 = 8 * (x_target0 - 1) / fs0 - 0.0006;
                tempo_di_volo10 = 8 * (x_target1 - 1) / fs1 - 0.0006;
                distanza01 = c * tempo_di_volo01; % 
                distanza10 = c * tempo_di_volo10; % 
                vel_vent = d/2*(1/tempo_di_volo01 - 1/tempo_di_volo10);
           

*/

#include <math.h> 
#include <stdio.h>
// #include "funzioniTOF.h"

//definisco variabili e costanti globali, forse è meglio definire tutto all'interno della funzione 
const double C = 343.6; //velocità del suono @ 25°C [m/s]
const double DISTANZA = 0.052; //distanza tra i sensori [m]
int ns; //numero di campioni per ogni misurazione 
const int ODR=2; //è 2 nello short range mode, mentre è 8 nel mode standard (ODR=output data range)
const double delay=0.0006; //anticipo nell'accensione del ricevitore rispetto al trigger 

// double ToFcalculation(int i, int q, int fs )
// {


// }

int main()
{

int numRighe=103, numCol0=31, numCol1=33;
double **i0=leggiMatriceDaFile("i0.bin", &numRighe, &numCol0);
if (i0 == NULL){
    fprintf(stderr, "Errore nella lettura della matrice ");
    return 1; 
}


    printf("Matrice letta da file:\n");
    stampaMatrice(i0, numRighe, numCol0);

    liberaMatrice(i0, numRighe);


 
return 0;
}