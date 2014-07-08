/* ============================================================================
**
** Demonstration of water rendering
** Copyright (C) 2005 Julien Guertault
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.zz
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**
** ========================================================================= */

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <jerror.h>
#include "noise.h"
#include "CG.h"
#include "model_obj.h"
//#include "screencasts.h"

#define RESOLUTION 64
#define PESO_ANDA 2

/*Variaveis para skybox*/
float camX=0.0,camY=0.0,camZ=5.0;
float camYaw=0.0;
float camPitch=0.0;
enum {SKY_LEFT=0,SKY_BACK,SKY_RIGHT,SKY_FRONT,SKY_TOP,SKY_BOTTOM}; //constants for the skybox faces, so we don't have to remember so much number
unsigned int skybox[6]; //the ids for the textures
#ifndef M_PI
#define M_PI 3.1415926535
#endif
void drawSkybox(float size);
void drawCube(float size);

static GLuint	texture;

static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;
static int	wire_frame = 0;
static int	normals = 0;
static int	xold = 0;
static int	yold = 0;
Model_OBJ *obj;
//static int tamanhox, tamanhoy;

//gluPerspective (20, width / (float) height, 0.1, 15);

static GLdouble obsX = -2.400000, obsY = 0.4, obsZ = 1.30;
int cont = 0;
static float	rotate_x = 30;
static float	rotate_y = 15;
static float	translate_z = 4;
float angle = 0.0;
static float	surface[6 * RESOLUTION * (RESOLUTION + 1)];
static float	normal[6 * RESOLUTION * (RESOLUTION + 1)];

float xpos = 0.568, ypos = 0.6351, zpos = 3.2824, xrot = 3, yrot = 68;

float lastx, lasty;


static float	z (const float x, const float y, const float t)
{
  const float x2 = x - 3;
  const float y2 = y + 1;
  const float xx = x2 * x2;
  const float yy = y2 * y2;
  return ((2 * sinf (20 * sqrtf (xx + yy) - 4 * t) +
Noise (10 * x, 10 * y, t, 0)) / 200);
}


/*
** Function to load a Jpeg file.
*/
int	load_texture (const char * filename,
unsigned char * dest,
const int format,
const unsigned int size)
{
  FILE *fd;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  unsigned char * line;

  cinfo.err = jpeg_std_error (&jerr);
  jpeg_create_decompress (&cinfo);

  if (0 == (fd = fopen(filename, "rb")))
    return 1;

  jpeg_stdio_src (&cinfo, fd);
  jpeg_read_header (&cinfo, TRUE);
  if ((cinfo.image_width != size) || (cinfo.image_height != size))
    return 1;

  if (GL_RGB == format)
    {
      if (cinfo.out_color_space == JCS_GRAYSCALE)
return 1;
    }
  else
    if (cinfo.out_color_space != JCS_GRAYSCALE)
      return 1;

  jpeg_start_decompress (&cinfo);

  while (cinfo.output_scanline < cinfo.output_height)
    {
      line = dest +
(GL_RGB == format ? 3 * size : size) * cinfo.output_scanline;
      jpeg_read_scanlines (&cinfo, &line, 1);
    }
  jpeg_finish_decompress (&cinfo);
  jpeg_destroy_decompress (&cinfo);
  return 0;
}

/*
** Function called to update rendering
*/
void	DisplayFunc (void)
{
  const float t = glutGet (GLUT_ELAPSED_TIME) / 1000.;
  const float delta = 15. / RESOLUTION;
  const unsigned int length = 2 * (RESOLUTION + 1);
  const float xn = (RESOLUTION + 1) * delta + 1;
  unsigned int i;
  unsigned int j;
  float x;
  float y;
  unsigned int indice;
  unsigned int preindice;

  /* Yes, I know, this is quite ugly... */
  float v1x;
  float v1y;
  float v1z;

  float v2x;
  float v2y;
  float v2z;

  float v3x;
  float v3y;
  float v3z;

  float vax;
  float vay;
  float vaz;

  float vbx;
  float vby;
  float vbz;

  float nx;
  float ny;
  float nz;

  float l;


  




  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawSkybox(20.0);
  glLoadIdentity ();
  glTranslatef (0, 0, -translate_z);
  ///glRotatef (rotate_y, 1, 0, 0);
  //glRotatef (rotate_x, 0, 1, 0);

  // gluLookAt(0,0,1,rotate_x,1,0, 0,1,0);
  // gluLookAt(0,0,1,0,rotate_y,0, 0,1,0);
 
  //drawCube(20);
  if(xrot >= 3 ){
    glRotatef(xrot,1.0,0.0,0.0); //rotate our camera on teh x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0); //rotate our camera on the y-axis (up and down)
  }
  glTranslated(-xpos,-0.470175,-zpos); //translate the screen to the position of our camera

  


  // printf("xpos: %f, ypos: %f, zpos: %f\n\n",xpos,ypos,zpos);
  // printf("xrot: %f, yrot: %f, zrot: %f\n\n",xrot,yrot,zpos);

/*
glTranslatef ( obsX , obsY , obsZ ) ;
printf("x: %f y: %f, z: %f",obsX,obsY,obsZ);
printf("\n\n");
glRotatef ( 0 ,1 ,0 ,0);
glRotatef ( 0 ,0 ,1 ,0);*/
  //gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,0,1);
  //gluLookAt(0,0,1, obsX,obsY,obsZ, 0,1,0);
  //glMatrixMode(GL_MODELVIEW);
  //glutPostRedisplay();

  /* Vertices */
  for (j = 0; j < RESOLUTION; j++)
    {
      y = (j + 1) * delta - 1;
      for (i = 0; i <= RESOLUTION; i++)
{
indice = 6 * (i + j * (RESOLUTION + 1));

x = i * delta - 1;
surface[indice + 3] = x;
surface[indice + 4] = z (x, y, t);
surface[indice + 5] = y;
if (j != 0)
{
/* Values were computed during the previous loop */
preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
surface[indice] = surface[preindice + 3];
surface[indice + 1] = surface[preindice + 4];
surface[indice + 2] = surface[preindice + 5];
}
else
{
surface[indice] = x;
surface[indice + 1] = z (x, -1, t);
surface[indice + 2] = -1;
}
}
    }

  /* Normals */
  for (j = 0; j < RESOLUTION; j++)
    for (i = 0; i <= RESOLUTION; i++)
      {
indice = 6 * (i + j * (RESOLUTION + 1));

v1x = surface[indice + 3];
v1y = surface[indice + 4];
v1z = surface[indice + 5];

v2x = v1x;
v2y = surface[indice + 1];
v2z = surface[indice + 2];

if (i < RESOLUTION)
{
v3x = surface[indice + 9];
v3y = surface[indice + 10];
v3z = v1z;
}
else
{
v3x = xn;
v3y = z (xn, v1z, t);
v3z = v1z;
}

vax = v2x - v1x;
vay = v2y - v1y;
vaz = v2z - v1z;

vbx = v3x - v1x;
vby = v3y - v1y;
vbz = v3z - v1z;

nx = (vby * vaz) - (vbz * vay);
ny = (vbz * vax) - (vbx * vaz);
nz = (vbx * vay) - (vby * vax);

l = sqrtf (nx * nx + ny * ny + nz * nz);
if (l != 0)
{
l = 1 / l;
normal[indice + 3] = nx * l;
normal[indice + 4] = ny * l;
normal[indice + 5] = nz * l;
}
else
{
normal[indice + 3] = 0;
normal[indice + 4] = 1;
normal[indice + 5] = 0;
}


if (j != 0)
{
/* Values were computed during the previous loop */
preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
normal[indice] = normal[preindice + 3];
normal[indice + 1] = normal[preindice + 4];
normal[indice + 2] = normal[preindice + 5];
}
else
{
/* v1x = v1x; */
v1y = z (v1x, (j - 1) * delta - 1, t);
v1z = (j - 1) * delta - 1;

/* v3x = v3x; */
v3y = z (v3x, v2z, t);
v3z = v2z;

vax = v1x - v2x;
vay = v1y - v2y;
vaz = v1z - v2z;

vbx = v3x - v2x;
vby = v3y - v2y;
vbz = v3z - v2z;

nx = (vby * vaz) - (vbz * vay);
ny = (vbz * vax) - (vbx * vaz);
nz = (vbx * vay) - (vby * vax);

l = sqrtf (nx * nx + ny * ny + nz * nz);
if (l != 0)
{
l = 1 / l;
normal[indice] = nx * l;
normal[indice + 1] = ny * l;
normal[indice + 2] = nz * l;
}
else
{
normal[indice] = 0;
normal[indice + 1] = 1;
normal[indice + 2] = 0;
}
}
      }


  /* The ground */
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glDisable (GL_TEXTURE_2D);
  glColor3f (1, 0.9, 0.7);
  glBegin (GL_TRIANGLE_FAN);
  glVertex3f (-1, 0, -1);
  glVertex3f (-1, 0, 1);
  glVertex3f ( 1, 0, 1);
  glVertex3f ( 1, 0, -1);
  glEnd ();

  glTranslatef (0, 0.2, 0);

  //display(angle);

 // glPushMatrix();
  obj->Draw();
  //glPopMatrix();

  angle += 0.5;
  if(angle > 360)
    angle -= 360;



  /* Render wireframe? */
  if (wire_frame != 0)
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

  /* The water */
  glEnable (GL_TEXTURE_2D);
  glColor3f (1, 1, 1);
  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_VERTEX_ARRAY);
  glNormalPointer (GL_FLOAT, 0, normal);
  glVertexPointer (3, GL_FLOAT, 0, surface);
  for (i = 0; i < RESOLUTION; i++)
    glDrawArrays (GL_TRIANGLE_STRIP, i * length, length);

  /* Draw normals? */
  if (normals != 0)
    {
      glDisable (GL_TEXTURE_2D);
      glColor3f (1, 0, 0);
      glBegin (GL_LINES);
      for (j = 0; j < RESOLUTION; j++)
for (i = 0; i <= RESOLUTION; i++)
{
indice = 6 * (i + j * (RESOLUTION + 1));
glVertex3fv (&(surface[indice]));
glVertex3f (surface[indice] + normal[indice] / 50,
surface[indice + 1] + normal[indice + 1] / 50,
surface[indice + 2] + normal[indice + 2] / 50);
}

      glEnd ();
    }
    
  /* End */
  glFlush ();
  glutSwapBuffers ();
  glutPostRedisplay ();
}

/*
** Function called when the window is created or resized
*/
void	ReshapeFunc (int width, int height)
{
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity ();
  gluPerspective (50, width / (float) height, 0.5, 20);
  glViewport (0, 0, width, height);
  glMatrixMode(GL_MODELVIEW);
  glutPostRedisplay();
}

/*
** Function called when a key is hit
*/
void	KeyboardFunc (unsigned char key, int x, int y)
{
  xold = x; /* Has no effect: just to avoid a warning */
  yold = y;
  
  

  switch (key)
    {
    case 'q':
    case 'Q':
    case 27:
      exit (0);
      break;
    case 'f':
    case 'F':
    case 'p':
    case 'P':
      wire_frame = 0;
      break;
    case 'l':
    case 'L':
      wire_frame = 1;
      break;
    case 'n':
    case 'N':
      if (0 == normals)
normals = 1;
      else
normals = 0;
      break;

    case 'A' :
    case 'a' :
              obsX -=0.1;
              break;
    case 'D' :
    case 'd' :
              obsX +=0.1;
              break;
    case 'W' :
    case 'w' :
              obsY +=0.1;
              break;
    case 's' :
    case 'S' :
              obsY -=0.1;
              break;

    case 'T' :
    case 't' :
              obsZ +=0.1;
              break;
    case 'g' :
    case 'G' :
              obsZ -=0.1;
              break;

    }

}

/*
** Function called when a mouse button is hit
*/
void	MouseFunc (int button, int state, int x, int y)
{
  if (GLUT_LEFT_BUTTON == button)
    left_click = state;
  if (GLUT_RIGHT_BUTTON == button)
    right_click = state;
  xold = x;
  yold = y;
}




/*
** Function called when the mouse is moved
*/
void	MotionFunc (int x, int y)
{
  if (GLUT_DOWN == left_click)
    {
      rotate_y = rotate_y + (y - yold) / 10.0;
      rotate_x = rotate_x + (x - xold) / 10.0;
      if (rotate_y > 90)
rotate_y = 90;
      if (rotate_y < -90)
rotate_y = -90;
      glutPostRedisplay ();
    }
  if (GLUT_DOWN == right_click)
    {
      rotate_x = rotate_x + (x - xold) / 5.;
      translate_z = translate_z +
(yold - y) / 50.;
      if (translate_z < 0.5)
translate_z = 0.5;
      if (translate_z > 10)
translate_z = 10;
      glutPostRedisplay ();
    }
  xold = x;
  yold = y;
}



void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy/50; //set the xrot to xrot with the addition of the difference in the y position
    yrot += (float) diffx/5; //set the xrot to yrot with the addition of the difference in the x position
}



void keyboard (unsigned char key, int x, int y) {
    x += y;

    if (key=='q')
    {
    xrot += 1;
    if (xrot >360)
      xrot -= 360;
    }

    if (key=='z')
    {
    xrot -= 1;
    if (xrot < -360)
      xrot += 360;
    }

    if (key=='w')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos += (float) sin(yrotrad)/PESO_ANDA ;
    zpos -= (float) cos(yrotrad)/PESO_ANDA ;
    if(ypos > 0.1)
      ypos -= (float)sin(xrotrad);
    //printf("xpos: %f, ypos: %f, zpos: %f\n\n",xpos,ypos,zpos);
    }

    if (key=='s')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos -= (float)sin(yrotrad)/PESO_ANDA;
    zpos += (float)cos(yrotrad)/PESO_ANDA;
    if(ypos < 0.7)
      ypos += (float)sin(xrotrad);
    //printf("xpos: %f, ypos: %f, zpos: %f\n\n",xpos,ypos,zpos);
    }

    if (key=='d')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos += (float)cos(yrotrad) * 0.2/PESO_ANDA;
    zpos += (float)sin(yrotrad) * 0.2/PESO_ANDA;
    }

    if (key=='a')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos -= (float)cos(yrotrad) * 0.2/PESO_ANDA;
    zpos -= (float)sin(yrotrad) * 0.2/PESO_ANDA;
    }


    if (key==27)
    {
    exit(0);
    }
}




unsigned int loadTexBMP( const char * filename )
{

  unsigned int texture;

  int width, height;

  unsigned char * data;

  FILE * file;

  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;
  width = 1024;
  height = 1024;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );
  int i;
 for(i = 0; i < width * height ; ++i)
{
   int index = i*3;
   unsigned char B,R;
   B = data[index];
   R = data[index+2];

   data[index] = R;
   data[index+2] = B;

}


glGenTextures( 1, &texture );
glBindTexture( GL_TEXTURE_2D, texture );
glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
free( data );

return texture;
}







void initSkybox(void) {
/*
SKY_FRONT 0
SKY_RIGHT 1
SKY_LEFT 2
SKY_BACK 3
SKY_UP 4
SKY_DOWN 5
*/
skybox[SKY_FRONT] = loadTexBMP("txStormydays_front.bmp");
skybox[SKY_RIGHT] = loadTexBMP("txStormydays_right.bmp");
skybox[SKY_LEFT] = loadTexBMP("txStormydays_left.bmp");
skybox[SKY_BACK] = loadTexBMP("txStormydays_back.bmp");
skybox[SKY_TOP] = loadTexBMP("txStormydays_up.bmp");
skybox[SKY_BOTTOM] = loadTexBMP("txStormydays_down.bmp");

}

void killskybox(void){
        glDeleteTextures(6,&skybox[0]);
}


void drawSkybox(float size)
{
        int b1=glIsEnabled(GL_TEXTURE_2D); //new, we left the textures turned on, if it was turned on
        glDisable(GL_LIGHTING); //turn off lighting, when making the skybox
        glDisable(GL_DEPTH_TEST); //turn off depth texting
        glEnable(GL_TEXTURE_2D); //and turn on texturing
        glBindTexture(GL_TEXTURE_2D,skybox[SKY_BACK]); //use the texture we want
        glBegin(GL_QUADS); //and draw a face
                //back face
                glTexCoord2f(0,0); //use the correct texture coordinate
                glVertex3f(size/2,size/2,size/2); //and a vertex
                glTexCoord2f(1,0); //and repeat it...
                glVertex3f(-size/2,size/2,size/2);
                glTexCoord2f(1,1);
                glVertex3f(-size/2,-size/2,size/2);
                glTexCoord2f(0,1);
                glVertex3f(size/2,-size/2,size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,skybox[SKY_LEFT]);
        glBegin(GL_QUADS);
                //left face
                glTexCoord2f(0,0);
                glVertex3f(-size/2,size/2,size/2);
                glTexCoord2f(1,0);
                glVertex3f(-size/2,size/2,-size/2);
                glTexCoord2f(1,1);
                glVertex3f(-size/2,-size/2,-size/2);
                glTexCoord2f(0,1);
                glVertex3f(-size/2,-size/2,size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,skybox[SKY_FRONT]);
        glBegin(GL_QUADS);
                //front face
                glTexCoord2f(1,0);
                glVertex3f(size/2,size/2,-size/2);
                glTexCoord2f(0,0);
                glVertex3f(-size/2,size/2,-size/2);
                glTexCoord2f(0,1);
                glVertex3f(-size/2,-size/2,-size/2);
                glTexCoord2f(1,1);
                glVertex3f(size/2,-size/2,-size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,skybox[SKY_RIGHT]);
        glBegin(GL_QUADS);
                //right face
                glTexCoord2f(0,0);
                glVertex3f(size/2,size/2,-size/2);
                glTexCoord2f(1,0);
                glVertex3f(size/2,size/2,size/2);
                glTexCoord2f(1,1);
                glVertex3f(size/2,-size/2,size/2);
                glTexCoord2f(0,1);
                glVertex3f(size/2,-size/2,-size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,skybox[SKY_TOP]);
        glBegin(GL_QUADS); //top face
                glTexCoord2f(1,0);
                glVertex3f(size/2,size/2,size/2);
                glTexCoord2f(0,0);
                glVertex3f(-size/2,size/2,size/2);
                glTexCoord2f(0,1);
                glVertex3f(-size/2,size/2,-size/2);
                glTexCoord2f(1,1);
                glVertex3f(size/2,size/2,-size/2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,skybox[SKY_BOTTOM]);
        glBegin(GL_QUADS);
                //bottom face
                glTexCoord2f(1,1);
                glVertex3f(size/2,-size/2,size/2);
                glTexCoord2f(0,1);
                glVertex3f(-size/2,-size/2,size/2);
                glTexCoord2f(0,0);
                glVertex3f(-size/2,-size/2,-size/2);
                glTexCoord2f(1,0);
                glVertex3f(size/2,-size/2,-size/2);
        glEnd();
        glEnable(GL_LIGHTING); //turn everything back, which we turned on, and turn everything off, which we have turned on.
        glEnable(GL_DEPTH_TEST);
        if(!b1)
                glDisable(GL_TEXTURE_2D);
}

void drawCube(float size)
{
        float difamb[]={1.0,0.5,0.3,1.0};
        glBegin(GL_QUADS);
                //front face
                glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,difamb);
                glNormal3f(0.0,0.0,1.0);
                glVertex3f(size/2,size/2,size/2);
                glVertex3f(-size/2,size/2,size/2);
                glVertex3f(-size/2,-size/2,size/2);
                glVertex3f(size/2,-size/2,size/2);
 
                //left face
                glNormal3f(-1.0,0.0,0.0);
                glVertex3f(-size/2,size/2,size/2);
                glVertex3f(-size/2,size/2,-size/2);
                glVertex3f(-size/2,-size/2,-size/2);
                glVertex3f(-size/2,-size/2,size/2);
 
                //back face
                glNormal3f(0.0,0.0,-1.0);
                glVertex3f(size/2,size/2,-size/2);
                glVertex3f(-size/2,size/2,-size/2);
                glVertex3f(-size/2,-size/2,-size/2);
                glVertex3f(size/2,-size/2,-size/2);
 
                //right face
                glNormal3f(1.0,0.0,0.0);
                glVertex3f(size/2,size/2,-size/2);
                glVertex3f(size/2,size/2,size/2);
                glVertex3f(size/2,-size/2,size/2);
                glVertex3f(size/2,-size/2,-size/2);
 
                //top face
                glNormal3f(0.0,1.0,0.0);
                glVertex3f(size/2,size/2,size/2);
                glVertex3f(-size/2,size/2,size/2);
                glVertex3f(-size/2,size/2,-size/2);
                glVertex3f(size/2,size/2,-size/2);
 
                //bottom face
                glNormal3f(0.0,-1.0,0.0);
                glVertex3f(size/2,-size/2,size/2);
                glVertex3f(-size/2,-size/2,size/2);
                glVertex3f(-size/2,-size/2,-size/2);
                glVertex3f(size/2,-size/2,-size/2);
        glEnd();
}





int	main (int narg, char ** args)
{
  unsigned char total_texture[4 * 256 * 256];
  unsigned char alpha_texture[256 * 256];
  unsigned char caustic_texture[3 * 256 * 256];
  unsigned int i;

  InitNoise ();

  /* Creation of the window */
  glutInit (&narg, args);
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutCreateWindow ("Water");

  /* OpenGL settings */
  glClearColor (0, 0, 0, 0);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);
  glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  leituraVertices();
  initSkybox();
  obj = new Model_OBJ();
  obj->Load((char*)"palm_tree.obj");

  
  //obsX = 20;
  //obsY =
//static GLdouble obsX=20, obsY = width/(float) height, obsZ = 0.1;


  /* Texture loading */
  glGenTextures (1, &texture);
  if (load_texture ("alpha.jpg", alpha_texture, GL_ALPHA, 256) != 0 ||
      load_texture ("reflection.jpg", caustic_texture, GL_RGB, 256) != 0)
    return 1;
  for (i = 0; i < 256 * 256; i++)
    {
      total_texture[4 * i] = caustic_texture[3 * i];
      total_texture[4 * i + 1] = caustic_texture[3 * i + 1];
      total_texture[4 * i + 2] = caustic_texture[3 * i + 2];
      total_texture[4 * i + 3] = alpha_texture[i];
    }
  glBindTexture (GL_TEXTURE_2D, texture);
  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_RGBA,
GL_UNSIGNED_BYTE, total_texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glEnable (GL_TEXTURE_GEN_S);
  glEnable (GL_TEXTURE_GEN_T);
  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  
  /* Declaration of the callbacks */
  glutDisplayFunc (&DisplayFunc);
  glutReshapeFunc (&ReshapeFunc);
  //glutKeyboardFunc (&KeyboardFunc);
  glutKeyboardFunc (&keyboard);
  glutMouseFunc (&MouseFunc);
  glutMotionFunc (&MotionFunc);
  glutPassiveMotionFunc(mouseMovement);

  /* Loop */
  glutMainLoop ();

  /* Never reached */
  return 0;
}

/* ========================================================================= */