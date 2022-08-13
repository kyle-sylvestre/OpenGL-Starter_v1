//------------------------------------------------------------------------------
//--------------------------Code By: 3DSage-------------------------------------
//----------------Video tutorial on YouTube-3DSage------------------------------
//------------------------------------------------------------------------------

#include <glut.h>
#include <Windows.h>
#include <kbt\kbt_common.h>
#include <kbt\kbt_bitmap_font.h>
#include "Load_Levels.txt"

// remove windows macros
#undef min
#undef max
#undef DrawText

void DrawText(const FONT *font, const char *text, int bx, int by, int color);
void DrawText(BitmapFont *font, const char *text, int bx, int by, int color);
PrintFn *const PrintImpl = [](const char *msg)
{ 
    fprintf(stdout, "%s", msg);
    fflush(stdout);
};

#define res        1                        // 0=160x120 1=360x240 4=640x480
#define SW         (160*res)                // screen width
#define SH         (120*res)                // screen height
#define SW2        (SW/2)                   // half of screen width
#define SH2        (SH/2)                   // half of screen height
#define PIXELSCALE (4/res)                  // OpenGL pixel scale
#define GLSW       (SW*PIXELSCALE)          // OpenGL window width
#define GLSH       (SH*PIXELSCALE)          // OpenGL window height
#define FRAMERATE  20
#define FRAME_MS   (1000 / FRAMERATE)
#define PI32       3.14159265359f
#define INTS_PER_SECTOR   6                 // how many ints per stored sector
#define INTS_PER_WALL     5                 // how many ints per stored wall
#define Coerce(x, min, max) ((x < min) ? min : (x > max) ? max : x)

//------------------------------------------------------------------------------
struct Time
{
    int this_ms;
    int last_ms;
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
    uint c;         // color
};

enum Surface
{
    Surface_None,
    Surface_Below,
    Surface_Above,
};

struct Sector
{
    uint ws, we;        // wall start index to end: [ws, we) 
    int z1, z2;         // height of bottom and top
    int d;              // add y distances to sort draw order
    int cb, ct;         // bottom and top surface colors
    int surf[SW];       // points for the surface
    Surface surface;    // what surface to draw
};

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
    Color_Black,
    Color_Count,
};

const RGB COLORS[Color_Count] = {
   {255, 255,   0}, // yellow	
   {160, 160,   0}, // yellow darker	
   {  0, 255,   0}, // green	
   {  0, 160,   0}, // green darker	
   {  0, 255, 255}, // cyan	
   {  0, 160, 160}, // cyan darker
   {160, 100,   0}, // brown	
   {110,  50,   0}, // brown darker
   {  0,  60, 130}, // background 
   {  0,   0,   0}, // black
};

Time time;
Keys keys;
Player p;
Math m;
Vector<Wall> walls;
Vector<Sector> sectors;

void DrawPixel(int x, int y, uint c)
{
    Assert(c < Color_Count);

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
    if (keys.a ==1 && keys.m==0){ p.a -= 5; if (p.a < 0)   p.a += 360; } // left
    if (keys.d ==1 && keys.m==0){ p.a += 5; if (p.a > 359) p.a -= 360; } // right

    int dx = m.sin[p.a] * 10.0;
    int dy = m.cos[p.a] * 10.0;
    if (keys.w ==1 && keys.m==0){ p.x += dx; p.y += dy; } // forward
    if (keys.s ==1 && keys.m==0){ p.x -= dx; p.y -= dy; } // backward
    if (keys.sr==1){ p.x += dy; p.y -= dx; }    // strafe right
    if (keys.sl==1){ p.x -= dy; p.y += dx; }    // strafe left

    //move up, down, look up, look down
    if (keys.a==1 && keys.m==1){ p.l -= 1; } // look up
    if (keys.d==1 && keys.m==1){ p.l += 1; } // look down
    if (keys.w==1 && keys.m==1){ p.z -= 5; } // move up
    if (keys.s==1 && keys.m==1){ p.z += 5; } // move down
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

void DrawWall(int x1, int x2, int b1, int b2, int t1, int t2, 
              int c, Sector *s, bool acc)
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

        if (acc)
        {
            // accumulate surface top/bottom values
            // set the surf start based upon the furthest wall
            if (s->surface == Surface_Below)
                s->surf[x] = y1;
            if (s->surface == Surface_Above)
                s->surf[x] = y2;
        }
        else
        {
            // draw bottom surface
            if (s->surface == Surface_Below)
                for (int y = s->surf[x]; y < y1; y++)
                    DrawPixel(x, y, Color_Green);

            // draw top surface
            if (s->surface == Surface_Above)
                for (int y = y2; y < s->surf[x]; y++)
                    DrawPixel(x, y, Color_Green);
            
            // draw walls
            for (int y = y1; y < y2; y++)
                DrawPixel(x, y, c);
        }
    }
}

void Draw3D()
{
    float CS = m.cos[p.a];
    float SN = m.sin[p.a];

    // sort sectors descending from current distance to player
    const auto SortDistanceDescending = [](const void *a, const void *b) -> int
    {
        return ((Sector *)b)->d - ((Sector *)a)->d;
    };

    qsort(sectors.data(), sectors.size(), sizeof(Sector), SortDistanceDescending);
    for (uint si = 0; si < sectors.size(); si++)
    {
        Sector *sector = &sectors[si];
        if (p.z < sector->z1)       sector->surface = Surface_Below;
        else if (p.z > sector->z1)  sector->surface = Surface_Above;
        else                        sector->surface = Surface_None;

        for (uint side = 0; side < 2; side++)
        {
            for (uint wi = sector->ws; wi < sector->we; wi++)
            {
                Assert(wi < walls.size());
                Wall *wall = &walls[wi];
                V3 w0, w1, w2, w3;
                w0 = w1 = w2 = w3 = {};

                // offset bottom 2 points by player
                int x1 = wall->p1.x - p.x; 
                int y1 = wall->p1.y - p.y;
                int x2 = wall->p2.x - p.x; 
                int y2 = wall->p2.y - p.y;

                // reorder points to draw the culled back face
                if (side == 0)
                {
                    int swp = 0;
                    swp = x1; x1 = x2; x2 = swp; 
                    swp = y1; y1 = y2; y2 = swp;
                }

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

                V3 wp = {};
                wp.x = (w0.x + w1.x) / 2;
                wp.y = (w0.y + w1.y) / 2;
                wp.z = (w0.z + w2.z) / 2;

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

                DrawWall(w0.x, w1.x, w0.y, w1.y, w2.y, w3.y, 
                         wall->c, sector, side == 0);
            }
        }

        sector->d /= (sector->we - sector->ws);
    }

    char buf[1024] = {};
    tsnprintf(buf, "x: %d, y: %d, a: %d", p.x, p.y, p.a);


    static BitmapFont bmpfont;
    static bool ok = ParseBDF("ignore/tom-thumb.bdf", &bmpfont);
    if (ok)
        DrawText(&bmpfont, buf, 0, 0, Color_Green);
}

void Display() 
{
    //glClear(GL_COLOR_BUFFER_BIT);
    time.this_ms = glutGet(GLUT_ELAPSED_TIME);
    if (time.this_ms - time.last_ms >= FRAME_MS)
    {
        ClearBackground();
        Move();
        Draw3D(); 

        glutSwapBuffers(); 
        glutReshapeWindow(GLSW,GLSH); // prevent window scaling
        time.last_ms = time.this_ms;
    }

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

// Doom testing -------------------------------------------------------
struct Vertex
{
    int16_t x;
    int16_t y;
};

struct Segment
{
    int16_t vertex_start;
    int16_t vertex_end;
    int16_t angle;
    int16_t linedef_num;
    int16_t direction;
    int16_t offset;
};

struct LineDef
{
    int16_t vertex_start;
    int16_t vertex_end;
    int16_t flags;
    int16_t special_type;
    int16_t sector_tag;
    int16_t front_sidedef;
    int16_t back_sidedef;
};

#pragma pack(2)
struct RSector // TODO: merge with tutorial Sector struct
{
    int16_t floor_height;
    int16_t ceiling_height;
    char    floor_texture_name[8];
    char    ceiling_texture_name[8];
    int16_t light_level;
    int16_t special_type;
    int16_t tag_number;

    // calculated at load time
    //int line_count;
    //LineDef *lines;
    //Vertex blockbox_BL;
    //Vertex blockbox_TR;
    //Vertex soundorg;
};
#pragma pack()


struct Map
{
    Vector<Vertex> vertices;
    Vector<LineDef> linedefs;
    Vector<RSector> rsectors;
};
//------------------------------------------------------------------------------

bool ReadMap(const char *wad_filename, Map *map)
{
    struct WadHeader
    {
        char magic[4];
        uint32_t lump_count;
        uint32_t directory_fpos;
    };

    struct LumpHeader
    {
        uint32_t data_fpos;
        uint32_t byte_count;
        char name[8];
    };

    bool result = false;
    WadHeader wad_header = {};
    MemorySpanIterator wad_file = {};
    *map = {};

    defer{ if (!result) *map = {}; };

    if (!ReadAllocIterator(wad_filename, &wad_file))
        return result;

    defer{ DestroyIterator(&wad_file); };

    ReadV(&wad_file, &wad_header);
    if (0 != memcmp("IWAD", wad_header.magic, sizeof(wad_header.magic)) &&
        0 != memcmp("PWAD", wad_header.magic, sizeof(wad_header.magic)))
    {
        Error("bad WAD magic number, got %.*s expected IWAD or PWAD",
              sizeof(wad_header.magic), wad_header.magic);
        return result;
    }

    // read in the lump headers within the directory section
    MemorySpanIterator lump_dir = GetInnerSpan(&wad_file, wad_header.directory_fpos,
                                               wad_header.lump_count * sizeof(LumpHeader));
    if (!IsValidSpan(&lump_dir.span))
        return result;

    for (uint ihdr = 0;
         ihdr < wad_header.lump_count;
         ihdr++)
    {
        LumpHeader lump = {};
        ReadV(&lump_dir, &lump);

        // extend length for string comparisons
        char lumpname[sizeof(lump.name) + 1] = {};
        memcpy(lumpname, lump.name, sizeof(lump.name));

        printf("lump %d: offset %d, bytes: %d, name: %.*s\n",
               ihdr, lump.data_fpos, lump.byte_count,
               sizeof(lump.name), lump.name);

        // get the lump data span
        MemorySpanIterator lump_data = GetInnerSpan(&wad_file, lump.data_fpos, lump.byte_count);
        if (lump.byte_count != 0 && !IsValidSpan(&lump_data.span))
            return result;

        const auto IsMultiple = [&](uint itemsize) -> bool
        {
            // lump data is an array of an items
            // make sure lump.bytecount is a multiple of bytes_per_item
            bool is_multiple = (lump.byte_count % itemsize == 0);
            if (!is_multiple)
                Error("%s byte count is not a multiple of its itemsize", lumpname);

            return is_multiple;
        };

        if (0 == strcmp(lumpname, "LINEDEFS"))
        {
            if (!IsMultiple(sizeof(LineDef)))
                return result;

            uint linedef_count = lump.byte_count / sizeof(LineDef);
            map->linedefs.resize(linedef_count);
            ReadN(&lump_data, map->linedefs.data(), lump.byte_count);

            for (uint iline = 0; iline < linedef_count; iline++)
            {
                LineDef *def = &map->linedefs[iline];
                printf("[%d, %d) sector tag %d\n",
                       def->vertex_start, def->vertex_end,
                       def->sector_tag);
            }
        }
        else if (0 == strcmp(lumpname, "SIDEDEFS"))
        {
            // texture info
        }
        else if (0 == strcmp(lumpname, "VERTEXES"))
        {
            if (!IsMultiple(sizeof(Vertex)))
                return result;

            uint count = lump.byte_count / sizeof(Vertex);
            map->vertices.resize(count);
            ReadN(&lump_data, map->vertices.data(), lump.byte_count);
        }
        else if (0 == strcmp(lumpname, "SEGS"))
        {
            if (!IsMultiple(sizeof(Segment)))
                return result;
        }
        else if (0 == strcmp(lumpname, "SSECTORS"))
        {

        }
        else if (0 == strcmp(lumpname, "SECTORS"))
        {
            const uint RSECTOR_FILE_SIZE = 26;
            if (!IsMultiple(RSECTOR_FILE_SIZE))
                return result;
            
            uint rsector_count = (lump.byte_count / RSECTOR_FILE_SIZE);
            map->rsectors.resize(rsector_count);
            for (uint isec = 0; isec < rsector_count; isec++)
            {
                ReadN(&lump_data, map->rsectors.data() + isec, RSECTOR_FILE_SIZE);
                RSector *rs = &map->rsectors[isec];
                printf("  floor height: %d, ceiling height: %d, floor name: %.*s, ceiling name %.*s tag %d\n",
                       rs->floor_height, rs->ceiling_height,
                       sizeof(rs->floor_texture_name), rs->floor_texture_name,
                       sizeof(rs->ceiling_texture_name), rs->ceiling_texture_name,
                       rs->tag_number);
            }
        }
    }

    result = true;
    return result;
}

// Doom testing -------------------------------------------------------

void APIENTRY DebugOpenGL(GLenum gl_source, 
                          GLenum gl_type, 
                          GLuint gl_id, 
                          GLenum gl_severity, 
                          GLsizei gl_length,
                          const char *gl_message,
                          const void *user_param) 
{
    const int GL_DEBUG_SOURCE_API = 0x8246;
    const int GL_DEBUG_SOURCE_WINDOW_SYSTEM = 0x8247;
    const int GL_DEBUG_SOURCE_SHADER_COMPILER = 0x8248;
    const int GL_DEBUG_SOURCE_THIRD_PARTY = 0x8249;
    const int GL_DEBUG_SOURCE_APPLICATION = 0x824A;
    const int GL_DEBUG_SOURCE_OTHER = 0x824B;
              
    const int GL_DEBUG_TYPE_ERROR = 0x824C;
    const int GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR = 0x824D;
    const int GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR = 0x824E;
    const int GL_DEBUG_TYPE_PORTABILITY = 0x824F;
    const int GL_DEBUG_TYPE_PERFORMANCE = 0x8250;
    const int GL_DEBUG_TYPE_OTHER = 0x8251;
    const int GL_DEBUG_TYPE_MARKER = 0x8268;
    const int GL_DEBUG_TYPE_PUSH_GROUP = 0x8269;
    const int GL_DEBUG_TYPE_POP_GROUP = 0x826A;
              
    const int GL_DEBUG_SEVERITY_HIGH = 0x9146;
    const int GL_DEBUG_SEVERITY_MEDIUM = 0x9147;
    const int GL_DEBUG_SEVERITY_LOW = 0x9148;


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
        Error("glDebugCallback Called\n"
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

int main(int argc, char* argv[])
{
    //extern void BitmapFontTest();
    //BitmapFontTest();

    //return 0;
    const auto ExitCallback = [](int code)
    {
        if (code == EXIT_FAILURE) 
            DebugBreak(); 

        exit(code);
    };

    __glutInitWithExit(&argc, argv, ExitCallback);
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

        int *sec            = loadSectors;
        uint sec_int_count  = ArrayCount(loadSectors);
        int *wall           = loadWalls;
        uint wall_int_count = ArrayCount(loadWalls);

        // load in sectors
        Assert(sec_int_count % INTS_PER_SECTOR == 0);
        for (uint i = 0; i < sec_int_count; i += INTS_PER_SECTOR)
        {
            Sector tmp = {};
            tmp.ws = sec[i + 0];
            tmp.we = sec[i + 1];
            tmp.z1 = sec[i + 2];
            tmp.z2 = sec[i + 3];
            tmp.cb = sec[i + 4];
            tmp.ct = sec[i + 5];

            Assert(tmp.ws < tmp.we);
            Assert(tmp.cb < Color_Count);
            Assert(tmp.ct < Color_Count);
            sectors.push_back(tmp);
        }

        // load in walls
        Assert(wall_int_count % INTS_PER_WALL == 0);
        for (uint i = 0; i < wall_int_count; i += INTS_PER_WALL)
        {
            Wall tmp = {};
            tmp.p1.x = wall[i + 0];
            tmp.p1.y = wall[i + 1];
            tmp.p2.x = wall[i + 2];
            tmp.p2.y = wall[i + 3];
            tmp.c = wall[i + 4];

            Assert(tmp.c >= 0 && tmp.c < Color_Count);
            walls.push_back(tmp);
        }

    }

    // maze testing
    //if (0)
    {
        walls.clear();
        sectors.clear();
        const int DIM = 64;     // floor square width/height
        const auto PushBoxSector = [&](V2 st, int c, int h)
        {
            Assert(c <= Color_Count);

            // add a sector with 4 walls
            V2 pts[4] = 
            {
                {st.x,        st.y      },
                {st.x + DIM,  st.y      },
                {st.x + DIM,  st.y + DIM},
                {st.x,        st.y + DIM},
            };

            size_t wall_start = walls.size();
            walls.resize(wall_start + 4);
            Wall *w = &walls[wall_start];
            w[0].p1 = pts[0]; w[0].p2 = pts[1]; w[0].c = c;
            w[1].p1 = pts[1]; w[1].p2 = pts[2]; w[1].c = c;
            w[2].p1 = pts[2]; w[2].p2 = pts[3]; w[2].c = c;
            w[3].p1 = pts[3]; w[3].p2 = pts[0]; w[3].c = c;

            Sector s = {};
            s.ws = wall_start;
            s.we = s.ws + 4;
            s.z1 = 0;
            s.z2 = h;
            s.cb = c;
            s.ct = c;
            s.d = 0;
            sectors.push_back(s);
        };

        // create sectors from a string definition
        // any non-space character is a wall for now
        // top left is (0,0), down is +y, right is +x
        const char *maze = 
        "################################"
        "# #    ###  ####   ###  ## #  ##"
        "# ####     #   # #  #  ### ## ##"
        "###  # ### ### # ##      #    ##"
        "# # ## ##      # #  #### ## #  #"
        "#   #     ###  # # ##     # ## #"
        "###   ########   #  ####### ## #"
        "#####        ###          #    #"
        "#     ## # #    ######### ## ###"
        "### # #  # #### ##           # #"
        "#   # #          # ####  ### ###"
        "## ## ####### ## #       #    ##"
        "##  #     #   #    ##### ## ####"
        "##    ### # # # ##     #    ## #"
        "## #      # ### # # #  # ##    #"
        "## ########     # # ## # #######"
        "##           #  # #  # #      ##"
        "#  ## ##  ## #    #    #### ## #"
        "#  ## ## ### ### ##### #     # #"
        "# #   #  ##  #   #       ### # #"
        "# # # # #   ## #   #####   #####"
        "##### ### #    ###     ###    ##"
        "#         #### #   ##    # ##  #"
        "# #  ####    # # ###  ####  # ##"
        "# ##  # # #  ### #   ##     # ##"
        "#  ## #   ##     #      ##### ##"
        "##  #   #  ### # # ####        #"
        "##### # # #    # # ####  #######"
        "#     ### # #### #            ##"
        "#  ###      # #    ########## ##"
        "## # # ###### # ##    #  # #####"
        "################################";
        const int COLUMN_COUNT = 32;
        const int ROW_COUNT = strlen(maze) / COLUMN_COUNT;
        for (int r = 0; r < ROW_COUNT; r++)
        {
            for (int c = 0; c < COLUMN_COUNT; c++)
            {
                int i = r * COLUMN_COUNT + c;
                if (' ' != maze[i])
                {
                    V2 point = { c * DIM, r * DIM };
                    PushBoxSector(point, i % Color_Background, 40);
                }
            }
        }

#if 0
        // placeholder for missing wall/sectors at part 1 18:35
        FILE *f = fopen("missing.txt", "wb");
        fprintf(f, "int loadSectors[]=\n{\n");
        for (uint i = 0; i < sector_count; i++)
        {
            Sector *s = &sectors[i];
            fprintf(f, "%d,%d, %d,%d, %d,%d,\n", 
                    s->ws, s->we, s->z1, s->z2, s->p.x, s->p.y);
        }
        fprintf(f, "};\n\n");

        fprintf(f, "int loadWalls[]=\n{\n");
        for (uint i = 0; i < wall_count; i++)
        {
            Wall *w = &walls[i];
            fprintf(f, "%d,%d, %d,%d, %d,\n",
                    w->p1.x, w->p1.y, w->p2.x, w->p2.y, w->c);

        }
        fprintf(f, "};\n\n");
        fclose(f); f = NULL;
#endif
    }

    // enable debug function for opengl 4.3 and higher
    const char *version = (const char *)glGetString(GL_VERSION);
    if (version)
    {
        typedef void *WGLGETPROC(const char *);
        typedef void APIENTRY GLDEBUGPROC(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei Length,const char *message,const void *userParam);
        typedef void APIENTRY GLCALLBACKPROC(GLDEBUGPROC *callback, const void *userParam);
        typedef void APIENTRY GLCONTROLPROC(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
        typedef BOOL WINAPI WGLSWAPINTERVALEXTPROC(int interval);
        typedef int WINAPI WGLGETSWAPINTERVALEXTPROC(void);

        int major = 0;
        int minor = 0;
        if (2 == sscanf(version, "%d.%d", &major, &minor) &&
            major >= 4 && minor >= 3)
        {
            static HMODULE libGL = LoadLibraryW(L"opengl32.dll");
            if (!libGL) Fatal("LoadLibraryW %s", ErrorWin32());
            atexit([](){ FreeLibrary(libGL); libGL = 0; });

            WGLGETPROC *wglGetProc = (WGLGETPROC *)GetProcAddress(libGL, "wglGetProcAddress");
            if (!wglGetProc) Fatal("GetProcAddress %s", ErrorWin32());

            const auto GetProc = [=](const char *name) -> void *
            {
                void *result = (void *)wglGetProc(name); 

                if (result == NULL)
                    result = (void *)GetProcAddress(libGL, name);

                if (result == NULL)
                    Fatal("opengl function not found: %s", name);

                return result;
            };

            const int GL_DEBUG_OUTPUT_SYNCHRONOUS = 0x8242;
            const int GL_DEBUG_OUTPUT = 0x92E0;

            GLCALLBACKPROC *glDebugMessageCallback = (GLCALLBACKPROC *)GetProc("glDebugMessageCallback");
            GLCONTROLPROC *glDebugMessageControl = (GLCONTROLPROC *)GetProc("glDebugMessageControl");
            //WGLSWAPINTERVALEXTPROC *wglSwapIntervalEXT = (WGLSWAPINTERVALEXTPROC *) GetProc("wglSwapIntervalEXT");
            //WGLGETSWAPINTERVALEXTPROC *wglGetSwapIntervalEXT = (WGLGETSWAPINTERVALEXTPROC *) GetProc("wglGetSwapIntervalEXT");

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
    return 0;
}

// (0,0) is bottom left, newline advances upwards`
void DrawText(BitmapFont *font, const char *text, int bx, int by, int color)
{
    int x = bx;
    int y = by;
    for (const char *iter = text;
         iter && *iter;
         iter++)
    {
        uint8 c = (uint8)*iter;
        if (c == '\n')
        {
            x = bx;
            y += font->height;
        }

        uint ibit = 0;
        BitmapFont::Char *ch = &font->ascii[c];
        uint8 *base = &font->data[ch->offset_to_data];
        for (uint row = 0; row < ch->height; row++)
        {
            for (uint col = 0; col < ch->width; col++)
            {
                uint flag = (1 << (7 - (ibit % 8)));
                if (base[ibit / 8] & flag)
                    DrawPixel(ch->offset_x + x + col,
                              ch->offset_y + y + (ch->height - row),
                              color);

                ibit += 1;
            }

            ibit = Align(ibit, 8);
        }

        x += ch->nextx;
        y += ch->nexty;
    }
}

void DrawText(const FONT *font, const char *text, int bx, int by, int color)
{
    const FONTINFO *info = &font->header;

    // dfBitsOffset relative to fpos 0, FONT::data is data after header
    size_t glyph_base = info->dfBitsOffset - sizeof(FONTINFO);  
    int pixel_byte_width = (info->dfPixWidth + 7) / 8; // round up byte alignment
    size_t bytes_per_char = info->dfPixHeight * pixel_byte_width;
    size_t pixel_stride = info->dfPixHeight * info->dfPixWidth;

    int x = bx;
    int y = by;
    for (const char *iter = text;
         iter && *iter;
         iter++)
    {
        uint8 c = (uint8)*iter;
        if (c == '\n')
        {
            x = bx;
            y += info->dfPixHeight;
        }

        uint offset = (c >= info->dfFirstChar && c <= info->dfLastChar)
                      ? c - info->dfFirstChar : info->dfDefaultChar - info->dfFirstChar;

        size_t glyph_offset = glyph_base + bytes_per_char * offset;
        for (size_t j = 0; j < bytes_per_char; j++)
        {
            uint64 pixcol = (j / info->dfPixHeight) * 8;
            uint64 pixrow = (j % info->dfPixHeight);

            Assert(glyph_offset < font->data.num_bytes);
            uint8 byte = font->data.bytes[ glyph_offset++ ];
            uint64 bit_iter = pixcol;

            for (int bit = 0; bit < 8; bit++)
            {
                if (bit_iter < info->dfPixWidth)
                {
                    // (0,0) is bottom left so needs to flip drawing pixels
                    int dc = ((0x80 & byte) != 0) ? color : Color_Black;
                    DrawPixel(x + bit_iter, y + (info->dfPixHeight - pixrow), dc);

                    bit_iter += 1;
                    byte <<= 1;
                }

            }
        }

        x += info->dfPixWidth;
    }
}


// KBT impl
#include <kbt\kbt_win32.cpp>
#include <kbt\kbt_bitmap_font.cpp>
