#ifndef Util
#define Util

<<<<<<< HEAD
#include <cmath> 
#include "includes.h"
=======
#include <cmath>

typedef struct Image {
   unsigned long sizeX;
   unsigned long sizeY;
   char* data;
} Image;

typedef struct RGB {
  GLubyte r;
  GLubyte g;
  GLubyte b;
} RGB;

using namespace std;

>>>>>>> added texture functions to Util.h and added grass texture for ground
namespace Util
{
    //Returns a random float 
    float randF()
    {
      return (float) rand() / RAND_MAX;
    }
    
    //Safely uses the glUniformMatrix4fv function
    void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[])
    {
      if(handle >= 0)
	glUniformMatrix4fv(handle, 1, GL_FALSE, data);
    }
    
    //Changes Degrees to radians
    float degreesToRadians(float degrees)
    {
      return degrees * 0.0174532925;
    }

    //changes radians to degrees.
    float radiansToDegrees(float radians)
    {
      return radians * 57.2957795;
    }
    
    void printVec3(glm::vec3 v)
    {
      printf("(%f, %f, %f)\n",v.x,v.y,v.z);
    }

<<<<<<< HEAD
    void printMat4(glm::mat4 m)
    {
      double dArray[16] = {0.0};

      const float *pSource = (const float*)glm::value_ptr(m);
      for (int i = 0; i < 16; ++i)
         dArray[i] = pSource[i];
   
      for (int i = 0; i < 16; ++i) {
         printf("%f ", dArray[i]);
         if ((i+1)%4 == 0) {
            printf("\n");
         }
      }
=======
    /* Texture Stuff */
    unsigned int getint(FILE *fp) {
       return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8) + (((unsigned int) getc(fp)) << 16) + (((unsigned int) getc(fp)) << 24);
    }

    unsigned int getshort(FILE *fp) {
       return ((unsigned int) getc(fp)) + (((unsigned int) getc(fp)) << 8);
    }

    int loadImage(char *filename, Image *image)
    {
       FILE *file;
       unsigned long size;                 // size of the image in bytes
       unsigned long i;                    // standard counter
       unsigned short int planes;          // number of planes in image (must be 1)
       unsigned short int bpp;             // number of bits per pixel (must be 24)
       char temp;                          // used to convert bgr to rgb color
  
       // make sure the file is there
       if ((file = fopen(filename, "rb"))==NULL) {
         printf("File Not Found : %s\n",filename);
         return 0;
       }
       // seek through the bmp header, up to the width height
       fseek(file, 18, SEEK_CUR);
       image->sizeX = getint (file); // read the width
       image->sizeY = getint (file); // read the height

       // calculate the size (assuming 24 bits or 3 bytes per pixel)
       size = image->sizeX * image->sizeY * 3;

       // read the planes
       planes = getshort(file);
       if (planes != 1) {
         printf("Planes from %s is not 1: %u\n", filename, planes);
         return 0;
       }
  
       // read the bpp
       bpp = getshort(file);
       if (bpp != 24) {
         printf("Bpp from %s is not 24: %u\n", filename, bpp);
         return 0;
       }
       // seek past the rest of the bitmap header
       fseek(file, 24, SEEK_CUR);

       // read the data
       image->data = (char *) malloc(size);
       if (image->data == NULL) {
         printf("Error allocating memory for color-corrected image data");
         return 0; 
       }
       if ((i = fread(image->data, size, 1, file)) != 1) {
         printf("Error reading image data from %s.\n", filename);
         return 0;
       }

       // reverse all of the colors. (bgr -> rgb)
       for (i=0;i<size;i+=3) {
         temp = image->data[i];
         image->data[i] = image->data[i+2];
         image->data[i+2] = temp;
       }
       fclose(file);
     
       return 1;
    }

    void loadTexture(char *filename, int texture_id)
    {
       Image* TextureImage;
       TextureImage = (Image *) malloc(sizeof(Image));

       if (TextureImage == NULL) {
          printf("Error allocating space for image");
          exit(1);
       }

       cout << "trying to load " << filename << endl;
       if (!loadImage(filename, TextureImage)) {
          exit(1);
       }

       glBindTexture(GL_TEXTURE_2D, texture_id);
       glTexImage2D(GL_TEXTURE_2D, 0, 3,
         TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // cheap scaling when image bigger than texture
       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // cheap scaling when image smalled than texture
>>>>>>> added texture functions to Util.h and added grass texture for ground
    }
}

#endif
