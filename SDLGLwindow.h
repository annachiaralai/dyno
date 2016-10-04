#ifndef SDLGLWINDOW_H
#define SDLGLWINDOW_H

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_opengl.h"

class SDLGLwindow
{
 int w,h,bp;
 SDL_Surface *screen;
 TTF_Font* font;
 SDL_Color color;
 public:
 SDLGLwindow(const char* app_name, int ww, int hh, int bpp,const char* font_name,int font_size);
 ~SDLGLwindow(){TTF_CloseFont(font);}
 void EndText();
 void BeginText();
 void Text(const char *text, int x, int y, int r, int g, int b);
 void resize(int width, int height);
 void WireSphere(float x, float y, float z, float radius,int subdivisions, int r,int g, int b,int a);
 void SolidSphere(float x, float y, float z, float radius,int subdivisions, int r,int g, int b,int a);
 void Axis(float x, float y, float z,float s);
};

void SDLGLwindow :: resize(int width, int height)
{
 screen = SDL_SetVideoMode(width,height, bp, SDL_SWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL|SDL_RESIZABLE);
 w=width; h=height;
 glViewport(0, 0, width, height);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(45., (GLdouble) width/(GLdouble) height, 1., 1000.);
}

SDLGLwindow :: SDLGLwindow(const char* app_name, int ww, int hh, int bpp,const char* font_name,int font_size)
{
 w=ww; h=hh; bp=bpp;

 // SDL initialization
 if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
 { printf("Unable to initialize SDL: %s\n", SDL_GetError()); exit(0); }

 // Window initialization
 SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
 screen = SDL_SetVideoMode( w, h, bp, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL |SDL_RESIZABLE); 
 if ( !screen )
 { printf("Unable to set video mode: %s\n", SDL_GetError()); exit(0); }

 SDL_WM_SetCaption(app_name,0);

 // OpenGL initialization
 glClearColor( 0, 0, 0, 0 );
 resize(w,h);

 glMatrixMode( GL_MODELVIEW );
 glLoadIdentity();

 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 glEnable(GL_BLEND);
 glShadeModel(GL_SMOOTH);
 glEnable(GL_DEPTH_TEST);

 ////////////////////////////////////////////////////////////////////////////////
 // LUCI
 GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
 GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
 GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
 glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
 glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
 glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
 glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 glEnable(GL_LIGHT0);

 ////////////////////////////////////////////////////////////////////////////////
 // MATERIALI
 GLfloat mat_ambient[]    = { 0.2f, 0.2f, 0.0f, 1.0f };
 GLfloat mat_diffuse[]    = { 0.2f, 0.2f, 1.0f, 1.0f };
 GLfloat mat_specular[]   = { 0.8f, 0.8f, 0.8f, 1.0f };
 GLfloat high_shininess[] = { 35.0f };
 glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
 glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
 glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
 glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
 glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
 glEnable(GL_COLOR_MATERIAL);

 // Timer initialization
 if ( SDL_Init(SDL_INIT_TIMER) != 0 )
 { printf("Unable to initialize Timers: %s\n", SDL_GetError()); exit(0); }

 // TTF initialization
 if ( TTF_Init() != 0 )
 { printf("Unable to initialize TTF: %s\n", SDL_GetError()); exit(0); }

 // Loading font
 if(!(font = TTF_OpenFont(font_name, font_size)))
 { printf("Error loading font: %s\n", TTF_GetError()); exit(0); }
}

void SDLGLwindow :: BeginText()
{
 glEnable(GL_TEXTURE_2D);
 glMatrixMode( GL_PROJECTION );
 glPushMatrix();
 glLoadIdentity();
 glOrtho( 0, w, h, 0, 1, -1 );
 glMatrixMode( GL_MODELVIEW );
 glPushMatrix();
 glLoadIdentity();
}

void SDLGLwindow :: EndText()
{
 glMatrixMode( GL_MODELVIEW );
 glPopMatrix();
 glMatrixMode( GL_PROJECTION );
 glPopMatrix();
 glDisable(GL_TEXTURE_2D);
}

void SDLGLwindow :: Text(const char *text, int x, int y, int r, int g, int b)
{
 color.r = r; color.g = g; color.b = b;
 SDL_Surface *surface;
 GLenum texture_format;
 GLuint texture;
 GLint  nOfColors;
 if ((surface = TTF_RenderText_Blended(font, text, color)))
 { 
  nOfColors = surface->format->BytesPerPixel;
  if (nOfColors == 4)   
  {
   if (surface->format->Rmask == 0x000000ff)
    texture_format = GL_RGBA;
   else
    texture_format = GL_BGRA;
  }
  else if (nOfColors == 3)     
  {
   if (surface->format->Rmask == 0x000000ff)
    texture_format = GL_RGB;
   else
    texture_format = GL_BGR;
  }
  else
  {
   printf("SDL could not recognize image bitplanes\n");
   SDL_Quit();
   exit(0);
  }
  glGenTextures( 1, &texture );
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels );
 }
 else 
 {
  printf("SDL could not generate texture: %s\n", SDL_GetError());
  SDL_Quit();
  exit(0);
 }   
 
 int wt=surface->w; int ht=surface->h;
 if ( surface ) SDL_FreeSurface( surface );

 glBegin(GL_QUADS);
  glTexCoord2i(0, 0); glVertex2f(x , y);
  glTexCoord2i(1, 0); glVertex2f(x + wt, y);
  glTexCoord2i(1, 1); glVertex2f(x + wt, y + ht);
  glTexCoord2i(0, 1); glVertex2f(x , y + ht);
 glEnd();

 glDeleteTextures(1, &texture);
}

void SDLGLwindow :: SolidSphere(float x, float y, float z, float radius,int subdivisions, int r,int g, int b,int a)
{
 glColor4f(r/255.,g/255.,b/255.,a/255.);
 GLUquadricObj *quadric=gluNewQuadric();
 gluQuadricNormals(quadric, GLU_SMOOTH);
 glPushMatrix();
 glTranslatef( x,y,z );
 gluSphere(quadric, radius, subdivisions,subdivisions);
 glPopMatrix();
 gluDeleteQuadric(quadric);
 glColor4f(1,1,1,1);
}

void SDLGLwindow :: WireSphere(float x, float y, float z, float radius,int subdivisions, int r,int g, int b,int a)
{
 glColor4f(r/255.,g/255.,b/255.,a/255.);
 GLUquadricObj *quadric = gluNewQuadric();
 gluQuadricNormals(quadric, GLU_SMOOTH);
 glPushMatrix();
 glTranslatef( x,y,z );
 gluQuadricDrawStyle(quadric, GLU_LINE); /* wireframe */
 gluSphere( quadric, radius, subdivisions,subdivisions);
 glPopMatrix();
 gluDeleteQuadric(quadric);
 glColor4f(1,1,1,1);
}

void SDLGLwindow :: Axis(float x, float y, float z,float s)
{
 glPushMatrix();
  glTranslatef(x,y,z);
  glBegin(GL_LINES); 
   glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(s,0,0);
   glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,s,0);
   glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,s);
  glEnd(); 
 glPopMatrix();
 glColor3f(1,1,1);
}

#endif

