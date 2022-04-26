// Author: Daniel Schindler <danielmschindler1@googlemail.com>
// Disclaimer: This source underlies the CERNrr-licence and mustn't be distributed

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
#include <time.h>
#define ADSIZE 10 // default 10
#define MAXFEUER 1
#define Bildanzahl 4
#define Bildgroesse 3
#define PLUS 0
using namespace std;
void update(int[][ADSIZE],int[],int[],int,int);
void vergleich(int[],int[],int,int*);

// Das Programm verfolgt die Adjazenzmatrizen und bestimmt die Attraktoren. Wenn der
// exakte Fehler für alle Attraktoren 0 ist, wird die Seed in eine Datei geschrieben.
// Im Augenblick werden die Bilder jeweils 3mal (=bildwiederholung) durchlaufen und die
// neuronalen Aktivitäten immer zu Beginn randomisiert.

int main() {
  ofstream outfile( "./Seeds.txt", ios_base::out);
  //
    int inhibitory=2;  // Anzahl inhibitorischer, interner Neuronen DEFAULT=2
  int internal_vertices=ADSIZE-2-Bildgroesse;  // Anzahl der internen Vertizes des Graphen
  int iseed;          // Seed fuer den Zufallszahlgenerator
    int Anzahl_Seeds=100;
    int Seedstart=0;
  int probability=50; // Besetzungswahrscheinlichkeit der Linien in der Ad-Matrix
    int active_neurons=ADSIZE-Bildgroesse;    //  =6 im normalen Programm
    int Zwei[10]={1,2,4,8,16,32,64,128,256,512};  // Potenzen von 2 für Zustandsangaben; Dimension=interne Neuronen (ADSIZE-3)
    int Basins[Zwei[ADSIZE-3] + 1];    // (65 für ADSIZE 9, 129 für ADSIZE 10, 257 für ADSIZE 11)
    cout << "Basin Check:" << sizeof(Basins)/sizeof(Basins[0]) << "\n";
    int Nstates=pow(2,active_neurons);  // = 64 für 6 aktive Vertizes oder ADSIZE = 9
    int Attraktor[100][52];   // Enthält die Infos zu den Attraktoren
    //
  int threshold=1;   // Ende der Respons, ab hier ist die Aktivität MAXFEUER. // default 1
  int ohne_messung=20;   // Anzahl der Updates ohne Vergleich mit Responsvektor
  int mit_messung=20;   // Anzahl der Updates pro Bild mit Vergleich 
  int neutral_failure=50;   // Anzahl der Aenderungen in der neutrality plane
    int bildwiederholung=3;
//   int seedstart=300;
//  int seedmax=100;
// cout << "maximale Seed " << SEED_MAX << "\n";
  //
  int Linecounter=internal_vertices*(Bildgroesse+4+internal_vertices-1); //Gesamtzahl
  //    der gerichteten Linien
  int peripherie=2+Bildgroesse;
  int bildnummer,nzahl,nattractor,testparameter;
  int varianz;
  int fitness;
  int neuronen[ADSIZE];              // Momentane Feuerrate der Neuronen
  int bild[Bildanzahl][Bildgroesse]; //Enthaelt die Muster der Bilder
    int inhib[ADSIZE];     // Vektor, der die inhibitorischen Neuronen charakterisiert
  int adjazenz[ADSIZE][ADSIZE];     // Adjazenzmatrix
  int reaktion[Bildanzahl]; // Optimale Reaktionen auf Bilder
  int linearray[Linecounter][2];  // Nummerierung der moeglichen Linien; Anfangs- und Endvertex
  //int better[Linecounter][2]; // Enthaelt in 0 die Nummern der Linien, die zu einer besseren
  // Fitness fuehren, und in 1 die neue Fitness.
  int equal[Linecounter];
  int nummer[Linecounter];  // Zahlen in richtiger Reihenfolge fuer Zufallszahlen
    int Tester[Bildanzahl][2];   //   Testet, wie viele verschiedene Outputs es bei CHSH=4 gibt
    int Histogramm[Bildanzahl][2];  //  Erstellt die Summe der Responsen auf die Bilder nach Häufigkeit
  int random_1,nr;
  //
  int ik,il,in,i_att;       // Zaehler und Hilfsgroessen
    int ibild;
    int ih1,ih2,ih3,ih4;    // Hilfsgroessen
  int nbetter,nequal;
  int best,bestvariance;
  int counter;
  int count2;
  int Lcounter,Lc;
    int ncounter,nntest;
  int ntest,ngesamt;
    // -------------------------  Definition von Variablen fuer die statistische Analyse der Attraktoren
    double Att_Prob;
    double Entropy1;
    double Entropy2;
    int count_Attractor;
    int count_AttStates;
    double count_AverageError;
    // Alle Histogramme werden initialisiert
    // ---------------------------------
  //
  //  Ausgabe der Anfangsdaten
  //
  outfile << "#Programm: Attraktor_Analyse 1 (CHSH)  Datum: xx.xx.xxxx\n\n";
  outfile << "#ADSIZE: " << ADSIZE << "\n";
  outfile << "#Bildgroesse: " << Bildgroesse <<  "   Bildanzahl: " << Bildanzahl << "   MAXFEUER: " << MAXFEUER << "\n";
  outfile << "#Threshold: " << threshold << "    Anzahl inhibitorischer Neuronen: " << inhibitory  << "\n";
  outfile << "# erlaubte Schritte in neutrality plane: " << neutral_failure << "\n";
  outfile << "# Mit Randomisierung zwischen Bildern!\n";
  outfile << "# Bildwiederholungen: " << bildwiederholung << "\n";
  outfile << "# PLUS=" << PLUS << "  (allgemeine Reaktion auf Bilder  PLUS  1-PLUS  PLUS PLUS) PLUS=1 heißt  1 0 1 1 oder  S=-4)\n";
  outfile << "\n";
    //
  //  Anfangszuweisungen
  //
  //     Definition der Bilder
  //
  for(ik=0;ik<Bildanzahl;++ik){
    for(il=0;il<Bildgroesse;++il){
      bild[ik][il]=MAXFEUER;}}
  // Ab hier speziell bildanzahl=4 und bildgroesse=3
  //
    bild[0][0]=0;
    bild[0][2]=0;
    bild[1][0]=0;
    bild[2][2]=0;

   //
    // Definition der Bilder abgeschlossen
    //
    // Definition der Reaktionen
    //
    // Zu jedem Bild (0-3) gibt es eine Reaktion.
    // Die Staerke der Reaktion ist gestaffelt von 0 bis "maxfeuer".
    // 
    // Bilder 0, 2 und 3 haben Reaktion 0, Bild 1 hat als Reaktion: 1.
      reaktion[0]=PLUS;
      reaktion[1]=1-PLUS;
      reaktion[2]=PLUS;
      reaktion[3]=PLUS;
    //
    //Definition der Reaktionen abgeschlossen
    //
    // Inhibitorische Neuronen - Vektor
    for(ik=0;ik<ADSIZE;++ik){
        inhib[ik]=1;}
    for(ik=ADSIZE-inhibitory;ik<ADSIZE;++ik){
        inhib[ik]=-1;}
    // Definition der "linearray": Je moegliche innere Linie hat eine Nummer
    // von 0 bis Linecounter. Jeder Nummer wird der Anfangs- und Endvertex zugeordnet.
    //   0 bis Bildgroesse-1 sind input Neuronen; Bildgroesse bis peripherie sind output Neuronen; Rest intern
 Lcounter=0;
  for(ik=0;ik<Bildgroesse;++ik){
    for(il=peripherie;il<ADSIZE;++il){         // Verbindung von input mit intern
      linearray[Lcounter][0]=ik;
      linearray[Lcounter][1]=il;
      Lcounter=Lcounter+1;}}
  for(ik=Bildgroesse;ik<peripherie;++ik){
    for(il=peripherie;il<ADSIZE;++il){         //  Verbindung von output mit intern
      linearray[Lcounter][0]=ik;
      linearray[Lcounter][1]=il;
      Lcounter=Lcounter+1;
      linearray[Lcounter][0]=il;
      linearray[Lcounter][1]=ik;
      Lcounter=Lcounter+1;}}
  for(ik=peripherie;ik<ADSIZE;++ik){
    for(il=peripherie;il<ik;++il){             // Verbindung intern mit intern
      linearray[Lcounter][0]=ik;
      linearray[Lcounter][1]=il;
      Lcounter=Lcounter+1;
      linearray[Lcounter][0]=il;
      linearray[Lcounter][1]=ik;
      Lcounter=Lcounter+1;}}
  //
  // Definition der linearray abgeschlossen
    for(ik=0;ik<Bildanzahl;++ik){
        Histogramm[ik][0]=0;
        Histogramm[ik][1]=0;
    }
  //
    counter=0;
  //
    for(iseed=Seedstart;iseed<Seedstart+Anzahl_Seeds;++iseed){ // Seeds durchlaufen die Liste in Seed[]
        //        srand(Seed[iseed]);                   // Initialisierung des Zufallszahlgenerators
        srand(iseed);                   // Initialisierung des Zufallszahlgenerators
//        cout << "---------------------------------------------------------------------------------------------------\n\n";
//        cout << iseed << "\n";
//        outfile << "Seed: " << iseed << "\n";
        //
    // Anfangskonfiguration: Zufallsverteilung für die Feuerraten der Neuronen
        ih1=MAXFEUER+1;
        for(ik=0;ik<ADSIZE;++ik){
        neuronen[ik]=rand()%ih1;}
    //
    // Definition der Adjazenzmatrix.
    // 
  // Definition der leeren Adjazenzmatrix
    for(ik=0;ik<ADSIZE;++ik){
      for(il=0;il<ADSIZE;++il){
	adjazenz[ik][il]=0;}}
    // Definition der leeren Adjazenzmatrix abgeschlossen
    //
    for(ik=0;ik<Linecounter;++ik){
	if(rand()%100<probability){
	  adjazenz[linearray[ik][1]][linearray[ik][0]]=1;}}
    //
    // Ende Definition der Adjazenzmatrix
    //
    // &&&&&&&&&&&& Beginn des eigentlichen Programms &&&&&&&&&&&&&&&&&&&&
    //
    ntest=0;      // Zaehlt die Anzahl der Schritte in der neutrality plane
    ngesamt=0;    // Zaehlt die Anzahl der Generationen
    fitness=10000;   //  Fitness wird auf hohen Wert gesetzt
    // In der folgenden while-Schleife werden so lange Linien auf
    // Entfernen oder Einfügen getestet, bis "test_line"-Mal keine
    // Besserung erfolgt ist
    while(ntest<neutral_failure&&fitness>0){                 // while Anfang
      ntest=ntest+1;
      ngesamt=ngesamt+1;
      // Test: Welche Linie soll veraendert werden
      nbetter=0;         // bisher noch keine bessere Adjazenzmatrix
      nequal=0;          // bisher noch keine gleichgute Adjazenzmatrix
      bestvariance=fitness;
      //   Initialisierung fuer Zufallszahlgenerator
  for(ik=0;ik<Linecounter;++ik){
    nummer[ik]=ik;}
  //
  Lc=0;
  while(nbetter==0&&Lc<Linecounter){      // solange keine bessere Ad-Matrix gefunden wurde
	varianz=0;
	                                      //  Bestimmung einer Zufallslinie
    in=Linecounter-Lc;
    random_1=rand()%in;
    nr=nummer[random_1];  // eine noch nicht dagewesene Zufallszahl zwischen 0 und der Anzahl der noch nicht
                          //  drangewesenen Linien
    for(ik=random_1;ik<in;++ik){     // der gewaehlte Eintag in nummer[] wird ueberschrieben und
      nummer[ik]=nummer[ik+1];}      // alle anderen Eintraege um einen Speicherplatz runtergesetzt.
      Lc=Lc+1;                       // Lc= Anzahl gewaehlter Linien um 1 erhoeht.
    //
      adjazenz[linearray[nr][1]][linearray[nr][0]]=1-adjazenz[linearray[nr][1]][linearray[nr][0]];
      // Dieser Befehl invertiert den Eintrag in der Adjazenzmatrix zu der gewaehlten Linie
      //
      for(count2=0;count2<bildwiederholung;++count2){
    // Der ibild-Loop durchlaeuft saemtliche Bilder und testet die Respons
      //
      for(ibild=0;ibild<Bildanzahl;++ibild){
      // Randomisierung der neuronalen Anregungen
          ih1=MAXFEUER+1;
	      for(ik=0;ik<ADSIZE;++ik){
	      neuronen[ik]=rand()%ih1;}
      //
      // Bildeingabe
    bildnummer=ibild;
    for(ik=0;ik<Bildgroesse;++ik) neuronen[ik]=bild[bildnummer][ik];
    // Ende der Eingabe des Bildes
    // Updates ohne Vergleich mit Responsvektor
    for(in=0;in<ohne_messung;++in){
      update(adjazenz,neuronen,inhib,bildnummer,threshold);}
    // Updates mit Vergleich mit Responsvektor
    for(in=0;in<mit_messung;++in){
      update(adjazenz,neuronen,inhib,bildnummer,threshold);
      vergleich(neuronen,reaktion,bildnummer,&varianz);
    }
    }              // Ende "for ibild=0"
      }            // Ende "for count2"
    //
      if(varianz<fitness){    // falls die Varianz besser ist als die bisher beste
	fitness=varianz;          // bisher beste wird auf neue Varianz gesetzt
	best=nr;                       // Parameter  best  wird gleich der gewaehlten Linie gesetzt
	nbetter=1;}                    //  gibt an, dass (mindestens) eine bessere Ad-Matrix gefunden wurde
    else if(varianz==fitness){     //  falls die Varianz gleich der bisher besten Varianz ist
      equal[nequal]=nr;
      nequal=nequal+1;}
    //
      adjazenz[linearray[nr][1]][linearray[nr][0]]=1-adjazenz[linearray[nr][1]][linearray[nr][0]];
      } // Ende "while nbetter==0"
      if(nbetter>0){           //  falls eine bessere Ad-Matrix gefunden wurde
	ntest=0;
	ik=best;                    // ik = die Linie, die zu einer Besserung gefuehrt hat
// 	fitness=bestvariance;
	adjazenz[linearray[ik][1]][linearray[ik][0]]=1-adjazenz[linearray[ik][1]][linearray[ik][0]];}
                       //  die Ad-Matrix wird an dieser Linie geaendert und die neue akzeptiert
      else if(nequal>0){          // falls keine bessere gefunden wurde, aber eine gleich gute
	il=rand()%nequal;             //  waehle eine der gleich guten Aenderungen
	ik=equal[il];
	adjazenz[linearray[ik][1]][linearray[ik][0]]=1-adjazenz[linearray[ik][1]][linearray[ik][0]];}
                                   // die Ad-Matrix wird auf die neue gleich gute gesetzt
      else {                      // es wurde keine bessere oder gleich gute gefunden
          ntest=neutral_failure;} //  nest wird auf neutral_failure gesetzt
                                  //  und damit while(ntest<neutral_failure...) beendet
        //
        //   &&&&&&&&&&&&&  Ab hier wird die Analyse der Attraktoren vorgenommen
//        if(ngesamt==42){
//        for(ik=0;ik<ADSIZE;++ik){
//            for(il=0;il<ADSIZE;++il){
//                outfile << adjazenz[ik][il] << " ";}
//            outfile << "\n";
//            }}
        //
        if(fitness==0){
        count_Attractor=0;
        count_AttStates=0;
        //
        testparameter=0;
            ibild=0;
            for(ik=0;ik<Bildanzahl;++ik){
                Tester[ik][0]=0;
                Tester[ik][1]=0;
            }
         while(ibild<4&&testparameter==0){
            bildnummer=ibild;
            // Vorbereitung der Attraktorzaehler
            for(ik=0;ik<Nstates;++ik){
                Basins[ik]=0;}
            for(ik=0;ik<50;++ik){
                for(il=0;il<52;++il){
                    Attraktor[ik][il]=0;}}
            ncounter=0;                 // Niedrigster Zustand, der noch keinen Attraktor hat
            nattractor=1;               // Attraktor-Nummer; gibt die Anzahl der untersuchten Attraktoren an
            // Bildeingabe
            for(ik=0;ik<Bildgroesse;++ik) neuronen[ik]=bild[ibild][ik];
            for(ik=Bildgroesse;ik<ADSIZE;++ik) neuronen[ik]=0;
            Basins[0]=1;
            //
            while(ncounter<Nstates){
                update(adjazenz,neuronen,inhib,bildnummer,threshold);
                nzahl=0;
                for(ik=3;ik<ADSIZE;++ik){
                    nzahl=nzahl + neuronen[ik]*Zwei[ik-3];}
                if(Basins[nzahl]==0){                      // 1. Moeglichkeit: der Zustand war noch nie dran
                    Basins[nzahl]=nattractor;
                }                                         // 1. Moeglichkeit Ende
                else if(Basins[nzahl]==nattractor){       //  2.Moeglichkeit: der Zustand ist Teil des momentanen Attraktors
                     nattractor=nattractor+1;      // wir sind fertig; es geht um den naechsten Attraktor
                    while(Basins[ncounter]>0&&ncounter<Nstates){     // wir suchen den naechsten Zustand, der noch nicht dran war
                        ncounter=ncounter+1;}
                    ih1=ncounter;                  //  ih1 wird gleich diesem Zustand gesetzt
                    Basins[ncounter]=nattractor;   //   diesem Zustand wird (vorlaeufig) der naechste Attraktor zugewiesen
                    // Die nächste Schleife übersetzt ncounter in eine neuronale Aktivität
                    for(ik=3;ik<ADSIZE;++ik){
                        neuronen[ik]=ih1%2;
                        ih1=(ih1-neuronen[ik])/2;}
                }                                           // 2. Moeglichkeit Ende
                else{                                    // 3. Moeglichkeit: der Zustand fuehrt in einen alten Attraktor
//                    in=0;
                    for(ik=0;ik<Nstates;++ik){
                        if(Basins[ik]==nattractor){      // Falls ein Zustand "nattractor" zugewiesen wurde ...
//                            in=in+1;
                            Basins[ik]=Basins[nzahl];}}   // ... wird diesem Zustand der alte Attraktor zugewiesen
                    while(Basins[ncounter]>0&&ncounter<Nstates){            // es wird der naechste noch nicht drangewesene Zustand gesucht
                        ncounter=ncounter+1;}
                    Basins[ncounter]=nattractor;    // da nattractor bisher in alten Attraktor lief, wird
                                                    // ncounter nun dieser neue Attraktor "nattractor" zugewiesen
                    if(nattractor>99){              // Falls es mehr als 49 Attraktoren gibt ->  Warnung!
                        cout << "Warnung: Attraktor > 99";
                        exit(EXIT_FAILURE);}
                    ih1=ncounter;
                    // Die nächste Schleife übersetzt ncounter in eine neuronale Aktivität
                    for(ik=3;ik<ADSIZE;++ik){
                        neuronen[ik]=ih1%2;
                        ih1=(ih1-neuronen[ik])/2;}
                }       //   Ende der "if, else if, else" Abfrage
            }      //   while(ncounter<(Nstates-1)
             //
             //       Hiermit ist  basin[]  bestimmt. Jedem Zustand wurde ein Attraktor zugewiesen
             //
             //        Nun werden die Attraktoren ausgewertet und die Info in Attraktor[][] gespeichert
             //
            for(ik=0;ik<Nstates;++ik){
                Attraktor[Basins[ik]][50]=Attraktor[Basins[ik]][50]+1;}
            for(i_att=1;i_att<nattractor;++i_att){
                ih1=0;
                while(Basins[ih1]!=i_att){
                    ih1=ih1+1;}
                //    die Zahl ih1 wird in eine neuronale Aktivität transformiert
                for(il=3;il<ADSIZE;++il){
                    neuronen[il]=ih1%2;
                    ih1=(ih1-neuronen[il])/2;}
                //    die neuronale Aktivität wird 100 mal "upgedated"
                for(in=0;in<100;++in){
                    update(adjazenz,neuronen,inhib,bildnummer,threshold);}
                //                      es wird getestet, ob der Fehler=0 ist
                ih1=neuronen[Bildgroesse];
                ih2=neuronen[Bildgroesse+1];
                ih3=(ih1-ih2)*(ih1-ih2)-reaktion[bildnummer];
                if(ih3!=0){testparameter=1;}  //  falls der Fehler>0 ist, wird testparameter=1 gesetzt
                //    die neuronale Aktivität wird in eine Zahl "nzahl" umgewandelt
                Tester[bildnummer][ih1]=1;    //  gibt an ob Output ih1 bei bildnummer vorliegt
                nzahl=0;
                for(ik=3;ik<ADSIZE;++ik){
                    nzahl=nzahl + neuronen[ik]*Zwei[ik-3];}   // nzahl enthaelt nun die Zahl zur neuronalen Aktivitaet
                nntest=0;                 //   "nntest" zählt von nun an die Anzahl der Zustände in dem Attraktor
                // es werden solange Updates gemacht, bis der neuron-Zustand wieder gleich nzahl ist
                // die Zustände werden jeweils im Attraktor gespeichtert
                ih2=-1;
                ih3=nzahl;      // Speichert nun die momentane Neuronenaktivitaet als Zahl; zunaechst = nzahl
                while(nzahl!=ih2){
                    nntest=nntest+1;
                    // Die Zahl wird als erster Attraktorzustand in Attraktor gespeichert
                    Attraktor[i_att][nntest]=ih3;
                    //  Es wird ein Update gemacht
                    update(adjazenz,neuronen,inhib,bildnummer,threshold);
                    //                      es wird getestet, ob der Fehler=0 ist
                    ih1=neuronen[Bildgroesse];
                    ih2=neuronen[Bildgroesse+1];
                    ih3=(ih1-ih2)*(ih1-ih2)-reaktion[bildnummer];
                    if(ih3!=0){testparameter=1;} //  falls der Fehler>0 ist, wird testparameter=1 gesetzt
                    Tester[bildnummer][ih1]=1;    //   testet of Output ih1 bei bildnummer vorliegt
                    ih3=0;
                    for(ik=3;ik<ADSIZE;++ik){
                        ih3=ih3 + neuronen[ik]*Zwei[ik-3];}       // nun ist ih3=momentaner neuronaler Aktivitaet
                    ih2=ih3;}    //   Ende:   while(nzahl!=ih2)
                if(nntest>48){
                    cout << "Warnung: Attraktorgroesse > 49";
                    outfile << "Warnung: Attraktorgroesse > 49";
                    exit(EXIT_FAILURE);}
                Attraktor[i_att][51]=nntest;
            }     // for i_att=1,i_att<attraktor
             ibild=ibild+1;
         }  //     while(ibild<4 ...
            if(testparameter==0){
                ih1=0;
                for(ik=0;ik<Bildanzahl;++ik){
                    ih1=ih1+Tester[ik][0]+Tester[ik][1];
                }
                if(ih1>4){
                cout << "TP=0   Seed: " << iseed;  //  << "\n";
                outfile << "TP=0 Seed: " << iseed; //  << "\n";
                    cout << "   Tester: " << ih1  << "\n";
                    outfile << "   Tester: " << ih1 << "\n";
//                    cout << "   Bild 1: " << Tester[1][0] + Tester[1][1] << "\n";
                    if (ih1==8) {cout << "                     Perfekter Lerner!\n";}
                }
                if(ih1==4){
                    for(ik=0;ik<Bildanzahl;++ik){
                        Histogramm[ik][0]=Histogramm[ik][0]+Tester[ik][0];
                        Histogramm[ik][1]=Histogramm[ik][1]+Tester[ik][1];
                    }
                }
            }
        //
     //   &&&&&&&&   Attraktoranalyse beendet  &&&&&&&&&&&&&&&
        //
        }         //  Ende der if(fitness==0) Bedingung
    }             // Ende der while(ntest<neutral_failure...) Schleife
        //
    }              // Ende "for iseed"
         outfile << "Histogramm:       A       B\n";
         cout << "Histogramm:       A       B\n";
    for(ik=0;ik<Bildanzahl;++ik){
        cout    << "Bild " << ik << "    A: " <<  Histogramm[ik][0] << "    B: " << Histogramm[ik][1] << "\n";
        outfile << "Bild " << ik << "    A: " <<  Histogramm[ik][0] << "    B: " << Histogramm[ik][1] << "\n";
    }
    //  ende=time(NULL)-start;
}
//&&&&&&&&&&  Funktionen  &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//
// &&&&&&&&&& Funktion  update
//
void update(int adjazenz[][ADSIZE],int neuronen[],int inhib[],
	    int bildnummer, int threshold){
  int ik,il,ih1;
  int hilfsvektor[ADSIZE];
  for(ik=Bildgroesse;ik<ADSIZE;++ik){
    ih1=0;
    for(il=0;il<ADSIZE;++il){
      ih1=ih1+adjazenz[ik][il]*inhib[il]*neuronen[il];}
      if(ih1<0){
          ih1=0;}
    if(ih1<threshold){
      hilfsvektor[ik]=0;}
    else{
      hilfsvektor[ik]=MAXFEUER;}}
  // hilfsvektor[ik]=respons[max_input-1];}}
for(ik=Bildgroesse;ik<ADSIZE;++ik){
  neuronen[ik]=hilfsvektor[ik];}
}
// &&&&&&&&&& Funktion  vergleich
void vergleich(int neuronen[],int reaktion[],
	       int bildnummer,int *varianz){
  int ik;
  int ih1,ih2,ih3,ih4;
  ih4=*varianz;
    ih1=Bildgroesse;
    ih2=Bildgroesse+1;
    ih3=(neuronen[ih1]-neuronen[ih2])*(neuronen[ih1]-neuronen[ih2])-reaktion[bildnummer];
    ih4=ih4+ih3*ih3;
  *varianz=ih4;
}
