//------------------------------------------------------------------------------
//--------------------------Code By: 3DSage-------------------------------------
//----------------Video tutorial on YouTube-3DSage------------------------------
//------------------------------------------------------------------------------
#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define Assert assert
#define CHECK_GL

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <glut.h>
#include "opengl_check.h"
#include <Windows.h>

#define res        1                        //0=160x120 1=360x240 4=640x480
#define SW         160*res                  //screen width
#define SH         120*res                  //screen height
#define SW2        (SW/2)                   //half of screen width
#define SH2        (SH/2)                   //half of screen height
#define PIXELSCALE 4/res                    //OpenGL pixel scale
#define GLSW       (SW*PIXELSCALE)          //OpenGL window width
#define GLSH       (SH*PIXELSCALE)          //OpenGL window height
#define FRAMERATE  20

//------------------------------------------------------------------------------
struct Time
{
    int this_ms;
    int last_ms;
    int fr1,fr2;           //frame 1 frame 2, to create constant frame rate
};

struct Keys
{
    int w,s,a,d;           //move up, down, left, right
    int sl,sr;             //strafe left, right 
    int m;                 //move up, down, look up, down
};
//------------------------------------------------------------------------------

int tick;
Keys K;
Time T;

enum Color
{
    Color_Yellow,
    Color_Yellow_Darker,
    Color_Green,
    Color_Green_Darker,
    Color_Cyan,
    Color_Cyan_Darker,
    Color_Brown,
    Color_Brown_Darker,
    Color_Background,
    Color_Count,
};

struct RGB
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

const RGB COLORS[Color_Count] = {
   {255, 255,   0}, //Yellow	
   {160, 160,   0}, //Yellow darker	
   {  0, 255,   0}, //Green	
   {  0, 160,   0}, //Green darker	
   {  0, 255, 255}, //Cyan	
   {  0, 160, 160}, //Cyan darker
   {160, 100,   0}, //brown	
   {110,  50,   0}, //brown darker
   {  0,  60, 130}, //background 
};

void pixel(int x,int y, int c)                  //draw a pixel at x/y with rgb
{
    Assert(c >= 0 && c < Color_Count);
    RGB color = COLORS[c];

    glBegin(GL_POINTS);
    glColor3ub(color.r, color.g, color.b); 
    glVertex2i(x*PIXELSCALE+2,y*PIXELSCALE+2);
    glEnd();
}

void movePlayer()
{
    //move up, down, left, right
    if (K.a ==1 && K.m==0){ printf("left\n");}  
    if (K.d ==1 && K.m==0){ printf("right\n");}
    if (K.w ==1 && K.m==0){ printf("up\n");}
    if (K.s ==1 && K.m==0){ printf("down\n");}
    //strafe left, right
    if (K.sr==1){ printf("strafe left\n");}
    if (K.sl==1){ printf("strafe right\n");}
    //move up, down, look up, look down
    if (K.a==1 && K.m==1){ printf("look up\n");}
    if (K.d==1 && K.m==1){ printf("look down\n");}
    if (K.w==1 && K.m==1){ printf("move up\n");}
    if (K.s==1 && K.m==1){ printf("move down\n");}
}

void clearBackground() 
{
    for (int y = 0; y < SH; y++)
    { 
        for (int x = 0; x < SW; x++)
        {
            pixel(x,y,Color_Background);
        }
    }	
}

void draw3D()
{
    int c = 0;
    for (int y=0;y<SH2;y++)
    {
        for (int x=0;x<SW2;x++)
        {
            pixel(x,y,c); 
            c = (c + 1) % Color_Count;
        }
    }

    //frame rate
    tick = (tick + 1) % FRAMERATE;
    pixel(SW2,SH2+tick,Color_Yellow); 
}

void display() 
{
    if (T.this_ms - T.last_ms >= 1000 / FRAMERATE)                        //only draw 20 frames/second
    { 
        clearBackground();
        movePlayer();
        draw3D(); 

        T.last_ms = T.this_ms;
        glutSwapBuffers(); 
        glutReshapeWindow(GLSW,GLSH);             //prevent window scaling
    }

    T.this_ms = glutGet(GLUT_ELAPSED_TIME);          //1000 Milliseconds per second
    glutPostRedisplay();
} 

void KeysDown(unsigned char key,int x,int y)   
{ 
    if (key=='w'){ K.w =1;} 
    if (key=='s'){ K.s =1;} 
    if (key=='a'){ K.a =1;} 
    if (key=='d'){ K.d =1;} 
    if (key=='m'){ K.m =1;} 
    if (key=='e'){ K.sr=1;} 
    if (key=='q'){ K.sl=1;} 
}
void KeysUp(unsigned char key,int x,int y)
{ 
    if (key=='w'){ K.w =0;}
    if (key=='s'){ K.s =0;}
    if (key=='a'){ K.a =0;}
    if (key=='d'){ K.d =0;}
    if (key=='m'){ K.m =0;}
    if (key=='e'){ K.sr=0;} 
    if (key=='q'){ K.sl=0;}
}

int main(int argc, char* argv[])
{
    __glutInitWithExit(&argc, argv, [](int code){ if (code == EXIT_FAILURE) DebugBreak(); exit(code); });
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(GLSW/2,GLSH/2);
    glutInitWindowSize(GLSW,GLSH);
    glutCreateWindow(""); 
    glPointSize(PIXELSCALE);    // pixel size
    gluOrtho2D(0,GLSW,0,GLSH);  // origin bottom left

    // init
    glutDisplayFunc(display);
    glutKeyboardFunc(KeysDown);
    glutKeyboardUpFunc(KeysUp);
    glutMainLoop();
    return 0;
} 
