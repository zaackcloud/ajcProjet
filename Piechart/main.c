/* Bring in gd library functions */
#include "gd.h"
#include "gdfonts.h"

/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

int* GetData(int *donnees){
    int total =0;
    float res[sizeof donnees ];
    static float curseur[sizeof donnees/sizeof(int)];

    for (int i =0; i<sizeof(donnees);i++){
         total+=donnees[i];
    }
    float div= 360/(float)total;

    for (int j =0; j<sizeof(donnees);j++){
        res[j]=(donnees[j]*div);
        if (j==0){
          curseur[j]=res[j];
        }else{
          curseur[j]=curseur[j-1]+res[j];
        }
    }
    return curseur;
}

int drawChart(float *data, char *title,char** label) {
    srand(time(NULL));
  /* Declare the image */
    int largeur=750;
    int hauteur=750;
    int largeurdiag= (3*largeur)/4;
    int hauteurdiag=(3*hauteur)/4;
  gdImagePtr im;
  gdFontPtr font;
  /* Declare output files */
  FILE *pngout, *jpegout;
  /* Declare color indexes */
  int black,coloralea,white;
  /* Allocate the image: 64 pixels across by 64 pixels tall */
  im = gdImageCreate(largeur,hauteur);
  font= gdFontGetSmall();
  /* Allocate the color black (red, green and blue all minimum).
    Since this is the first color in a new image, it will
    be the background color. */
  //enum couleur {black,beige,blue,red,green,yellow,purple,white};
  black = gdImageColorAllocate(im, 0, 0, 0);
  white = gdImageColorAllocate(im, 255, 255, 255);

  gdImageFilledRectangle(im,0,0,largeur,hauteur,white);
  /* Draw a line from the upper left to the lower right,
    using white color index. */
  char*T=title;
  char**lab=label;
//  for (int i=0;i<sizeof(lab);i++){
//      printf("%s\n",&lab[i]);
//  }

  for (int i=0; i<6;i++){
      coloralea=gdImageColorAllocate(im,rand()%256,rand()%256,rand()%256);
      if (i==0){
          gdImageSetThickness(im, 3);
          gdImageArc(im, largeur/2+10, hauteur/2+5, largeurdiag, hauteurdiag,0,data[i], black);
          gdImageArc(im, largeur/2, hauteur/2, largeurdiag, hauteurdiag,data[i],360, black);
          int startX = largeur/2+10 + largeurdiag/2;
          int startY = hauteur/2+5 + hauteurdiag/2;
          int endX = largeur/2+10 + largeurdiag/2 * cos(data[i] * M_PI / 180);
          int endY = hauteur/2+5 + hauteurdiag/2 * sin(data[i] * M_PI / 180);
          gdImageLine(im, largeur/2, hauteur/2, startX, startY, black);
          gdImageLine(im, largeur/2, hauteur/2, endX, endY, black);
          gdImageSetThickness(im, 1);
          gdImageFilledArc(im, largeur/2+10, hauteur/2+5, largeurdiag, hauteurdiag, 0, data[i], coloralea, gdPie);
      }else{
          gdImageSetThickness(im, 1);
          gdImageFilledArc(im, largeur/2, hauteur/2, largeurdiag, hauteurdiag, data[i-1], data[i], coloralea, gdPie);
          gdImageSetThickness(im, 3);
          int startX = largeur/2 + largeurdiag/2 * cos(data[i-1] * M_PI / 180);
          int startY = hauteur/2 + hauteurdiag/2 * sin(data[i-1] * M_PI / 180);
          int endX = largeur/2 + largeurdiag/2 * cos(data[i] * M_PI / 180);
          int endY = hauteur/2 + hauteurdiag/2 * sin(data[i] * M_PI / 180);
          gdImageLine(im, largeur/2, hauteur/2, startX, startY, black);
          gdImageLine(im, largeur/2, hauteur/2, endX, endY, black);
      }
   }
  gdImageString(im, font , (1*largeur)/10, (1*hauteur)/10, &lab[0], black);
  /* Open a file for writing. "wb" means "write binary", important
    under MSDOS, harmless under Unix. */
  pngout = fopen("test.png", "wb");

  /* Do the same for a JPEG-format file. */
  jpegout = fopen("test.jpg", "wb");

  /* Output the image to the disk file in PNG format. */
  gdImagePng(im, pngout);

  /* Output the same image in JPEG format, using the default
    JPEG quality setting. */
  gdImageJpeg(im, jpegout, -1);

  /* Close the files. */
  fclose(pngout);
  fclose(jpegout);

  /* Destroy the image in memory. */
  gdImageDestroy(im);
}

int main(){
    char* label[]={"Perdrix", "Canards", "Lapins", "Faisans", "Cerfs", "Sangliers"};
//    char* temp=(char*)malloc(sizeof(char*));
//    for (int i=0;i<sizeof(label);i++){
//        strcpy(temp,label[i]);
//        printf("%s\n",temp);
//    }
    int tab[6]={56,120,47,69,12,23};
    char title[]="resultats de la chasse";
    int* data;
    data=GetData(tab);
    drawChart(data, title, label);
    return 0;
}
