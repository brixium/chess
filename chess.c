/*Nulla di questo programma è finito o completo, sconsigliato l'utilizzo*/
#include "libchess.h"

#define CLEN 4

int sanitizeInput(char[]);

int main(int argc, char * argv[]){
	char coord[CLEN+1];
	int i, crd[CLEN];
	initCB();
	printf("Formato coordinate accettate (esempio): d2d4\ndove d2 è la casella di partenza e d4 quella di arrivo\n");
	while(!checkmate){
		printCB();
		printf("Qual è la tua mossa? ");
		scanf("%s", coord);
		if(!sanitizeInput(coord)){
			while(!sanitizeInput(coord)){
				printf("Inserisci un input in questa forma: d2d4\ndove d2 è la casella di partenza e d4 quella di arrivo\n");
				scanf("%s", coord);
			}
		}
		for(i=0; i<CLEN; i++){
			if(i%2 == 0)
				crd[i] = coord[i] - 'a';
			else
				crd[i] = coord[i] - '0' - 1;
		}
		if(!move(crd[0], crd[1], crd[2], crd[3]))
			printf("Il programmatore ha sbagliato qualcosa e adesso sta piangendo in un angolo\n");
		printf("Tocca all'avversario\n");
	}
	return 0;
}

int sanitizeInput(char c[]){
	if(c[0] >= 'a' && c[0] <= 'h' && c[2] >= 'a' && c[2] <= 'h')
		if(c[1] > '0' && c[1] < '9' && c[3] > '0' && c[3] < '9')
			return 1;
	return 0;
}
