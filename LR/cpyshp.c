#include <stdio.h>
#include "shp.h"

/* nel livello corrente, copia nell'immagine RGB la shape numero n nel riquadro r,c */
void cpyshp(int level,unsigned char rgb[],int n,int r,int c)
{
  static int err; /* livello in cui si è verificato un errore */
  int x,y;

  if (n>9) /* se non è una shape valida */
    {
      if (err!=level) /* e se non è gia stato segnalato un errore */
        {
          fprintf(stderr,"Errore: livello %03i\n",level); /* lo riporta */
          err=level; /* tiene traccia del livello con l'errore */
        }
      n=10; /* usa '?' come shape per indicare l'errore nell'immagine RGB */
    }
  for (y=0;y<22;y++) /* 22 righe */
    for (x=0;x<60;x++) /* 20 colonne per 3 componenti colore */
      /* magic numbers! */
      /* 1680 = 560 colonne per 3 componenti colore */
      /* 60 = 20 colonne per 3 componenti colore */
      /* 36960 = 560 colonne per 3 componenti colore per 22 righe */
      /* 1320 = 20 colonne per 3 componenti colore per 22 righe */
      /* 60 = 20 colonne per 3 componenti colore */
      rgb[(1680)*y+(60)*c+(36960)*r+x]=shp[(1320)*n+(60)*y+x];
}
