// GRAY SCALE MESURE //

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"

// #include "pbPlots/pbPlots.h"
// #include "pbPlots/supportLib.h"

#include "pbPlots/pbPlots.c"     // Direct import des fonctions pour ne pas utiliser de makefile
#include "pbPlots/supportLib.c"

#define MAX_STR 200

int main(void){
    int width, height, channels;


    // **********  LOAD DE L'IMAGE  ********************** //

    char imageInput[MAX_STR] = "test_gray.jpg";   // Image  avec CHEMIN COMPLET (C:\\etc\\image.jpg ...) ou RELATIF (image.jpg) si présente dans le dossier

    unsigned char *img = stbi_load(imageInput, &width, &height, &channels, 0);   
    if(img == NULL){
        printf("Error of loading image\n");
        exit(1);
    }
    printf("Image loaded: width = %dpx ; height = %dpx ; channels = %d\n",width,height,channels);


    /********************************************************/

    // QUELQUES TEST ...
    // printf("PIXEL : %d\n",img[0]);
    

    // ****************  Matrice GRAYSCALE  ************** //
    uint8_t mat_img[width][height];
    size_t img_size = width * height * channels;
    unsigned int cpt = 0;
    uint8_t int_p;
    int l = 0;
    int c = 0;

    for(unsigned char *p = img; p != img + img_size; p += channels){
        // int_p = p;

        int_p = (uint8_t) ( ( *(p) + *(p + 3 ) + *(p + 2 ) )/3.0 );   // Conversion en Gray
        // printf("PIXEL: %u \n", int_p);
        cpt++;


        // Remplissage d'une matrice représentant l'image en gris
        if(c < width){
            mat_img[l][c] = int_p;
            c++;
            
            if(c == width){
                // printf("c=%d ; l = %d\n",c,l);
                c = 0;
                l++;
            }
        }

    }

    /****************************************************************/

    // QUELQUES TEST .....
    printf("Amount of pixels : %d\n",cpt);
    

    /**************** Profile de Gray sur segment AB ****************/

    double grayAB[width];  // Liste des pixels sur une ligne X horizontale de l'iamge
    double pixel[width];

    int segmentAB = 200;  // Position X de la ligne à sélectionner
    printf("Lign's position to analyze : %d\n",segmentAB);


    for(int c=0; c<width; c++){
        
        grayAB[c] = mat_img[segmentAB][c];
        pixel[c] = c; 

        // printf("Pixel %d\n", grayAB[l]);
    }

    /*****************************************************************/

    // *****************  Save + Draw plot  ******************* //

    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();

    StringReference *errorMessage = (StringReference *)malloc(sizeof(StringReference));

    _Bool success;
    success = DrawScatterPlot(imageRef, 800, 600, pixel, width, grayAB, width, errorMessage);


    if(success){
        size_t length;
        double *pngdata = ConvertToPNG(&length, imageRef->image);
        DeleteImage(imageRef->image);

        WriteToFile(pngdata, length, "myPlotResult.png");             // Nom du fichier de sortie
	}else{
        fprintf(stderr, "Error: ");
        for(int i = 0; i < errorMessage->stringLength; i++){
            fprintf(stderr, "%c", errorMessage->string[i]);
        }
        fprintf(stderr, "\n");
    }

    /**********************************************************/


    // Libération mémoire 
    stbi_image_free(img);

    return 0;
}