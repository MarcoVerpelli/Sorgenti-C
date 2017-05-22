/* Automa cellulare esagonale
**
** Tratto dalle indicazioni date nella didascalia di pag.138 e dalla foto alla
** pagina seguente dell'articolo: "Software nella scienza e nella matematica"
** di Stephen Wolfram apparso nel numero monografico di "Le Scienze" 195
** del novembre '84
**
** Lo schema dei colori mi pare una buona approssimazione di quanto si vede
** nella fotografia citata (funziona solo se ci sono almeno 65536 colori).
*/

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include "icon.xbm"

/* stati possibili della cellula */
#define VAPORE 0
#define GHIACCIO 1

/*
** lato del quadrato (deve essere pari) che rappresenterà una cellula
** per piccole dimensioni: quadrato = esagono
*/
#define DIMC 2

/* numero massimo di generazioni da visualizzare (deve essere < 999) */
#define MAXG 128

/*
** da qui in poi NON si devono modificare i #define
*/

/* quanto alternare la posizione delle cellule tra una colonna e l'altra */
#define OFFS DIMC/2

/* dimensione della finestra (quadrata) */
#define DIMW DIMC*(2*MAXG+1)

/* numero massimo di cellule contenute nella matrice (quadrata) */
#define MAXC DIMW/DIMC

/*
** in realtà quasi da ogni parte si considera la matrice come
** circondata da una fascia della morte con cellule sempre a zero
*/
#define MAXE (MAXC-1)

/* font da usare nella didascalia */
#define FONTNAME "9x15"

/*
** Attenzione i valori dipendono dalla dimensione del font (9x15 pixel)
** e dalla lunghezza della didascalia (15 caratteri)
*/

/* posizione didascalia (angolo in basso a destra) */
#define DX DIMW-1-15*9
#define DY DIMW-4

/* le solite variabili e strutture per X usate da più parti */
Display *display;
Window window;
XWMHints *wm_hints;
XSizeHints *size_hints;
XFontStruct *font_info;
GC gcc,gcd;
Colormap colormap;
XColor color_label,color_cell;
Atom wm_protocols,wm_delete_window;

typedef struct cellula
  { /* la struttura che rappresenta una cellula */
    short int stato; /* 0 = vapore, 1 = ghiaccio */
    unsigned long colore; /* stesso tipo di XColor.pixel */
  }
cellule;

/* le matrici che contengono la generazione corrente '0' e la successiva '1' */
cellule g0[MAXC][MAXC],g1[MAXC][MAXC]; /* vengono creati pieni di '0'!!! */

int g=1; /* il contatore delle generazioni */

int init() /* inizializza il tutto */
{
  /* variabili e strutture per X */
  int screen;
  Pixmap icon_pixmap;
  int r,c; /* indici righe, colonne */

  if ((display=XOpenDisplay(NULL))==NULL)
    {
      fprintf(stderr,"Non posso aprire la finestra\n");
      return 1;
    }
  screen=DefaultScreen(display);
  window=XCreateSimpleWindow(display,RootWindow(display,screen),1,1,DIMW,DIMW,0,
                             BlackPixel(display,screen),BlackPixel(display,screen));
  wm_protocols=XInternAtom(display,"WM_PROTOCOLS",False);
  wm_delete_window=XInternAtom(display,"WM_DELETE_WINDOW",False);
  XSetWMProtocols(display,window,&wm_delete_window,1);
  XStoreName(display,window,"Snowflake");
  XSetIconName(display,window,"Snowflake");
  icon_pixmap=XCreateBitmapFromData(display,window,(char *)icon_bits,icon_width,icon_height);
  wm_hints=XAllocWMHints();
  wm_hints->flags=IconPixmapHint;
  wm_hints->icon_pixmap=icon_pixmap;
  XSetWMHints(display,window,wm_hints);
  size_hints=XAllocSizeHints();
  size_hints->flags=PMinSize|PMaxSize; /* la finestra ha dimensioni fisse */
  size_hints->min_width=DIMW;
  size_hints->min_height=DIMW;
  size_hints->max_width=DIMW;
  size_hints->max_height=DIMW;
  XSetWMNormalHints(display,window,size_hints);
  if ((font_info=XLoadQueryFont(display,FONTNAME))==NULL)
    {
      fprintf(stderr,"Non posso aprire il font %s\n",FONTNAME);
      return 1;
    }
  colormap=DefaultColormap(display,screen);
  gcd=XCreateGC(display,window,0,0); /* contesto grafico per la didascalia */
   /* la didascalia è verde */
  XParseColor(display,colormap,"green",&color_label);
  XAllocColor(display,colormap,&color_label);
  XSetForeground(display,gcd,color_label.pixel);
  XSetFont(display,gcd,font_info->fid);
  gcc=XCreateGC(display,window,0,0); /* contesto grafico per le cellule */
  color_cell.red=0xffff; /* l'unica cellula iniziale è rossa */
  color_cell.green=0x0;
  color_cell.blue=0x0;
  XAllocColor(display,colormap,&color_cell);
  XSelectInput(display,window,ExposureMask|KeyPressMask);
  XMapWindow(display,window);
  /* si parte con una sola cellula ghiacciata (rossa) posta al centro */
  r=c=MAXC>>1;
  g0[r][c].stato=g1[r][c].stato=GHIACCIO;
  g0[r][c].colore=g1[r][c].colore=color_cell.pixel;
  color_cell.blue=0xffff;
  return 0;
}

void show() /* rappresenta graficamente il contenuto della matrice */
{
   /* indici matrice, offset colonna, lunghezza didascalia in caratteri */
  int r,c,o,len;
  char label[16]; /* testo didascalia */

  for (r=1;r<MAXE;r++) /* non considera il bordo esterno */
    {
      o=OFFS*!(r%2); /* sapientemente offuscato! */
      for (c=1;c<MAXE;c++) /* non considera il bordo esterno */
        if (g0[r][c].stato==GHIACCIO) /* se la cellula è ghiacciata */
          {
            XSetForeground(display,gcc,g0[r][c].colore); /* imposta colore */
            /* la disegna */
            XFillRectangle(display,window,gcc,r*DIMC,c*DIMC-o,DIMC,DIMC);
          }
    }
   /* stampa la didascalia, cancellando la precedente */
  len=sprintf(label,"generazione=%3i",g);
  XClearArea(display,window,DX+font_info->min_bounds.lbearing,DY-font_info->max_bounds.ascent,
             XTextWidth(font_info,label,len),font_info->max_bounds.ascent+font_info->max_bounds.descent,0);
  XDrawString(display,window,gcd,DX,DY,label,len);
}

void nextgen() /* calcola la prossima generazione */
{
  /* quanto virare verso il blu ad ogni generazione */
  /* stesso tipo di XColor.red (o green o blue) */
  static unsigned short delta=0xffff; /* all'inizio il cambio è drastico */
  /* a quale generazione il colore delle cellule tornerà rosso pieno */
  static int cc=2; /* la prima potenza di 2 */
  int r,c,o; /* indici righe, colonne ed offset colonna */

  ++g; /* la generazione che si sta calcolando */
  if (g>cc) /* se il colore deve tornare rosso pieno */
    {
      color_cell.red=0xffff; /* resetta il colore */
      delta>>=1; /* di quanto virare da rosso a blu per ogni generazione */
      /* prossima generazione alla quale il colore tornerà rosso pieno */
      cc<<=1;
    }
  else /*
       ** altrimenti è un blueshift:
       ** l'universo si contrae
       */
    {
      color_cell.red-=delta;
    }
  /* riutilizza sempre lo stesso */
  XFreeColors(display,colormap,&color_cell.pixel,1,0);
  XAllocColor(display,colormap,&color_cell);
  /* il calcolo vero e proprio della prossima generazione */
  for (r=1;r<MAXE;r++) /* non considera il bordo esterno */
    {
      o=!(r%2); /* ugualmente offuscato */
      for (c=1;c<MAXE;c++) /* non considera il bordo esterno */
        /* la regola è:
           **              la cellula corrente ghiaccia se e solo se è
           **              circondata da un numero dispari di cellule
           **              ghiacciate, altrimenti rimane vapore
           */
        if (!g0[r][c].stato&&(g0[r-1][c-o].stato+g0[r-1][c+1-o].stato+g0[r][c+1].stato+
             g0[r+1][c+1-o].stato+g0[r+1][c-o].stato+g0[r][c-1].stato)%2)
          { /* se è vapore e se ha un numero dispari di vicini ghiacciati */
            g1[r][c].stato=GHIACCIO; /* la cellula diventa ghiaccio */
            g1[r][c].colore=color_cell.pixel;
          }
    }
}

/* copia la matrice nuova generazione in quella vecchia
   per permettere un nuovo calcolo */
void copy()
{
  int r,c; /* indici righe, colonne */

  for (r=1;r<MAXE;r++) /* non considera il bordo esterno */
    for (c=1;c<MAXE;c++) /* non considera il bordo esterno */
      g0[r][c]=g1[r][c];
}

void clear() /* rilascia le risorse */
{
  XFree(wm_hints);
  XFree(size_hints);
  XUnloadFont(display,font_info->fid);
  XFreeGC(display,gcc);
  XFreeGC(display,gcd);
  XFreeColors(display,colormap,&color_label.pixel,1,0);
  XFreeColors(display,colormap,&color_cell.pixel,1,0);
  XCloseDisplay(display);
  return;
}

int main()
{
  XEvent report; /* l'evento da gestire */

  init(); /* avvia le macchine */
  while (1) /* loop (infinito) per gestire gli eventi */
    {
      XNextEvent(display,&report);
      switch (report.type)
        {
        case ClientMessage: /* il window manager ha chiesto la chiusura */
          if (report.xclient.message_type==wm_protocols&&report.xclient.data.l[0]==wm_delete_window)
            clear(); /* pulisce */
            return 0; /* esce */
        case Expose: /* lo stato delle finestre è cambiato */
          if (report.xexpose.count==0)
            show(); /* (ri)disegna il contenuto */
          break;
        case KeyPress: /* è stato premuto un tasto */
          switch(XLookupKeysym(&report.xkey,0))
            {
            case XK_space: /* era spazio */
              if (g<MAXG) /* se è ancora interamente rappresentabile */
                {
                  nextgen(); /* calcola una nuova generazione */
                  copy(); /* la copia nella vecchia */
                  show(); /* la visualizza */
                }
              break;
            case XK_Escape: /* era ESC */
              clear(); /* pulisce */
              return 0; /* esce */
            }
        }
    }
}
