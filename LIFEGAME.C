//Teend”k:
//-v‚gtelen t bla k‚sz¡t‚se
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>

#define SOR 24
#define OSZ 24
#define URES ' '  //res a mez”
#define TELI 'O'  //l‚tezik egy sejt
#define TILT 'x'  //megszunik a sejt
#define SZUL 's'  //szletend”
#define NMAX 100  //l‚p‚sek sz ma

//
//Menvez‚relt C program, mely a Conway f‚le LIFE (‚letj t‚k) modellt val¢s¡tja meg.
//R‚szletek a http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life c¡men.

//A program k‚pes:
//- 	24*24-es ‚lett‚ren,
//-	a felhaszn l¢  ltal,
//-	illetve file-b¢l beolvasott kezdeti  llapotb¢l indulni,
//-	a szimul ci¢ folyamatos vagy l‚p‚senk‚nti megjelen¡t‚s‚re.
//-     van v‚letlenszeru gener l s is

void init(char eletter[SOR][OSZ])
{
  int i;
  int j;
  for (i= 0; i < SOR; i++)
  {
    for (j= 0; j < OSZ; j++)
    {
      eletter[i][j]= URES;
    }
  }
  //eletter[3][3]= TELI;
  //eletter[3][4]= TELI;
  //eletter[3][5]= TELI;

}

void kiir(char eletter[SOR][OSZ])
{
  int i;
  int j;
  clrscr();
  for (i= 0; i < SOR; i++)
  {
    printf("     ");
    for(j= 0; j < OSZ; j++)
    {
      printf("%c  ", eletter[i][j]);   //‚lett‚r ki¡rat sa
    }
    printf("\n");
  }
}

void feltoltes(char eletter[SOR][OSZ])
{
  int i;
  int j;
  int c;

  init(eletter);

  clrscr();
  printf(">>>Felt”lt‚s<<<\n");
  for (i= 0; i < SOR; i++)
  {
    for(j= 0; j < OSZ; j++)
    {
      c= getch();   //‚lett‚r egy cell j nak beolvas sa
      if (c < '0')
      {
	c= URES;
      }
      else
      {
	c= TELI;
      }
      eletter[i][j]= c;
      putchar(c);
    }
    printf("\n");
  }

  printf("V‚ge.\n");
  getchar();
}

void beolvas(char eletter[SOR][OSZ])
{
  int i;
  int j;
  FILE *fp;
  int c;

  init(eletter);

  fp= fopen("lifegame.txt", "r+");

  clrscr();
  printf(">>>Beolvas s<<<\n");
  for (i= 0; i < SOR; i++)
  {
    for(j= 0; j < OSZ; j++)
    {
      c= fgetc(fp);   //‚lett‚r egy cell j nak beolvas sa
      if (c < '0')
      {
	c= URES;
      }
      else
      {
	c= TELI;
      }
      eletter[i][j]= c;
    }
    c=fgetc(fp);
  }

  fclose(fp);

  kiir(eletter);
  printf("V‚ge.\n");
  getchar();
}

void general(char eletter[SOR][OSZ])
{
  int i;
  int j;
  int r;

  init(eletter);
  randomize();

  clrscr();
  printf(">>>Gener l s<<<\n");

  for (i= 0; i < SOR; i++)
  {
    for (j= 0; j < OSZ; j++)
    {
      r= random(2);
      if (r < 1)
      {
	eletter[i][j]= URES;
      }
      else
      {
	eletter[i][j]= TELI;
      }
    }
  }

  printf("K‚sz.\n");
  getchar();
}

void mentes(char eletter[SOR][OSZ])
{
  int i;
  int j;
  FILE *fp;

  fp= fopen("lifegame.txt", "w+");

  clrscr();
  printf(">>>Ment‚s<<<\n");
  for (i= 0; i < SOR; i++)
  {
    for(j= 0; j < OSZ; j++)
    {
      fprintf(fp, "%c", eletter[i][j]);   //‚lett‚r ki¡rat sa
    }
    fprintf(fp,"\n");
  }

  fclose(fp);

  printf("V‚ge.\n");
  getchar();
}

char get_ter(char eletter[SOR][OSZ], int i, int j)
{
  if (i < 0 )
    i= SOR-1;
  if (i > SOR-1 )
    i= 0;
  if (j < 0 )
    j= OSZ-1;
  if (j > OSZ-1 )
    j= 0;

  return eletter[i][j];
}

// bal fels”b”l indulunk, az ¢ramutat¢val megegyez” ir nyban
char szomszed(char eletter[SOR][OSZ], int i, int j, int k)
{
  if      (k == 1)                   //bal fels” szomsz‚d
  {
      return get_ter(eletter, i-1, j-1);
  }
  else if (k == 2)                   // fels”
  {
      return get_ter(eletter, i-1, j+0);

  }
  else if (k == 3)		     // jobb fels”
  {
      return get_ter(eletter, i-1, j+1);
  }
  else if (k == 4)                   // jobb oldali
  {
      return get_ter(eletter, i+0, j+1);
  }
  else if (k == 5)                   // jobb als¢
  {
      return get_ter(eletter, i+1, j+1);
  }
  else if (k == 6)                   // als¢
  {
      return get_ter(eletter, i+1, j+0);
  }
  else if (k == 7)                   // bal als¢
  {
      return get_ter(eletter, i+1, j-1);
  }
  else if (k == 8)                   // baloldali
  {
      return get_ter(eletter, i+0, j-1);
  }
  else
  {
    return URES;		//erre az  gra nem kerl sor
  }

}

int szomszeddb(char eletter[SOR][OSZ], int i, int j)
{
  int k;
  int s;  //”sszeg

  s= 0;
  for(k= 1; k <= 8; k++)
  {
//    s= szomszed(eletter, i, j, k);
    if (szomszed(eletter, i, j, k) == TELI ||
	szomszed(eletter, i, j, k) == TILT)
    {
      s= s + 1;
    }
  }
  return s;
}

void fut(char eletter[SOR][OSZ], int leptet)
{
  char c;
  int n;
  int db;
  int i;
  int j;

  n= 0;
  while (1)
  {
    kiir(eletter);
    if (leptet)
    {
      c= getch();
      if (c == 27) break;
    }
    else
    {
      if (n > NMAX) break;
      delay(500);
    }

    for(i= 0; i < SOR; i++)
    {
      for (j= 0; j < OSZ; j++)
      {
	db= szomszeddb(eletter, i, j);
	if (db == 3)
	{
	  if (eletter[i][j] == URES)
	  {
	    eletter[i][j]= SZUL;               //szletik egy £j sejt
	  }
	}
	else if (db == 2 || db == 3)
	{
	  //t£l‚li
	}
	else
	{
	  if (eletter[i][j] == TELI)
	  {
	    eletter[i][j]= TILT;            //eltntetend”nek jel”ljk
	  }
	}

      }
    }
    for(i= 0; i < SOR; i++)
    {
      for (j= 0; j < OSZ; j++)
      {
	if (eletter[i][j] == TILT)
	{
	  eletter[i][j]= URES;
	}
	if (eletter[i][j] == SZUL)
	{
	  eletter[i][j]= TELI;
	}
      }
    }
    n++;
  }
}

void main(void)
{
  char c;
  char eletter[SOR][OSZ];

  init(eletter);
//  general(eletter);
  while (1)     //men
  {
    clrscr();

    printf("             letj t‚k               \n");
    printf("                                     \n");
    printf("        Felt”lt‚s                1   \n");
    printf("        Beolvas s                2   \n");
    printf("        Gener l s                3   \n");
    printf("        Szimul l s(folyamatos)   4   \n");
    printf("        Szimul l s(l‚p‚senk‚nt)  5   \n");
    printf("        Ment‚s                   6   \n");
    printf("                                     \n");
    printf("        V‚ge                     0   \n");

    c= getch();
    if      (c == '1') feltoltes(eletter);  //bet”lt‚s a felhaszn l¢  ltal
    else if (c == '2') beolvas(eletter);    //beolvas s f jlb¢l
    else if (c == '3') general(eletter);    //v‚letlenszeru gener l s
    else if (c == '4') fut(eletter, 0);     //folyamatos szimul l s
    else if (c == '5') fut(eletter, 1);     //l‚p‚senk‚nti szimul l s
    else if (c == '6') mentes(eletter);     //az utols¢  llapot elment‚se
    else if (c == '0') break;
    else if (c ==  27) break;               // ESC-el is ki lehet l‚pni
    else               continue;
  }

  printf("V‚ge\n");
  getchar();
}
