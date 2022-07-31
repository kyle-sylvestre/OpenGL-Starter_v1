//------------------------------------------------------------------------------
//--------------------------Code By: 3DSage-------------------------------------
//----------------Video tutorial on YouTube-3DSage------------------------------
//------------------------------------------------------------------------------
#pragma warning(disable:4996)   // This function or variable may be unsafe
#pragma warning(disable:4505)   // unreferenced function removed
#pragma warning(disable:4702)   // unreachable code
#pragma warning(disable:4127)   // conditional expression is constant
#pragma warning(disable:4189)   // local variable is initialized but not referenced
#pragma warning(disable:4706)   // assignment within conditional expression
#pragma warning(disable:4100)   // unreferenced formal parameter

#pragma warning(error:4717)     // recursive on all control paths
#pragma warning(error:4293)     // '<<': shift count negative or too big
#pragma warning(error:4715)     // not all code paths return a value
#pragma warning(error:4701)     // potentially uninitialized local variable
#pragma warning(error:4309)     // truncation of constant value
#pragma warning(error:4002)     // too many arguments for function-like macro
#pragma warning(error:4456)     // declaration of 'foo' hides previous local declaration
#pragma warning(error:4457)     // declaration of 'foo' hides function parameter
#pragma warning(error:4458)     // declaration of 'foo' hides class member
#pragma warning(error:4459)     // declaration of 'foo' hides global declaration

// printf family warnings
#pragma warning(error:4473)     // not enough arguments for format string
#pragma warning(error:4474)     // too many arguments for format string
#pragma warning(error:4475)     // format string length modifier invalid for type field character
#pragma warning(error:4476)     // unknown type field character
#pragma warning(error:4477)     // format string type mismatch
#pragma warning(error:4478)     // positional and non-positional placeholders cannot be mixed in the same format string
#pragma warning(error:4774)     // format string not a string literal
#pragma warning(error:4775)     // nonstandard extension used in format string
#pragma warning(error:4776)     // %<conversion-specifier> is not allowed in the format string
#pragma warning(error:4777)     // format string type mismatch(how is this different from C4477???)
#pragma warning(error:4778)     // unterminated format string

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define PrintErrorf(fmt, ...) do { fprintf(stderr, fmt, __VA_ARGS__); DebugBreak(); }while(0)
#define Assert(cond) if (!(cond)) PrintErrorf("assertion failed: %s\n", #cond)

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <glut.h>
#include <Windows.h>

// opengl version >= 4.3 definitions
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_OUTPUT 0x92E0

#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B

#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A

#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148

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

// draw a pixel at x/y with rgb
void DrawPixel(int x, int y, int c)
{
    Assert(c >= 0 && c < Color_Count);
    RGB color = COLORS[c];
    glColor3ub(color.r, color.g, color.b); 

    glBegin(GL_POINTS);
    glVertex2i(x * PIXELSCALE + 2, y * PIXELSCALE + 2);
    glEnd();
}

void MovePlayer()
{
    //move up, down, left, right
    if (K.a ==1 && K.m==0){ printf("left\n");}  
    if (K.d ==1 && K.m==0){ printf("right\n");}
    if (K.w ==1 && K.m==0){ printf("up\n");}
    if (K.s ==1 && K.m==0){ printf("down\n");}
    //strafe left, right
    if (K.sr==1){ printf("strafe right\n");}
    if (K.sl==1){ printf("strafe left\n");}
    //move up, down, look up, look down
    if (K.a==1 && K.m==1){ printf("look up\n");}
    if (K.d==1 && K.m==1){ printf("look down\n");}
    if (K.w==1 && K.m==1){ printf("move up\n");}
    if (K.s==1 && K.m==1){ printf("move down\n");}
}

void ClearBackground() 
{
    for (int y = 0; y < SH; y++)
    { 
        for (int x = 0; x < SW; x++)
        {
            DrawPixel(x, y, Color_Background);
        }
    }	
}

void Draw()
{
    int c = 0;
    for (int y = 0; y < SH2; y++)
    {
        for (int x = 0; x < SW2; x++)
        {
            DrawPixel(x, y, c); 
            c = (c + 1) % Color_Count;
        }
    }

    //frame rate
    tick = (tick + 1) % FRAMERATE;
    DrawPixel(SW2, SH2+tick, Color_Yellow); 
}

void Display() 
{
    if (T.this_ms - T.last_ms >= 1000 / FRAMERATE) //only draw 20 frames/second
    { 
        ClearBackground();
        MovePlayer();
        Draw(); 

        T.last_ms = T.this_ms;
        glutSwapBuffers(); 
        glutReshapeWindow(GLSW,GLSH); // prevent window scaling
    }

    T.this_ms = glutGet(GLUT_ELAPSED_TIME); // 1000 Milliseconds per second
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

void APIENTRY DebugOpenGL(GLenum gl_source, 
                          GLenum gl_type, 
                          GLuint gl_id, 
                          GLenum gl_severity, 
                          GLsizei gl_length,
                          const char *gl_message,
                          const void *user_param) 
{
    const char *source;
    switch (gl_source) {
    case GL_DEBUG_SOURCE_API:
    {
        // TODO: disabled until better stack traces are used
        //source = "OpenGL API"; break;
        source = "OpenGL API";
        break; //return;
    }
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source = "window system";   break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: source = "shader compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     source = "third party";     break;
    case GL_DEBUG_SOURCE_APPLICATION:     source = "application";     break;
    case GL_DEBUG_SOURCE_OTHER:           source = "other";           break;
    default: source = "???"; break;
    }

    const char *type;
    switch (gl_type) {
    case GL_DEBUG_TYPE_ERROR:       type = "error";                      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type = "deprecated";         break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type = "undefined behavior"; break;
    case GL_DEBUG_TYPE_PORTABILITY: type = "portability";                break;
    case GL_DEBUG_TYPE_PERFORMANCE: type = "performance";                break;
    case GL_DEBUG_TYPE_MARKER:      type = "marker";                     break;
    case GL_DEBUG_TYPE_PUSH_GROUP:  type = "push group";                 break;
    case GL_DEBUG_TYPE_POP_GROUP:   type = "pop group";                  break;
    case GL_DEBUG_TYPE_OTHER:       type = "other";                      break;
    default:
        type = "???"; break;
    }

    const char *severity;
    switch (gl_severity) {
    case GL_DEBUG_SEVERITY_HIGH:   severity = "high";   break;
    case GL_DEBUG_SEVERITY_MEDIUM: severity = "medium"; break;
    case GL_DEBUG_SEVERITY_LOW:    severity = "low";    break;
    default:                       
        return;
    }

    if (gl_severity == GL_DEBUG_SEVERITY_HIGH)
    {
        PrintErrorf("glDebugCallback Called\n"
                    "  severity: %s\n"
                    "  source: %s\n" 
                    "  type: %s\n"
                    "  message: %s\n", 
                    severity, source, type, gl_message);
        DebugBreak();
    }
    else // med or low
    {
        printf("glDebugCallback Called\n"
               "  severity: %s\n"
               "  source: %s\n" 
               "  type: %s\n"
               "  message: %s\n", 
               severity, source, type, gl_message);
    }
}

typedef void *GETPROCFN(const char *);
typedef void APIENTRY GLDEBUGPROC(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei Length,const char *message,const void *userParam);
typedef void APIENTRY GLCALLBACKFN(GLDEBUGPROC *callback, const void *userParam);
typedef void APIENTRY GLCONTROLFN(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
HMODULE libGL;
GETPROCFN *wglGetProc;

void *GetProc(const char *name) 
{
    void *result = NULL;
    if (libGL != NULL)
    {
        if (wglGetProc != NULL)
        {
            result = (void *)wglGetProc(name); 
        }

        if (result == NULL)
        {
            result = (void*)GetProcAddress(libGL, name);
        }
    }

    return result;
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

#if 1
    // opengl 4.3 debug functions
    //printf("opengl version: %s\n", glGetString(GL_VERSION));
    libGL = LoadLibraryW(L"opengl32.dll");
    if (libGL != NULL) 
    {
        atexit([](){ FreeLibrary(libGL); libGL = NULL; });
        wglGetProc = (GETPROCFN *)GetProcAddress(libGL, "wglGetProcAddress");
        if (!wglGetProc)
            PrintErrorf("error getting wglGetProcAddress\n");
    }
    else
    {
        PrintErrorf("error in LoadLibraryW opengl32.dll");
    }

    GLCALLBACKFN *glDebugMessageCallback = (GLCALLBACKFN *)GetProc("glDebugMessageCallback");
    GLCONTROLFN *glDebugMessageControl = (GLCONTROLFN *)GetProc("glDebugMessageControl");

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugOpenGL, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif

    // init
    glutDisplayFunc(Display);
    glutKeyboardFunc(KeysDown);
    glutKeyboardUpFunc(KeysUp);
    glutMainLoop();
    return EXIT_SUCCESS;
} 
