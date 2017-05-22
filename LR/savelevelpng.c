#include <stdio.h>
#include <png.h>

/* codice rubato (con modifiche) da XPlanet di Hari Nair <hari@alumni.caltech.edu> */
/* genera png (senza canale alpha) a partire dai dati contenuti in immagine RGB    */
void savelevelpng(int level,unsigned char rgb[])
{
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep row_ptr;

  int i;

  char outfilename[9];
  FILE *outfile;

  sprintf(outfilename,"L%03u.png",level);
  outfile=fopen(outfilename,"wb");

  png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING,(png_voidp)NULL,(png_error_ptr)NULL,(png_error_ptr)NULL);

  if (!png_ptr)
  return;

  info_ptr=png_create_info_struct(png_ptr);
  if (!info_ptr)
    {
      png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
      return;
    }

  png_init_io(png_ptr,outfile);

  /* 560 colonne per 352 righe per 8 bit */
  png_set_IHDR(png_ptr,info_ptr,560,352,8,PNG_COLOR_TYPE_RGB,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);

  png_write_info(png_ptr,info_ptr);

  for (i=0;i<352;i++) /* righe */
    {
      row_ptr=rgb+3*i*560; /* colonne */
      png_write_rows(png_ptr,&row_ptr,1);
    }

  png_write_end(png_ptr,info_ptr);
  png_destroy_write_struct(&png_ptr,&info_ptr);

  fclose(outfile);
}
