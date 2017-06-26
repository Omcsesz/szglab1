//Teendok:
//-vegtelen tabla keszitese
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <dos.h>

#define SOR 24
#define OSZ 24
#define URES ' '  //ures a mezo
#define TELI 'O'  //letezik egy sejt
#define TILT 'x'  //megszunik a sejt
#define SZUL 's'  //szuletendo
#define NMAX 100  //lepesek szama

//
//Menvezurelt C program, mely a Conway fele LIFE (eletjatek) modellt valositja meg.
//Reszletek a http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life cimen.

//A program kepes:
//- 	24*24-es eletteren,
//-	a felhasznalo altal,
//-	illetve file-bol beolvasott kezdeti allapotbol indulni,
//-	a szimulacio folyamatos vagy lepesenkenti megjelenitesere.
//-     van veletlenszeru generalas is

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
      printf("%c  ", eletter[i][j]);   //eletter kiiratasa
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
  printf(">>>Feltolt‚s<<<\n");
  for (i= 0; i < SOR; i++)
  {
    for(j= 0; j < OSZ; j++)
    {
      c= getch();   //eletter egy cellajanak beolvasasa
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

  init(eletter);

  fp= fopen("lifegame.txt", "r+");

  clrscr();
  printf(">>>Beolvasas<<<\n");
  for (i= 0; i < SOR; i++)
  {
    for(j= 0; j < OSZ; j++)
    {
      eletter[i][j]= fgetc(fp);   //eletter egy cellajanak beolvasasa
    }
    fgetc(fp);
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
  printf(">>>Generalas<<<\n");

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

  printf("Kesz.\n");
  getchar();
}

void mentes(char eletter[SOR][OSZ])
{
  int i;
  int j;
  FILE *fp;

  fp= fopen("lifegame.txt", "w+");

  clrscr();
  printf(">>>Mentes<<<\n");
  for (i= 0; i < SOR; i++)
  {
    for(j= 0; j < OSZ; j++)
    {
      fprintf(fp, "%c", eletter[i][j]);   //eletter kiiratasa
    }
    fprintf(fp,"\n");
  }

  fclose(fp);

  printf("V‚ge.\n");
  getchar();
}

// bal felsobol indulunk, az oramutatoval megegyezo iranyban
char szomszed(char eletter[SOR][OSZ], int i, int j, int k)
{
  if(k == 1)                //bal felso szomszed
  {
    if (i == 0 && j == 0)         // hogy ne fussunk ki a jatekterrol
      return URES;
    else
      return eletter[i-1][j-1];
  }
  else if (k == 2)                // felso
  {
    if (i == 0)
      return URES;
    else
      return eletter[i-1][j+0];

  }
  else if (k == 3)		  // jobb felso
  {
    if (i == 0 && j == OSZ-1)
      return URES;
    return eletter[i-1][j+1];
  }
  else if (k == 4)                // jobb oldali
  {
    if (j == OSZ-1)
      return URES;
    else
      return eletter[i+0][j+1];
  }
  else if (k == 5)                // jobb also
  {
    if (i == OSZ-1 && j == SOR-1)
      return URES;
    else
      return eletter[i+1][j+1];
  }
  else if (k == 6)                // also
  {
    if (i == SOR-1)
      return URES;
    else
      return eletter[i+1][j+0];
  }
  else if (k == 7)                // bal also
  {
    if (i == SOR-1 && j == 0)
      return URES;
    else
      return eletter[i+1][j-1];
  }
  else if (k == 8)                // baloldali
  {
    if (j == 0)
      return URES;
    else
      return eletter[i+0][j-1];
  }
  else
  {
    return URES;		//erre az agra nem kerul sor
  }
}

int szomszeddb(char eletter[SOR][OSZ], int i, int j)
{
  int k;
  int s;  //osszeg

  s= 0;
  for(k= 1; k <= 8; k++)
  {
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
	    eletter[i][j]= SZUL;               //szuletik egy uj sejt
	  }
	}
	else if (db == 2 || db == 3)
	{
	  //tuleli
	}
	else
	{
	  if (eletter[i][j] == TELI)
	  {
	    eletter[i][j]= TILT;            //eltuntetendonek jeloljuk
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

    printf("             Eletjatek               \n");
    printf("                                     \n");
    printf("        Feltoltes                1   \n");
    printf("        Beolvasas                2   \n");
    printf("        Generalas                3   \n");
    printf("        Szimulalas(folyamatos)   4   \n");
    printf("        Szimulalas(lepesenkent)  5   \n");
    printf("        Mentes                   6   \n");
    printf("                                     \n");
    printf("        Vege                     0   \n");

    c= getch();
    if      (c == '1') feltoltes(eletter);  //betoltes a felhasznalo altal
    else if (c == '2') beolvas(eletter);    //beolvasas fajlbol
    else if (c == '3') general(eletter);    //veletlenszeru generalas
    else if (c == '4') fut(eletter, 0);     //folyamatos szimulalas
    else if (c == '5') fut(eletter, 1);     //lepesenkenti szimulalas
    else if (c == '6') mentes(eletter);     //az utolso allapot elmentese
    else if (c == '0') break;
    else if (c ==  27) break;               // ESC-el is ki lehet lepni
    else               continue;
  }

  printf("Vege\n");
  getchar();
}
