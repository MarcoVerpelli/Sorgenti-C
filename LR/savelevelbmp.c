#include <stdio.h>

/* codice rubato (con modifiche) da XPlanet di Hari Nair <hari@alumni.caltech.edu> */
/* genera bmp a partire dai dati contenuti in immagine RGB                         */
void savelevelbmp(int level,unsigned char rgb[])
{
  struct BMPHeader
  {
    char bfType[2];       /* "BM" */
    int bfSize;           /* Size of file in bytes */
    int bfReserved;       /* set to 0 */
    int bfOffBits;        /* Byte offset to actual bitmap data (= 54) */
    int biSize;           /* Size of BITMAPINFOHEADER, in bytes (= 40) */
    int biWidth;          /* Width of image, in pixels */
    int biHeight;         /* Height of images, in pixels */
    short biPlanes;       /* Number of planes in target device (set to 1) */
    short biBitCount;     /* Bits per pixel (24 in this case) */
    int biCompression;    /* Type of compression (0 if no compression) */
    int biSizeImage;      /* Image size, in bytes (0 if no compression) */
    int biXPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biYPelsPerMeter;  /* Resolution in pixels/meter of display device */
    int biClrUsed;        /* Number of colors in the color table (if 0, use maximum allowed by biBitCount) */
    int biClrImportant;   /* Number of important colors.  If 0, all colors are important */
  } bmph;
  int i,j,ipos;
  unsigned char line[560*3];

  char outfilename[9];
  FILE *outfile;

  sprintf(outfilename,"L%03u.bmp",level);
  outfile=fopen(outfilename,"wb");

  bmph.bfType[0]='B';
  bmph.bfType[1]='M';
  bmph.bfSize=560*352+54;
  bmph.bfReserved=0;
  bmph.bfOffBits=54;
  bmph.biSize=40;
  bmph.biWidth=560;
  bmph.biHeight=352;
  bmph.biPlanes=1;
  bmph.biBitCount=24;
  bmph.biCompression=0;
  bmph.biSizeImage=560*352*3;
  bmph.biXPelsPerMeter=0;
  bmph.biYPelsPerMeter=0;
  bmph.biClrUsed=0;
  bmph.biClrImportant=0;

  fwrite(&bmph.bfType,2,1,outfile);
  fwrite(&bmph.bfSize,4,1,outfile);
  fwrite(&bmph.bfReserved,4,1,outfile);
  fwrite(&bmph.bfOffBits,4,1,outfile);
  fwrite(&bmph.biSize,4,1,outfile);
  fwrite(&bmph.biWidth,4,1,outfile);
  fwrite(&bmph.biHeight,4,1,outfile);
  fwrite(&bmph.biPlanes,2,1,outfile);
  fwrite(&bmph.biBitCount,2,1,outfile);
  fwrite(&bmph.biCompression,4,1,outfile);
  fwrite(&bmph.biSizeImage,4,1,outfile);
  fwrite(&bmph.biXPelsPerMeter,4,1,outfile);
  fwrite(&bmph.biYPelsPerMeter,4,1,outfile);
  fwrite(&bmph.biClrUsed,4,1,outfile);
  fwrite(&bmph.biClrImportant,4,1,outfile);

  for (i=352-1;i>=0;i--)
    {
      for (j=0;j<560;j++)
        {
          ipos=3*(560*i+j);
          line[3*j]=rgb[ipos+2];
          line[3*j+1]=rgb[ipos+1];
          line[3*j+2]=rgb[ipos];
        }
      fwrite(line,560*3,1,outfile);
    }

 fclose(outfile);
}
