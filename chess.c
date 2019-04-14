/*Nulla di questo programma è finito o completo, sconsigliato l'utilizzo*/
#include "libchess.h"

#define CLEN 4 /*Coordinate length*/

int sanitizeInput(char[]);

int main(int argc, char * argv[]){
	char coord[CLEN+1], color, acol;
	int i, crd[CLEN], mossaOK;

	initCB();
	do{
		printf("Scegli il tuo colore [B/W] (IN CAPS): ");
		scanf(" %c", &color);
	}while(color != 'W' && color != 'B');
/*pezzo da togliere poi, solo x testing*/
	if(color == 'B')
		acol = 'W';
	else
		acol = 'B';
/**/
	while(!checkmate){
		printCB();
		if(isScacco(color))
			printf("SEI SOTTO SCACCO!\n");
		mossaOK = 0;
		while(!mossaOK){
			printf("Inserisci la mossa nel formato d2d4, dove d2 è la pedina da muovere e d4 è la posizione di arrivo ");
			scanf("%s", coord);

			for(i=0; i<CLEN; i++){
				if(i%2 == 0)
					crd[i] = coord[i] - 'a';
				else
					crd[i] = coord[i] - '0' - 1;
			}
			/*First x then y*/
			if(move(crd[1], crd[0], crd[3], crd[2], color))
				mossaOK = 1;
			else
				printf("Non puoi muoverti lì!\n");
		}
		if(isScacco(acol))
			printf("Bella mossa!\n");
		printf("Tocca all'avversario\n");
	}
	return 0;
}

