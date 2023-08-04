/* Bring in gd library functions */
#include "gd.h"
#include "gdfonts.h"

/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>
#include <stdlib.h>
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


void createSegment(gdImagePtr im, int centerX, int centerY, int sizeX, int sizeY, int angleStart, int angleEnd, int colorExt, int colorInt){
    //calculate the coordinates for the segment
    float startX = centerX + sizeX/2* cos(angleStart * M_PI / 180);
    float startY = centerY + sizeY/2* sin(angleStart * M_PI / 180);
    float endX = centerX + sizeX/2 * cos(angleEnd * M_PI / 180);
    float endY = centerY + sizeY/2 * sin(angleEnd * M_PI / 180);
    //draw the limits of the segment
    gdImageSetThickness(im, 4);
    gdImageArc(im, centerX, centerY, sizeX, sizeY,angleStart,angleEnd, colorExt);
    gdImageLine(im, centerX, centerY, startX, startY, colorExt);
    gdImageLine(im, centerX, centerY, endX, endY, colorExt);
    //fill the segment
    gdImageSetThickness(im, 1);
    gdImageFilledArc(im, centerX, centerY, sizeX, sizeY, angleStart, angleEnd, colorInt, gdPie);
}

void createLabel(gdImagePtr im,int centerX, int centerY, int size, int angleStart, int angleEnd, int color,char* label){
    gdFontPtr font= gdFontGetSmall();;
    double median = (angleStart + angleEnd) / 2.0;
    double labelangle;
    if (cos(median * M_PI / 180) < 0){
        labelangle = size/1.8 + strlen(label)*(15/1.2);
        }else{
        size/1.8;
    }
    int labelX = centerX + labelangle * cos(median * M_PI / 180);
    int labelY = centerY + size/1.8 * sin(median * M_PI / 180);
    int pinX = centerX + size/2 * cos(median * M_PI / 180);
    int pinY = centerY + size/2 * sin(median * M_PI / 180);
    int pinXend = centerX + size/1.8 * cos(median * M_PI / 180);
    int pinYend = labelY;
    gdImageString(im, font , labelX, labelY, label, color);
    gdImageLine(im, pinX, pinY, pinXend, pinYend, color);
}

int drawChart(float *data, char *title,char** label) {
    srand(time(NULL));
  /* Declare the image */
    int largeur=750;
    int hauteur=750;
    int largeurdiag= (3*largeur)/4;
    int hauteurdiag=(3*hauteur)/4;
  gdImagePtr im;
  /* Declare output files */
  FILE *pngout, *jpegout;
  /* Declare color indexes */
  int black,coloralea,white;
  /* Allocate the image: 64 pixels across by 64 pixels tall */
  im = gdImageCreate(largeur,hauteur);

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
  int centerX=largeur/2;
  int centerY=hauteur/2;
  int centerXalt=largeur/2+(5*largeur/100);
  int centerYalt=hauteur/2+(2.5*hauteur/100);

  for (int i=0; i<6;i++){
      coloralea=gdImageColorAllocate(im,rand()%256,rand()%256,rand()%256);
      /*the first segment is reparated from the rest of the diagram*/
      if (i==0){
          createSegment(im,centerXalt,centerYalt,largeurdiag,hauteurdiag,0,data[i],black,coloralea);
          createLabel(im,centerXalt,centerYalt,largeurdiag,0,data[i],black,label[i]);
      }else{
          createSegment(im,centerX,centerY,largeurdiag,hauteurdiag,data[i-1],data[i],black,coloralea);
          createLabel(im,centerX,centerY,largeurdiag,data[i-1],data[i],black,label[i]);
      }
   }
  //gdImageString(im, font , (1*largeur)/10, (1*hauteur)/10, &lab[0], black);


  /* write and register the output file */
  pngout = fopen(title, "wb");
  gdImagePng(im, pngout);


  /* Close the file. */
  fclose(pngout);

  /* Destroy the image in memory. */
  gdImageDestroy(im);
}

int main(int argc, char* argv[]){
    char* label[]={"Perdrix", "Canards", "Lapins", "Faisans", "Cerfs", "Sangliers"};

    //const char *title = argv[1];
    int tab[6]={56,120,47,69,12,23};
    char *title="resultats de la chasse";
    int* data;
    data=GetData(tab);
    drawChart(data, title, label);
    return 0;
}
