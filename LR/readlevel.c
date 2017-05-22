#include <stdio.h>

void cpyshp();

/* un livello da 16 righe per 28 colonne occupa 256 byte    */
/* i primi 224 codificano l'immagine (1 byte per 2 colonne) */
/* i rimanenti sono usati in certe versioni per il nome del */
/* livello o non sono significativi                         */
void readlevel(int level,FILE *infile,unsigned char rgb[])
{
  unsigned char byte,nibble;
  int r,c;

  for (r=0;r<16;r++) /* per 16 righe */
    {
      for (c=0;c<28;c=c+2) /* per 28 colonne (1 byte per 2 colonne) */
        {
          byte=getc(infile);
          nibble=byte&0x0F; /* nibble alto */
          cpyshp(level,rgb,nibble,r,c);
          nibble=byte>>4; /* nibble basso */
          cpyshp(level,rgb,nibble,r,c+1);
        }
    }
  for (c=0;c<32;c++) /* scarta gli ultimi byte */
    getc(infile);
}
