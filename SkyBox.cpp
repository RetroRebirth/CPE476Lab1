#include "SkyBox.h"
using namespace std;

SkyBox::SkyBox(GLint _h_aPos, GLint _h_aNor, GLint _h_uM, GLint _h_uTexUnit, GLint _h_aTexCoord)
{
   // Defined attribute values
   h_aPos = _h_aPos;
   h_aNor = _h_aNor;
   h_uM = _h_uM;
   h_uTexUnit = _h_uTexUnit;
   h_aTexCoord = _h_aTexCoord;

   init();
   loadTexture((char *)"cloud5.bmp", 0);
}

SkyBox::~SkyBox()
{
   // Delete stuff
}

void SkyBox::init()
{
   // Position array of sky box
   GLfloat CubePos[] = {
      -1.5, -1.5, -1.5, //back face 5 verts :0 - clockwise 
      -1.5,  1.5, -1.5,
       1.5,  1.5, -1.5,
       1.5, -1.5, -1.5,
       1.5, -1.5,  1.5, //right face 5 verts :4
       1.5,  1.5,  1.5,
       1.5,  1.5, -1.5,
       1.5, -1.5, -1.5,
      -1.5, -1.5,  1.5, //front face 4 verts :8
      -1.5,  1.5,  1.5,
       1.5,  1.5,  1.5,
       1.5, -1.5,  1.5,
      -1.5, -1.5, -1.5, //left face 4 verts :12
      -1.5,  1.5, -1.5,
      -1.5,  1.5,  1.5,
      -1.5, -1.5,  1.5
   };
   // Amplify sky box size
   for (int i = 0, last = sizeof(CubePos)/sizeof(GLfloat); i < last; i++) {
      CubePos[i] *= SIZE;
   }

   // Index array of sky box   
   unsigned short idx[] = {0, 1, 2,  2, 3, 0,  4, 5, 6, 6, 7, 4,  8, 9, 10, 10, 11, 8,  12, 13, 14, 14, 15, 12};

   // Texture index mapping of sky box
   static GLfloat CubeTex[] = {
      0, 0, // back 
      0, 1,
      1, 1,
      1, 0,
      0, 0, //right 
      0, 1,
      1, 1,
      1, 0,
      0, 0, //front 
      0, 1,
      1, 1,
      1, 0,
      0, 0, // left 
      0, 1,
      1, 1,
      1, 0
   };

   glGenBuffers(1, &boxBufIDs.pos);
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.pos);
   glBufferData(GL_ARRAY_BUFFER, sizeof(CubePos), CubePos, GL_STATIC_DRAW);

   glGenBuffers(1, &boxBufIDs.ind);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxBufIDs.ind);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

   glGenBuffers(1, &boxBufIDs.nor);
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.nor);
   glBufferData(GL_ARRAY_BUFFER, sizeof(CubeTex), CubeTex, GL_STATIC_DRAW);
}

void SkyBox::loadTexture(char* image_file, int texID)
{
   TextureImage = (Image *) malloc(sizeof(Image));

   if (TextureImage == NULL) {
      printf("Error allocating space for image");
      exit(1);
   }

   cout << "trying to load " << image_file << endl;
   if (!loadImage(image_file, TextureImage)) {
      exit(1);
   }

   glBindTexture(GL_TEXTURE_2D, texID);
   glTexImage2D(GL_TEXTURE_2D, 0, 3,
   TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // cheap scaling when image bigger than texture
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // cheap scaling when image smalled than texture
}

int SkyBox::loadImage(char *filename, Image *image) {
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

void SkyBox::draw(Camera* camera)
{
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(h_uTexUnit, 0);

   glBindTexture(GL_TEXTURE_2D, 0);

   // Bind position buffer
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.pos);
   glVertexAttribPointer(h_aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   // Bind index buffer
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxBufIDs.ind);
   // Bind texture buffer
   GLSL::enableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, boxBufIDs.nor);
   glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0); 

   // Apply translation
   glm::vec3 trans = camera->pos;
   glm::mat4 T = glm::translate(glm::mat4(1.0f), trans);
   safe_glUniformMatrix4fv(h_uM, glm::value_ptr(T));

   glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_SHORT, 0);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
