#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <string.h>
#include "GL/glut.h"

#define VIEWPORT_WIDTH              640
#define VIEWPORT_HEIGHT             480
#define KEY_ESCAPE                  27

CvCapture* g_Capture;
GLint g_hWindow;

GLvoid InitGL();
GLvoid OnDisplay();
GLvoid OnReshape(GLint w, GLint h);
GLvoid OnKeyPress (unsigned char key, GLint x, GLint y);
GLvoid OnIdle();

int main(int argc, char* argv[])
{
	// Create GLUT Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

	g_hWindow = glutCreateWindow("Video Texture");

	// Create OpenCV camera capture
	// If multiple cameras are installed, please choose correct index
	g_Capture = cvCreateCameraCapture(0);
	assert(g_Capture);

	// Initialize OpenGL
	InitGL();

	glutMainLoop();

	return 0;
}

GLvoid InitGL()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);

	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnReshape);
	glutKeyboardFunc(OnKeyPress);
	glutIdleFunc(OnIdle);
}

GLvoid OnDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	// Set Projection Matrix
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, 0);

	// Switch to Model View Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw a textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(VIEWPORT_WIDTH, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, VIEWPORT_HEIGHT);
	glEnd();

	glFlush();
	glutSwapBuffers();
}

GLvoid OnReshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
}

GLvoid OnKeyPress(unsigned char key, int x, int y)
{
	switch (key) {
	  case KEY_ESCAPE:
		  cvReleaseCapture(&g_Capture);
		  glutDestroyWindow(g_hWindow);
		  exit(0);
		  break;
	}
	glutPostRedisplay();
}

GLvoid OnIdle()
{
	// Capture next frame
	IplImage *image = cvQueryFrame(g_Capture);

	// Convert to RGB
	cvCvtColor(image, image, CV_BGR2RGB);

	// Create Texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);

	// Update View port
	glutPostRedisplay();
}