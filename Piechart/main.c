/* Bring in gd library functions */
#include "gd.h"
/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>

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

int drawChart(float *data, char *title) {
  /* Declare the image */
  gdImagePtr im;
  /* Declare output files */
  FILE *pngout, *jpegout;
  /* Declare color indexes */
  int black,beige,white,blue,red,green,yellow,purple;
  /* Allocate the image: 64 pixels across by 64 pixels tall */
  im = gdImageCreate(750,750);
  /* Allocate the color black (red, green and blue all minimum).
    Since this is the first color in a new image, it will
    be the background color. */
  black = gdImageColorAllocate(im, 0, 0, 0);
  beige = gdImageColorAllocate(im, 250, 200, 156);
  blue = gdImageColorAllocate(im, 41, 134, 205);
  red = gdImageColorAllocate(im, 225, 0, 0);
  green = gdImageColorAllocate(im, 150, 200, 125);
  yellow = gdImageColorAllocate(im, 255, 217, 100);
  purple = gdImageColorAllocate(im, 142, 124, 200);
  white = gdImageColorAllocate(im, 255, 255, 255);

  gdImageFilledRectangle(im,0,0,750,750,white);
  /* Draw a line from the upper left to the lower right,
    using white color index. */
  gdImageEllipse(im, 375, 375, 500, 500, black);
  gdImageFilledArc(im, 375, 375, 500, 500, 0, data[0], beige, gdPie);
  gdImageFilledArc(im, 375, 375, 500, 500, data[0], data[1], blue, gdPie);
  gdImageFilledArc(im, 375, 375, 500, 500, data[1], data[2], green, gdPie);
  gdImageFilledArc(im, 375, 375, 500, 500, data[2], data[3], red, gdPie);
  gdImageFilledArc(im, 375, 375, 500, 500, data[3], data[4], yellow, gdPie);
  gdImageFilledArc(im, 375, 375, 500, 500, data[4], data[5], purple, gdPie);
  gdImageString(im, 3, 600, 375, title, black);
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

char* lecture(){
    char* labels=(char*)malloc(sizeof(char*));
    int * data=(int*)malloc(sizeof(int*));
    char* title=(char*)malloc(sizeof(char*));

    printf("Bonjour, veuillez entrer vos données, label puis quantité");
    //scanf();

    return labels, data, title;
}

int main(){
    char* label[6]={"Perdrix, Canards, Lapins, Faisans, Cerfs, Sangliers"};
    int tab[6]={56,120,47,69,12,23};
    char title[]="resultats de la chasse";
    int* data;
    data=GetData(tab);
    drawChart(data, title);
    return 0;
}

