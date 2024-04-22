#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <math.h>
#include <time.h>

#define BUFFER_SIZE 256
#define PI 3.14159265358979323846

double CalcoloTOF0(int I[], int Q[], int n, int fs) {
    // Vettore ampiezza
    double A[n]; 

    for (int i = 0; i < n; i++) {
        A[i] = sqrt(I[i] * I[i] + Q[i] * Q[i]);
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
    double step = (double)(indice_sup - indice_inf) / (interp - 1);

    // Calcolo dei punti di interpolazione
    for (int i = 0; i < interp; i++) {
        xp[i] = indice_inf + i * step;
        yp[i] = A[indice_inf] + (xp[i] - indice_inf) * (A[indice_sup] - A[indice_inf]) / (indice_sup - indice_inf);
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

    // Calcolo dell'indice del target all'interno della misurazione
    double indice_inf_double = (double)indice_inf;
    double indice_target_double = (double)indice_target;
    double campione = (double)(indice_inf_double + indice_target_double / (interp-1));
    const int multiplo=1000000;
    const double ODR = 2*multiplo;
    const double PRE_TRIGGER = 0.0006*multiplo;

    // Calcolo del Tempo di Volo e della distanza
    double tof0 = ODR / fs * (campione - 1) - PRE_TRIGGER;
    //double tof_1000 = (ODR*1000) / fs * (campione - 1) - (PRE_TRIGGER*1000);
    double distanza = tof0 * 343;
    //printf("TOF*1000: %f\n", tof_1000);
    //printf("Time of flight: %f\n", tof);
   // printf("Distanza tra i sensori: %f\n", distanza);

    return tof0;
}

double CalcoloPhi0(int I[], int Q[], int n, int fs) {
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

    // Calcolo dei punti di interpolazione
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

    return Phi0;
}

int main() {
    char risposta,risposta_cal;
    int calibrazione=2;
    
    FILE *file; // Puntatore al file
    time_t current_time;
    char* c_time_string;
    
    int porta_com;
    char porta_com_stringa[4];

    // Chiedi all'utente di inserire il numero della porta COM
    printf("Inserisci il numero della porta COM da aprire: ");
    scanf("%d", &porta_com);
    sprintf(porta_com_stringa, "COM%d", porta_com);
    printf("I valori verranno letti dalla Porta %s\n", porta_com_stringa);


    time_t current_time_nf;
    struct tm *timeinfo_nf;
    char timestamp_nf[15]; // La lunghezza della stringa, includendo il terminatore null

    // Ottenere il tempo corrente
    time(&current_time_nf);
    timeinfo_nf = localtime(&current_time_nf);

    // Formattare il timestamp
    strftime(timestamp_nf, sizeof(timestamp_nf), "%Y%m%d%H%M%S", timeinfo_nf);

    // Stampare il timestamp
    
    strcat(timestamp_nf, ".txt");

    // Apre il file in modalità scrittura ("w")
    file = fopen(timestamp_nf, "w");
    
    printf("Vuoi fare la calibrazione? (si/no): ");
    scanf(" %c", &risposta); // Attenzione al whitespace prima di %c per ignorare eventuali spazi

    if (risposta == 's' || risposta == 'S') {
        printf("Per fare la calibrazione copri i sensori con un coperchio e poi resetta il dispositivo. Digita 'i' per iniziare la calibrazione.\n");
        scanf(" %c", &risposta); // Attenzione al whitespace prima di %c per ignorare eventuali spazi
        if (risposta == 'i' || risposta == 'I') {
            calibrazione=1;    
        } else {
        printf("Risposta non valida.\n");
        // Qui puoi decidere come gestire una risposta non valida
        
        }
        // Qui inserisci il codice per continuare l'esecuzione
    } else if (risposta == 'n' || risposta == 'N') {
        printf("Hai scelto di non calibrare il sensore. Resetta il dispositivo e inizia la misura.\n");
        calibrazione=0;
        // Qui inserisci il codice per uscire dal programma
    } else {
        printf("Risposta non valida.\n");
        // Qui puoi decidere come gestire una risposta non valida
    }


    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };

    // Apri la porta seriale
    hSerial = CreateFile(porta_com_stringa, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Impossibile aprire la porta seriale. Errore %ld\n", GetLastError());
        fprintf(file, "Impossibile aprire la porta seriale. Errore %ld\n", GetLastError());
        return 1;
    }

    // Imposta i parametri della porta seriale
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Impossibile ottenere i parametri della porta seriale. Errore %ld\n", GetLastError());
        fprintf(file, "Impossibile ottenere i parametri della porta seriale. Errore %ld\n", GetLastError());
        CloseHandle(hSerial);
        return 1;
    }
    dcbSerialParams.BaudRate = 1000000;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Impossibile impostare i parametri della porta seriale. Errore %ld\n", GetLastError());
        fprintf(file, "Impossibile impostare i parametri della porta seriale. Errore %ld\n", GetLastError());
        CloseHandle(hSerial);
        return 1;
    }

    // Imposta i timeout per la porta seriale
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        fprintf(stderr, "Impossibile impostare i timeout della porta seriale. Errore %ld\n", GetLastError());
        fprintf(file, "Impossibile impostare i timeout della porta seriale. Errore %ld\n", GetLastError());
        CloseHandle(hSerial);
        return 1;
    }

    // Lettura e stampa dei dati dal sensore
    char buf[BUFFER_SIZE];
    DWORD bytesRead;
    char receivedChar;
    int bufIndex = 0;
    BOOL endOfLine = FALSE; // Variabile di controllo per indicare la fine della riga
    int Sensore=-1;
    int Porta=-2;
    int Frequenza;
    int Nsamples=-999;
    int Q[105]={0}; // Matrice per i campioni
    int I[105]={0}; // Matrice per i campioni
    int sampleIndex = -99;
    int Misure_0=0, Misure_1=0;
    int Coppia;
    double tof_basso=250.0;//usec
    double tof_alto=350.0;//usec
    double tof_0=0, tof_1=0, phi_0=0, phi_1=0, tof_corrente=0, phi_corrente=0, tof_finale_0, tof_finale_1, tof_cal0, tof_cal1, phi_finale_0, phi_finale_1, phi_cal0, phi_cal1;
    double delta_vento=0;
    double correzione_tof0[10]={0};
    double correzione_tof1[10]={0};
    double correzione_phi0[10]={0};
    double correzione_phi1[10]={0};
    double correzione_vento[10]={0};
    int cont_0=0;
    int cont_1=0;
   
    int cont_cal=0;
    double vento, vento_cal, vento_correzione;
    double suono, suono_cal, suono_correzione;
    double temp;
    double distanza=96; //mm 
    while (1) {
        if (!ReadFile(hSerial, &receivedChar, sizeof(receivedChar), &bytesRead, NULL)) {
            fprintf(stderr, "Errore durante la lettura dalla porta seriale. Errore %ld\n", GetLastError());
            fprintf(file, "Errore durante la lettura dalla porta seriale. Errore %ld\n", GetLastError());
            break;
        }
        if (bytesRead > 0) {
            if (receivedChar == '\n') {
                // Fine della riga, termina la stringa e stampa
                buf[bufIndex] = '\0';
                //printf("Riga ricevuta: %s\n", buf);
                // Analizza la riga ricevuta qui
                if (strstr(buf, "Ricezione") != NULL) {
                    if (Misure_0 >0 && Misure_1>0){ //significa che ho fatto già la doppia misura
                        tof_finale_0=tof_0/Misure_0;
                        tof_finale_1=tof_1/Misure_1;
                        phi_finale_0=phi_0/Misure_0;
                        phi_finale_1=phi_1/Misure_1;
                        //printf("calcolo velocità del vento e del suono");
                        if (calibrazione==1){ 
                            //vento_cal=500*(distanza/tof_finale_0 - distanza/tof_finale_1);
                            //suono_cal=500*(distanza/tof_finale_0 + distanza/tof_finale_1);
                            correzione_tof0[cont_cal]=tof_finale_0;
                            correzione_tof1[cont_cal]=tof_finale_1;
                            correzione_phi0[cont_cal]=phi_finale_0;
                            correzione_phi1[cont_cal]=phi_finale_1;
                            //correzione_vento[cont_cal]=vento_cal;
                            //vento_correzione=vento_correzione+vento_cal;
                            //suono_correzione=suono_correzione+suono_cal;
                            Misure_0=0;
                            Misure_1=0;
                            tof_0=0;
                            tof_1=0;
                            phi_0=0;
                            phi_1=0;
                            cont_cal++;
                            //fprintf(file,"-------------------cal tof_finale_0: %f\n", tof_finale_0);
                            //fprintf(file,"-------------------cal tof_finale_1: %f\n", tof_finale_1);
                            //fprintf(file,"conta cal: %d\n", cont_cal);
                            if(cont_cal==10){
                                calibrazione=0;
                                for (int p = 0; p < 10; p++) {
                                    tof_cal0 = tof_cal0+correzione_tof0[p];
                                    tof_cal1 = tof_cal1+correzione_tof1[p];
                                    phi_cal0 = phi_cal0+correzione_phi0[p];
                                    phi_cal1 = phi_cal1+correzione_phi1[p];
                                    printf("phi0 %f e phi1 %f\n", correzione_phi0[p], correzione_phi1[p]);
                                    //delta_vento = delta_vento + correzione_vento[p];
                                }
                                // tempi di volo mediati fra le 10 misurazioni prese dalla calibrazione
                                tof_cal0 = tof_cal0/10;
                                tof_cal1 = tof_cal1/10;
                                phi_cal0 = phi_cal0/10;
                                phi_cal1 = phi_cal1/10;
                                //delta_vento=delta_vento/10;
                                printf("tof0 %f e tof1 %f\n", tof_cal0, tof_cal1);
                                fprintf(file,"La calibrazione è terminata!\n");
                                //printf("Vel: %f\n", vento_correzione);
                                //printf("Suono: %f\n", suono_correzione);
                                //fprintf(file,"Delta TOF: %f\n", delta_tof);
                                //fprintf(file,"Delta Vento: %f\n", delta_vento);

                            }
                        }else if  (calibrazione==0){
                            //tof_finale_1=tof_finale_1-delta_tof;
                            vento=500*(distanza/tof_finale_0 - distanza/tof_finale_1);
                            vento=vento-delta_vento;
                            suono=500*(distanza/tof_finale_0 + distanza/tof_finale_1);
                            //printf("-----------distanza: %f\n", distanza);
                            //fprintf(file,"-------------------tof_finale_0: %f\n", tof_finale_0);
                            //fprintf(file,"-------------------tof_finale_1: %f\n", tof_finale_1);
                            //printf("----------------Vel: %f\n", vento);
                            //printf("--------------Suono: %f\n", suono);
                            Misure_0=0;
                            Misure_1=0;
                            tof_0=0;
                            tof_1=0;
                             // Ottiene la data e l'ora corrente
                            current_time = time(NULL);
                            // Converte la data e l'ora corrente in una stringa leggibile
                            c_time_string = ctime(&current_time);
                            temp=suono*suono;
                            temp=temp/1.4;
                            temp=temp/287.05;
                            temp=temp-273.15;
                            // Scrive la data e l'ora nel file
                            //if(vento<20 && vento >-20){
                                fprintf(file, "Timestamp: %s", c_time_string);
                                fprintf(file, "Vento: %f, Suono: %f, Temperatura sonica: %f\n",vento,suono,temp);
                                printf("Vento: %f m/s\n",vento);
                            //}
                        }
                    }
                    Sensore = atoi(&buf[bufIndex - 1]);
                    //printf("Sensore: %d\n", Sensore);
                    
                }
                if (strstr(buf, "Freqq") != NULL) {
                    
                    if (sscanf(buf + strlen("Freqq "), "%d", &Frequenza) == 1) {
                        //printf("Frequenza: %d\n", Frequenza);    
                    } 
                }

                if (strstr(buf, "Porta") != NULL) {
                // Se trovi la parola, cerca il numero successivo
                    Nsamples=0;
                    if (sscanf(buf + strlen("Porta "), "%d", &Porta) == 1) {
                        // Salva e stampa il numero trovato
                        //printf("Valore della porta: %d\n", Porta);
                        // Qui puoi fare altro con il valore, come salvarlo su file
                    }
                }
                if (strstr(buf, "I/Q samples copied") != NULL) {
                // Se trovi la parola, cerca il numero successivo
                    if(Sensore==Porta){
                        
                        if ( sscanf(buf + strlen("I/Q samples copied "), "%d", &Nsamples) == 1) {
                            // Salva e stampa il numero trovato
                            //printf("N samples: %d\n", Nsamples);
                            sampleIndex=0;
                            if (Sensore==0){
                                Misure_0++;
                            }else if(Sensore==1){
                                Misure_1++;
                            }
                            
                            //fprintf(file,"Misure 0: %d\n", Misure_0);
                            //fprintf(file,"Misure 1: %d\n", Misure_1);
                            // Qui puoi fare altro con il valore, come salvarlo su file
                        }
                    }
                }
                if (strstr(buf, ",") != NULL) {
                    if(Sensore==Porta){
                   
                        int val1,val2;
                        
                        if (sscanf(buf, "%d,%d", &val1, &val2) == 2 && sampleIndex<=Nsamples) {
                            Q[sampleIndex] = val1;
                            I[sampleIndex] = val2;
                            //fprintf(file,"%d,%d\n", Q[sampleIndex], I[sampleIndex]);
                            //printf("%d,%d,%d,%d\n", Nsamples,sampleIndex,Q[sampleIndex], I[sampleIndex]);
                            
                            sampleIndex++;
                        }
                        
                    }
                }
                
                if(sampleIndex==Nsamples && Sensore==Porta ){
                    //fprintf(file,"d-Misure 0: %d\n", Misure_0);
                    //fprintf(file,"d-Misure 1: %d\n", Misure_1);
                    //chiamo la funzione di Marco e Ludovico  che calcola tof_corrente
                    int n = sizeof(I) / sizeof(I[0]);
                    tof_corrente = CalcoloTOF0(I, Q, n, Frequenza);
                    phi_corrente = CalcoloPhi0(I, Q, n, Frequenza);
                    //scarto misure errate
                    if(Sensore==0){
                        if(tof_corrente>tof_alto || tof_corrente<tof_basso){
                            
                            Misure_0--;
                            //fprintf(file,"Misura scartata: %f\n", tof_corrente);
                            //fprintf(file,"Non considero Misura 0");
                            tof_corrente=0;
                        }
                    }else if(Sensore==1){
                        if(tof_corrente>tof_alto || tof_corrente<tof_basso){
                            //fprintf(file,"tof alto: %f\n", tof_alto);
                            //fprintf(file,"tof basso: %f\n", tof_basso);
                            Misure_1--;
                            //fprintf(file,"Misura scartata: %f\n", tof_corrente);
                            //fprintf(file,"Non considero Misura 1");
                            tof_corrente=0;
                        }
                    }
                    /*for(int j=0;j<50;j++){
                        //printf("%d,%d\n", Q[j], I[j]);
                       
                    }*/
                    //fprintf(file,"-------tof: %f------------\n", tof_corrente);
                    sampleIndex=-99;
                    Nsamples=-999;
                    if(Sensore==0){
                        tof_0=tof_0+tof_corrente;
                        phi_0=phi_0+phi_corrente;
                        cont_0++;
                    }else if(Sensore==1){
                        tof_1=tof_1+tof_corrente;
                        phi_1=phi_1+phi_corrente;
                        cont_1++;
                    }
                }
                

                // Resetta l'indice del buffer per la prossima riga
                bufIndex = 0;
            } else {
                // Aggiungi il carattere al buffer
                buf[bufIndex++] = receivedChar;
                // Controlla se il buffer è pieno, se lo è, termina la stringa e stampa
                if (bufIndex >= BUFFER_SIZE - 1) {
                    buf[bufIndex] = '\0';
                    printf("Riga ricevuta (buffer pieno): %s\n", buf);
                    // Analizza la riga ricevuta qui
                    // Resetta l'indice del buffer per la prossima riga
                    bufIndex = 0;
                }
            }
        }
    }

    // Chiudi la porta seriale
    CloseHandle(hSerial);
    
    // Chiude il file
    fclose(file);

    return 0;
}
