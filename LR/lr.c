/* Lode Runner:                                                                 */
/* legge un file di 38400 byte ovvero 150 livelli da 256 byte l'uno, oppure     */
/* legge una immagine disco da 143360 byte e si posiziona all'inizo dei livelli */
/* genera 150 immagini bmp/png, una per ogni livello                            */

/* l'immagine deve essere in DOS 3.3 order (raw) */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

#define BMP 1
#define PNG 2

#ifdef WIN32
# define DEFAULT_FORMAT BMP
#endif
#ifdef UNIX
# define DEFAULT_FORMAT PNG
#endif

#define DISK_SIZE 143360
#define LEVELS_SIZE 38400

struct stat b;
FILE *infile;

void readlevel();
void savelevelpng();
void savelevelbmp();

int main(int argc,char **argv)
{
  int opt,fmt=0,level;
  unsigned char rgb[352*560*3];

  opterr=0; /* getopt deve stare zitta */
  while ((opt=getopt(argc,argv,"f:"))!=-1)
    {
      switch (opt)
        {
        case 'f':
          if (fmt)
            {
              fprintf(stderr,"usare solo una volta l'opzione f\n");
              return 1;
            }
          if (!strcasecmp(optarg,"bmp"))
            fmt=BMP;
          else
          if (!strcasecmp(optarg,"png"))
            fmt=PNG;
          else
            {
              fprintf(stderr,"'%s': argomento non valido per l'opzione f\n",optarg);
              return 1;
            }
          break;
        case '?':
          fprintf(stderr,"'%c': opzione sconosciuta\n",optopt);
          return 1;
        }
    }

  if (fmt==0)
    fmt=DEFAULT_FORMAT;

  if (argc-optind==0)
    {
      fprintf(stderr,"manca nome disco/file\n");
      return 1;
    }
  if (argc-optind>1)
    {
      fprintf(stderr,"troppi argomenti\n");
      return 1;
    }

  if (stat(argv[optind],&b)==-1)
    {
      fprintf(stderr,"'%s': deve esistere\n",argv[optind]);
      return 1;
    }
  if ((b.st_mode&S_IFMT)!=S_IFREG)
    {
      fprintf(stderr,"'%s': deve essere regolare\n",argv[optind]);
      return 1;
    }
  if (b.st_size!=DISK_SIZE && b.st_size!=LEVELS_SIZE)
    {
      fprintf(stderr,"'%s': deve essere una immagine disco o deve contenere 150 livelli\n",argv[optind]);
      return 1;
    }
  if ((infile=fopen(argv[optind],"rb"))==NULL) /* b è ignorato da UNIX e richiesto da WIN32 */
    {
      fprintf(stderr,"'%s': deve essere leggibile\n",argv[optind]);
      return 1;
    }

  if (b.st_size==DISK_SIZE) /* se è una immagine disco */
    fseek(infile,0x03000,SEEK_SET); /* si posiziona alla traccia 3 settore 0, inizio dei livelli */

  for (level=1;level<=150;level++) /* per 150 livelli */
    {
      readlevel(level,infile,rgb); /* legge e converte un livello */
      switch (fmt)
        {
        case BMP:
          savelevelbmp(level,rgb); /* crea e salva bmp del livello */
          break;
        case PNG:
          savelevelpng(level,rgb); /* crea e salva png del livello */
        }
    }

  fclose(infile);

  return 0;
}
