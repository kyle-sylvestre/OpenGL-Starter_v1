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

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <glut.h>
#include <Windows.h>
#include "Load_Levels.txt"
#undef min
#undef max

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define PrintErrorf(fmt, ...) do { fprintf(stderr, fmt, __VA_ARGS__); DebugBreak(); }while(0)
#define Assert(cond) if (!(cond)) PrintErrorf("assertion failed: %s\n", #cond)
#define Coerce(x, min, max) (x < min) ? min : (x > max) ? max : x;

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

#define res        1                        // 0=160x120 1=360x240 4=640x480
#define SW         (160*res)                // screen width
#define SH         (120*res)                // screen height
#define SW2        (SW/2)                   // half of screen width
#define SH2        (SH/2)                   // half of screen height
#define PIXELSCALE (4/res)                  // OpenGL pixel scale
#define GLSW       (SW*PIXELSCALE)          // OpenGL window width
#define GLSH       (SH*PIXELSCALE)          // OpenGL window height
#define FRAMERATE  20
#define PI32       3.14159265359f
#define MAX_WALLS        100
#define MAX_SECTORS      100
#define INTS_PER_SECTOR   6                   // how many ints per stored sector
#define INTS_PER_WALL     5                   // how many ints per stored wall

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

struct V3
{
    int x,y,z;
};

struct V2
{
    int x,y;
};

struct Player 
{
    int x, y, z;    // position
    int a;          // y rotation (left/right)
    int l;          // z rotation (up/down)
};

struct Wall
{
    V2 p1;          // bottom line point 1
    V2 p2;          // bottom line point 2
    int c;          // color
} walls[MAX_WALLS];

struct Sector
{
    int ws, we;     // wall start index to end: [ws, we) 
    int z1, z2;     // height of bottom and top
    V2 p;           // center position of sector
    int d;          // add y distances to sort draw order
} sectors[MAX_SECTORS];

struct Math
{
    float sin[360]; // deg -> rad
    float cos[360]; // deg -> rad
};

struct RGB
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

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

//------------------------------------------------------------------------------

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

Keys keys;
Time time;
Player p;
Math m;
int sector_count;
int wall_count;

void DrawPixel(int x, int y, int c)
{
    Assert(c >= 0 && c < Color_Count);

    // don't draw any offscreen pixels 
    //if ((x < 0 || x > SW) ||
    //    (y < 0 || y > SH))
    //    return;

    RGB color = COLORS[c];
    glColor3ub(color.r, color.g, color.b); 

    glBegin(GL_POINTS);
    glVertex2i(x * PIXELSCALE + 2, y * PIXELSCALE + 2);
    glEnd();
}

void Move()
{
    //move up, down, left, right
    if (keys.a ==1 && keys.m==0){ p.a -= 4; if (p.a < 0)   p.a += 360; } // left
    if (keys.d ==1 && keys.m==0){ p.a += 4; if (p.a > 359) p.a -= 360; } // right

    int dx = m.sin[p.a] * 10.0;
    int dy = m.cos[p.a] * 10.0;
    if (keys.w ==1 && keys.m==0){ p.x += dx; p.y += dy; } // forward
    if (keys.s ==1 && keys.m==0){ p.x -= dx; p.y -= dy; } // backward
    if (keys.sr==1){ p.x += dy; p.y -= dx; }    // strafe right
    if (keys.sl==1){ p.x -= dy; p.y += dx; }    // strafe left

    //move up, down, look up, look down
    if (keys.a==1 && keys.m==1){ p.l -= 1; } // look up
    if (keys.d==1 && keys.m==1){ p.l += 1; } // look down
    if (keys.w==1 && keys.m==1){ p.z -= 4; } // move up
    if (keys.s==1 && keys.m==1){ p.z += 4; } // move down
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

void ClipBehindPlayer(V3 *p1, const V3 *p2)
{
    float da = p1->y;
    float db = p2->y;
    float d = da - db; if (d == 0) d = 1;
    float s = da / d;
    p1->x += s * (p2->x - p1->x);
    p1->y += s * (p2->y - p1->y); if (p1->y == 0) p1->y = 1;
    p1->z += s * (p2->z - p1->z);
}

int Distance(V2 p1, V2 p2)
{
    float x = p2.x - p1.x;
    float y = p2.y - p1.y;
    return sqrt(x*x + y*y);
}

void DrawWall(int x1, int x2, int b1, int b2, int t1, int t2, int c)
{
    // hold difference in distance
    int dyb = b2 - b1;
    int dyt = t2 - t1;
    int dx = x2 - x1;
    if (dx == 0) dx = 1;
    int xs = x1;

    // clip x
    x1 = Coerce(x1, 1, SW - 1);
    x2 = Coerce(x2, 1, SW - 1);

    // draw x verticle lines
    for (int x = x1; x < x2; x++)
    {
        // the y start and end point
        int y1 = dyb * (x - xs + 0.5) / dx + b1; // y bottom point
        int y2 = dyt * (x - xs + 0.5) / dx + t1; // y top point

        // clip y
        y1 = Coerce(y1, 1, SH - 1);
        y2 = Coerce(y2, 1, SH - 1);

        for (int y = y1; y < y2; y++)
        {
            DrawPixel(x, y, c);
        }
    }

}

void Draw3D()
{
    float CS = m.cos[p.a];
    float SN = m.sin[p.a];

    // order sectors by distance
    for (int si = 0; si < sector_count - 1; si++)
    {
        for (int i = 0; i < sector_count - si - 1; i++)
        {
            if (sectors[i].d < sectors[i + 1].d)
            {
                Sector tmp = sectors[i];
                sectors[i] = sectors[i + 1];
                sectors[i + 1] = tmp;
            }
        } 
    }

    for (int si = 0; si < sector_count; si++)
    {
        Sector *sector = &sectors[si];
        for (int wi = sector->ws; wi < sector->we; wi++)
        {
            Assert(wi < wall_count);
            Wall *wall = &walls[wi];
            V3 w0, w1, w2, w3;
            w0 = w1 = w2 = w3 = {};

            // offset bottom 2 points by player
            //int x1 = 40 - p.x; 
            //int y1 = 10 - p.y;
            //int x2 = 40 - p.x; 
            //int y2 =290 - p.y;
            int x1 = wall->p1.x - p.x; 
            int y1 = wall->p1.y - p.y;
            int x2 = wall->p2.x - p.x; 
            int y2 = wall->p2.y - p.y;
            int swp = x1; x1 = x2; x2 = swp; swp = y1; y1 = y2; y2 = swp;

            // world x position
            w0.x = x1 * CS - y1 * SN;
            w1.x = x2 * CS - y2 * SN;
            w2.x = w0.x;
            w3.x = w1.x;

            // world y position
            w0.y = y1 * CS + x1 * SN;
            w1.y = y2 * CS + x2 * SN;
            w2.y = w0.y;
            w3.y = w1.y;
            sector->d += Distance( {0,0}, {(w0.x + w1.x) / 2, (w0.y + w1.y) / 2});

            // world z height
            w0.z = sector->z1 - p.z + ((p.l * w0.y) / 32.0f);
            w1.z = sector->z1 - p.z + ((p.l * w1.y) / 32.0f);
            w2.z = w0.z + sector->z2;
            w3.z = w1.z + sector->z2;

            if (w0.y < 1 && w1.y < 1) 
            {
                continue; // wall behind player, skip
            }
            else if (w0.y < 1)
            {
                ClipBehindPlayer(&w0, &w1); // bottom line
                ClipBehindPlayer(&w2, &w3); // top line
            }
            else if (w1.y < 1)
            {
                ClipBehindPlayer(&w1, &w0); // bottom line
                ClipBehindPlayer(&w3, &w2); // top line
            }

            // screen x and screen y
            w0.x=w0.x*200/w0.y+SW2; w0.y=w0.z*200/w0.y+SH2;
            w1.x=w1.x*200/w1.y+SW2; w1.y=w1.z*200/w1.y+SH2;
            w2.x=w2.x*200/w2.y+SW2; w2.y=w2.z*200/w2.y+SH2;
            w3.x=w3.x*200/w3.y+SW2; w3.y=w3.z*200/w3.y+SH2;

            DrawWall(w0.x, w1.x, w0.y, w1.y, w2.y, w3.y, wall->c);
        }

        sector->d /= (sector->we - sector->ws);
    }
}

void Display() 
{
    if (time.this_ms - time.last_ms >= 1000 / FRAMERATE) //only draw 20 frames/second
    { 
        ClearBackground();
        Move();
        Draw3D(); 

        time.last_ms = time.this_ms;
        glutSwapBuffers(); 
        glutReshapeWindow(GLSW,GLSH); // prevent window scaling
    }

    time.this_ms = glutGet(GLUT_ELAPSED_TIME); // 1000 Milliseconds per second
    glutPostRedisplay();
} 

void KeysDown(unsigned char key,int x,int y)   
{ 
    if (key=='w'){ keys.w =1;} 
    if (key=='s'){ keys.s =1;} 
    if (key=='a'){ keys.a =1;} 
    if (key=='d'){ keys.d =1;} 
    if (key=='m'){ keys.m =1;} 
    if (key=='e'){ keys.sr=1;} 
    if (key=='q'){ keys.sl=1;} 
}
void KeysUp(unsigned char key,int x,int y)
{ 
    if (key=='w'){ keys.w =0;}
    if (key=='s'){ keys.s =0;}
    if (key=='a'){ keys.a =0;}
    if (key=='d'){ keys.d =0;}
    if (key=='m'){ keys.m =0;}
    if (key=='e'){ keys.sr=0;} 
    if (key=='q'){ keys.sl=0;}
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
    const auto exit_callback = [](int code)
    {
        if (code == EXIT_FAILURE) 
            DebugBreak(); 

        exit(code);
    };

    __glutInitWithExit(&argc, argv, exit_callback);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(GLSW/2,GLSH/2);
    glutInitWindowSize(GLSW,GLSH);
    glutCreateWindow("Yee Boi"); 
    glPointSize(PIXELSCALE);    // pixel size
    gluOrtho2D(0,GLSW,0,GLSH);  // origin bottom left

    {
        // initialize sin/cos lookup
        for (int i = 0; i < 360; i++)
        {
            m.sin[i] = sin(i * (PI32 / 180.0));
            m.cos[i] = cos(i * (PI32 / 180.0));
        }

        // initialize player
        p.x = 70;
        p.y = -110;
        p.z = 20;
        p.a = 0;
        p.l = 0;

        int *sec            = loadSectors_1;
        int sec_int_count   = ArrayCount(loadSectors_1);
        int *wall           = loadWalls_1;
        int wall_int_count  = ArrayCount(loadWalls_1);

        // load in sectors
        Assert(sec_int_count % INTS_PER_SECTOR == 0);
        for (int i = 0; i < sec_int_count; i += INTS_PER_SECTOR)
        {
            if (sector_count >= MAX_SECTORS)
            {
                PrintErrorf("exceeded sector capacity\n");
                exit(EXIT_FAILURE);
            }

            Sector *s = &sectors[sector_count++];
            s->ws = sec[i + 0];
            s->we = sec[i + 1];
            s->z1 = sec[i + 2];
            s->z2 = sec[i + 3];
            s->p.x = sec[i + 4];
            s->p.y = sec[i + 5];

            bool good_sector = (s->ws < s->we);
            if (!good_sector)
            {
                PrintErrorf("bad sector at index: %d\n", i / INTS_PER_SECTOR);
                exit(EXIT_FAILURE);
            }
        }

        // load in walls
        Assert(wall_int_count % INTS_PER_WALL == 0);
        for (int i = 0; i < wall_int_count; i += INTS_PER_WALL)
        {
            if (wall_count >= MAX_WALLS)
            {
                PrintErrorf("exceeded wall capacity\n");
                exit(EXIT_FAILURE);
            }

            Wall *w = &walls[wall_count++];
            w->p1.x = wall[i + 0];
            w->p1.y = wall[i + 1];
            w->p2.x = wall[i + 2];
            w->p2.y = wall[i + 3];
            w->c = wall[i + 4];

            bool good_wall = (w->c >= 0 && w->c < Color_Count);
            if (!good_wall)
            {
                PrintErrorf("bad wall at index: %d\n", i / INTS_PER_WALL);
                exit(EXIT_FAILURE);
            }
        }

        // placeholder for missing wall/sectors at part 1 18:35
        sector_count = 0;
        wall_count = 0;
        const auto PushSector = [&](V2 st, int c)
        {
            V2 pts[4] = {
                {st.x,       st.y},
                {st.x + 32,  st.y},
                {st.x + 32,  st.y + 32},
                {st.x,       st.y + 32},
            };

            Wall *w = &walls[wall_count];
            w[0].p1 = pts[0]; w[0].p2 = pts[1]; w[0].c = c;
            w[1].p1 = pts[1]; w[1].p2 = pts[2]; w[1].c = c + 1;
            w[2].p1 = pts[2]; w[2].p2 = pts[3]; w[2].c = c;
            w[3].p1 = pts[3]; w[3].p2 = pts[0]; w[3].c = c + 1;

            Sector *s = &sectors[sector_count];
            s->ws = wall_count;
            s->we = s->ws + 4;
            s->z1 = 0;
            s->z2 = 40;
            s->p = {};  //unused
            s->d = 0;

            sector_count += 1;
            wall_count += 4;
        };

        PushSector({0, 0}, 0);
        PushSector({64,0}, 2);
        PushSector({64,64}, 4);
        PushSector({0, 64}, 6);

    }

    // enable debug function for opengl 4.3 and higher
    const char *version = (const char *)glGetString(GL_VERSION);
    if (version)
    {
        int major = 0;
        int minor = 0;
        if (2 == sscanf(version, "%d.%d", &major, &minor) &&
            major >= 4 && minor >= 3)
        {
            libGL = LoadLibraryW(L"opengl32.dll");
            if (libGL != NULL) 
            {
                atexit([](){ FreeLibrary(libGL); libGL = NULL; });
                wglGetProc = (GETPROCFN *)GetProcAddress(libGL, "wglGetProcAddress");
                if (!wglGetProc)
                {
                    PrintErrorf("error getting wglGetProcAddress\n");
                }
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
        }
    }

    // set the callback functions
    glutDisplayFunc(Display);
    glutKeyboardFunc(KeysDown);
    glutKeyboardUpFunc(KeysUp);
    //glutMotionFunc();             // mouse motion while 1 or more buttons down
    //glutPassiveMotionFunc();      // mouse motion while no buttons down
    glutMainLoop();
    return EXIT_SUCCESS;
} 
