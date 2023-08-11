/* Bring in gd library functions */
#include <gd.h>
#include <gdfonts.h>

/* Bring in standard I/O so we can output the PNG to a file */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

int* GetData(int data[], int nbarg){
    int total =0;
    int *donnees=data;
    float res[nbarg];
    static float curseur[sizeof donnees/sizeof(int)];

    for (int i =0; i<nbarg ;i++){
         total+=donnees[i];
    }
    float div= 360/(float)total;

    for (int j =0; j<nbarg;j++){
        res[j]=(donnees[j]*div);
        if (j==0){
          curseur[j]=res[j];
        }else{
          curseur[j]=curseur[j-1]+res[j];
          printf("%f\n",curseur[j]);
        }
    }
    return (int*)curseur;
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
    char* lab=label;
    double median = (angleStart + angleEnd) / 2.0;
    double labelangle;
    if (cos(median * M_PI / 180) < 0){
        labelangle = size/1.8 + strlen(label)*(10/1.2);
        }else{
        labelangle=size/1.8;
    }
    int labelX = centerX + labelangle * cos(median * M_PI / 180);
    int labelY = centerY + size/1.8 * sin(median * M_PI / 180);
    int pinX = centerX + size/2 * cos(median * M_PI / 180);
    int pinY = centerY + size/2 * sin(median * M_PI / 180);
    int pinXend = centerX + size/1.8 * cos(median * M_PI / 180);
    int pinYend = labelY;
    gdImageString(im, font , labelX, labelY, lab, color);
    gdImageLine(im, pinX, pinY, pinXend, pinYend, color);
}

//int drawChart(int *data, const char *title,char* label[],int nbarg) {
//    srand(time(NULL));
//  /* Declare the image */
//    int largeur=750;
//    int hauteur=750;
//    int largeurdiag= (largeur)/2;
//    int hauteurdiag=(hauteur)/2;
//    char* lab[nbarg];
//    for (int i=0;i<nbarg;i++){
//        lab[i]=strdup(label[i]);
//    }
//  gdImagePtr im;
//  /* Declare output files */
//  FILE *pngout, *jpegout;
//  /* Declare color indexes */
//  int black,coloralea,white;
//  /* Allocate the image: 64 pixels across by 64 pixels tall */
//  im = gdImageCreate(largeur,hauteur);

//  /* Allocate the color black (red, green and blue all minimum).
//    Since this is the first color in a new image, it will
//    be the background color. */
//  //enum couleur {black,beige,blue,red,green,yellow,purple,white};
//  black = gdImageColorAllocate(im, 0, 0, 0);
//  white = gdImageColorAllocate(im, 255, 255, 255);

//  gdImageFilledRectangle(im,0,0,largeur,hauteur,white);
//  /* Draw a line from the upper left to the lower right,
//    using white color index. */

//  int centerX=largeur/2;
//  int centerY=hauteur/2;
//  int centerXalt=largeur/2+(5*largeur/100);
//  int centerYalt=hauteur/2+(2.5*hauteur/100);

//  for (int i=0; i<6;i++){
//      coloralea=gdImageColorAllocate(im,rand()%256,rand()%256,rand()%256);
//      /*the first segment is reparated from the rest of the diagram*/
//      if (i==0){
//          //printf("%s\n",lab[i]);
//          createSegment(im,centerXalt,centerYalt,largeurdiag,hauteurdiag,0,data[i],black,coloralea);
//          createLabel(im,centerXalt,centerYalt,largeurdiag,0,data[i],black,lab[i]);
//      }else{
//          createSegment(im,centerX,centerY,largeurdiag,hauteurdiag,data[i-1],data[i],black,coloralea);
//          createLabel(im,centerX,centerY,largeurdiag,data[i-1],data[i],black,lab[i]);
//      }
//   }
//  //gdImageString(im, font , (1*largeur)/10, (1*hauteur)/10, &lab[0], black);


//  /* write and register the output file */
//  pngout = fopen(title, "wb");
//  gdImagePng(im, pngout);


//  /* Close the file. */
//  fclose(pngout);

//  /* Destroy the image in memory. */
//  gdImageDestroy(im);
//}*/

int main(int argc, char* argv[]){
 /*declaration des différentes valeurs utilisées pour créer le diagramme*/
    int largeur=750;
    int hauteur=750;
    int largeurdiag= (largeur)/2;
    int hauteurdiag=(hauteur)/2;
    int black,coloralea,white;
    int centerX=largeur/2;
    int centerY=hauteur/2;
    int centerXalt=largeur/2+(5*largeur/100);
    int centerYalt=hauteur/2+(2.5*hauteur/100);

    gdImagePtr im;
    FILE *pngout;
    im = gdImageCreate(largeur,hauteur);

    /*récupere la valeur utilisée pour le titre*/
    const char *title = strcat(argv[1],".png");

    /*récupere les données pondérales passées en arguments*/
    int curseurAngle=0;
    int total = 0;
        for (int i = 2; i < argc - 1; i += 2) {
            total += atoi(argv[i]);
        }
    black = gdImageColorAllocate(im, 0, 0, 0);
    white = gdImageColorAllocate(im, 255, 255, 255);
    gdImageFilledRectangle(im,0,0,largeur,hauteur,white);
    for (int i=2; i<argc; i+=2){
        coloralea=gdImageColorAllocate(im,rand()%256,rand()%256,rand()%256);
        double angle = atof(argv[i]);
        int curseurAngleEnd = curseurAngle + (360 * angle / total);
//        if (i == 2){

//            createSegment(im,centerXalt,centerYalt,largeurdiag,hauteurdiag,curseurAngle,curseurAngleEnd,black,coloralea);
//            createLabel(im, centerXalt, centerYalt, largeurdiag,curseurAngle, curseurAngleEnd,black,argv[i+1]);
//        }else{

            createSegment(im,centerX,centerY,largeurdiag,hauteurdiag,curseurAngle,curseurAngleEnd,black,coloralea);
            createLabel(im, centerX, centerY, largeurdiag,curseurAngle, curseurAngleEnd,black,argv[i+1]);
//        }
        curseurAngle=curseurAngleEnd;
    }


    /* write and register the output file */
    pngout = fopen(title, "wb");
    gdImagePng(im, pngout);


    /* Close the file. */
    fclose(pngout);

    /* Destroy the image in memory. */
    gdImageDestroy(im);

    return 0;
}
