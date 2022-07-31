#pragma once

#include <Windows.h> // DebugBreak()
#include <stdio.h>

static struct
{
    bool in_begin_end;
} opengl;

inline void glCheckError()
{
    if (opengl.in_begin_end) return; // cant call this inside glBegin/glEnd pair

    GLenum err = glGetError();
    if (err == GL_NO_ERROR) return;

    const char *enum_msg = "";
    switch (err) 
    {
        case GL_INVALID_ENUM:                   enum_msg = "invalid enum";      break;
        case GL_INVALID_VALUE:                  enum_msg = "invalid value";     break;
        case GL_INVALID_OPERATION:              enum_msg = "invalid operation"; break; 
        //case GL_INVALID_FRAMEBUFFER_OPERATION:  enum_msg = "invalid framebuffer operation"; break;
        case GL_OUT_OF_MEMORY:                  enum_msg = "out of memory";     break;
        case GL_STACK_UNDERFLOW:                enum_msg = "stack underflow";   break;
        case GL_STACK_OVERFLOW:                 enum_msg = "stack overflow";    break;
    }

    fprintf(stderr, "opengl error enum: %i, message: %s\n", err, enum_msg);
    DebugBreak();
}

// debug evaluation of opengl functions without having to check
// glGetError() == GL_NO_ERROR every time you call a function

#if defined(CHECK_GL)
void _glAccum (GLenum op, GLfloat value)
{
    glAccum (op, value) ;
    glCheckError();
}
#undef glAccum 
#define glAccum  _glAccum 

void _glAlphaFunc (GLenum func, GLclampf ref)
{
    glAlphaFunc (func, ref) ;
    glCheckError();
}
#undef glAlphaFunc 
#define glAlphaFunc  _glAlphaFunc 

void _glArrayElement (GLint i)
{
    glArrayElement (i) ;
    glCheckError();
}
#undef glArrayElement 
#define glArrayElement  _glArrayElement 

void _glBegin (GLenum mode)
{
    Assert(!opengl.in_begin_end);
    glBegin (mode) ;
    opengl.in_begin_end = true;
}
#undef glBegin 
#define glBegin  _glBegin 

void _glBindTexture (GLenum target, GLuint texture)
{
    glBindTexture (target, texture) ;
    glCheckError();
}
#undef glBindTexture 
#define glBindTexture  _glBindTexture 

void _glBitmap (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
{
    glBitmap (width, height, xorig, yorig, xmove, ymove, bitmap) ;
    glCheckError();
}
#undef glBitmap 
#define glBitmap  _glBitmap 

void _glBlendFunc (GLenum sfactor, GLenum dfactor)
{
    glBlendFunc (sfactor, dfactor) ;
    glCheckError();
}
#undef glBlendFunc 
#define glBlendFunc  _glBlendFunc 

void _glCallList (GLuint list)
{
    glCallList (list) ;
    glCheckError();
}
#undef glCallList 
#define glCallList  _glCallList 

void _glCallLists (GLsizei n, GLenum type, const GLvoid *lists)
{
    glCallLists (n, type, lists) ;
    glCheckError();
}
#undef glCallLists 
#define glCallLists  _glCallLists 

void _glClear (GLbitfield mask)
{
    glClear (mask) ;
    glCheckError();
}
#undef glClear 
#define glClear  _glClear 

void _glClearAccum (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearAccum (red, green, blue, alpha) ;
    glCheckError();
}
#undef glClearAccum 
#define glClearAccum  _glClearAccum 

void _glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    glClearColor (red, green, blue, alpha) ;
    glCheckError();
}
#undef glClearColor 
#define glClearColor  _glClearColor 

void _glClearDepth (GLclampd depth)
{
    glClearDepth (depth) ;
    glCheckError();
}
#undef glClearDepth 
#define glClearDepth  _glClearDepth 

void _glClearIndex (GLfloat c)
{
    glClearIndex (c) ;
    glCheckError();
}
#undef glClearIndex 
#define glClearIndex  _glClearIndex 

void _glClearStencil (GLint s)
{
    glClearStencil (s) ;
    glCheckError();
}
#undef glClearStencil 
#define glClearStencil  _glClearStencil 

void _glClipPlane (GLenum plane, const GLdouble *equation)
{
    glClipPlane (plane, equation) ;
    glCheckError();
}
#undef glClipPlane 
#define glClipPlane  _glClipPlane 

void _glColor3b (GLbyte red, GLbyte green, GLbyte blue)
{
    glColor3b (red, green, blue) ;
    glCheckError();
}
#undef glColor3b 
#define glColor3b  _glColor3b 

void _glColor3bv (const GLbyte *v)
{
    glColor3bv (v) ;
    glCheckError();
}
#undef glColor3bv 
#define glColor3bv  _glColor3bv 

void _glColor3d (GLdouble red, GLdouble green, GLdouble blue)
{
    glColor3d (red, green, blue) ;
    glCheckError();
}
#undef glColor3d 
#define glColor3d  _glColor3d 

void _glColor3dv (const GLdouble *v)
{
    glColor3dv (v) ;
    glCheckError();
}
#undef glColor3dv 
#define glColor3dv  _glColor3dv 

void _glColor3f (GLfloat red, GLfloat green, GLfloat blue)
{
    glColor3f (red, green, blue) ;
    glCheckError();
}
#undef glColor3f 
#define glColor3f  _glColor3f 

void _glColor3fv (const GLfloat *v)
{
    glColor3fv (v) ;
    glCheckError();
}
#undef glColor3fv 
#define glColor3fv  _glColor3fv 

void _glColor3i (GLint red, GLint green, GLint blue)
{
    glColor3i (red, green, blue) ;
    glCheckError();
}
#undef glColor3i 
#define glColor3i  _glColor3i 

void _glColor3iv (const GLint *v)
{
    glColor3iv (v) ;
    glCheckError();
}
#undef glColor3iv 
#define glColor3iv  _glColor3iv 

void _glColor3s (GLshort red, GLshort green, GLshort blue)
{
    glColor3s (red, green, blue) ;
    glCheckError();
}
#undef glColor3s 
#define glColor3s  _glColor3s 

void _glColor3sv (const GLshort *v)
{
    glColor3sv (v) ;
    glCheckError();
}
#undef glColor3sv 
#define glColor3sv  _glColor3sv 

void _glColor3ub (GLubyte red, GLubyte green, GLubyte blue)
{
    glColor3ub (red, green, blue) ;
    glCheckError();
}
#undef glColor3ub 
#define glColor3ub  _glColor3ub 

void _glColor3ubv (const GLubyte *v)
{
    glColor3ubv (v) ;
    glCheckError();
}
#undef glColor3ubv 
#define glColor3ubv  _glColor3ubv 

void _glColor3ui (GLuint red, GLuint green, GLuint blue)
{
    glColor3ui (red, green, blue) ;
    glCheckError();
}
#undef glColor3ui 
#define glColor3ui  _glColor3ui 

void _glColor3uiv (const GLuint *v)
{
    glColor3uiv (v) ;
    glCheckError();
}
#undef glColor3uiv 
#define glColor3uiv  _glColor3uiv 

void _glColor3us (GLushort red, GLushort green, GLushort blue)
{
    glColor3us (red, green, blue) ;
    glCheckError();
}
#undef glColor3us 
#define glColor3us  _glColor3us 

void _glColor3usv (const GLushort *v)
{
    glColor3usv (v) ;
    glCheckError();
}
#undef glColor3usv 
#define glColor3usv  _glColor3usv 

void _glColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
{
    glColor4b (red, green, blue, alpha) ;
    glCheckError();
}
#undef glColor4b 
#define glColor4b  _glColor4b 

void _glColor4bv (const GLbyte *v)
{
    glColor4bv (v) ;
    glCheckError();
}
#undef glColor4bv 
#define glColor4bv  _glColor4bv 

void _glColor4d (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
    glColor4d (red, green, blue, alpha) ;
    glCheckError();
}
#undef glColor4d 
#define glColor4d  _glColor4d 

void _glColor4dv (const GLdouble *v)
{
    glColor4dv (v) ;
    glCheckError();
}
#undef glColor4dv 
#define glColor4dv  _glColor4dv 

void _glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glColor4f (red, green, blue, alpha) ;
    glCheckError();
}
#undef glColor4f 
#define glColor4f  _glColor4f 

void _glColor4fv (const GLfloat *v)
{
    glColor4fv (v) ;
    glCheckError();
}
#undef glColor4fv 
#define glColor4fv  _glColor4fv 

void _glColor4i (GLint red, GLint green, GLint blue, GLint alpha)
{
    glColor4i (red, green, blue, alpha) ;
    glCheckError();
}
#undef glColor4i 
#define glColor4i  _glColor4i 

void _glColor4iv (const GLint *v)
{
    glColor4iv (v) ;
    glCheckError();
}
#undef glColor4iv 
#define glColor4iv  _glColor4iv 

void _glColor4s (GLshort red, GLshort green, GLshort blue, GLshort alpha)
{
    glColor4s (red, green, blue, alpha) ;
    glCheckError();
}
#undef glColor4s 
#define glColor4s  _glColor4s 

void _glColor4sv (const GLshort *v)
{
    glColor4sv (v) ;
    glCheckError();
}
#undef glColor4sv 
#define glColor4sv  _glColor4sv 

void _glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
    glColor4ub (red, green, blue, alpha) ;
    glCheckError();
}
#undef glColor4ub 
#define glColor4ub  _glColor4ub 

void _glColor4ubv (const GLubyte *v)
{
    glColor4ubv (v) ;
    glCheckError();
}
#undef glColor4ubv 
#define glColor4ubv  _glColor4ubv 

void _glColor4ui (GLuint red, GLuint green, GLuint blue, GLuint alpha)
{
    glColor4ui (red, green, blue, alpha) ;
    glCheckError();
}
#undef glColor4ui 
#define glColor4ui  _glColor4ui 

void _glColor4uiv (const GLuint *v)
{
    glColor4uiv (v) ;
    glCheckError();
}
#undef glColor4uiv 
#define glColor4uiv  _glColor4uiv 

void _glColor4us (GLushort red, GLushort green, GLushort blue, GLushort alpha)
{
    glColor4us (red, green, blue, alpha) ;
    glCheckError();
}
#undef glColor4us 
#define glColor4us  _glColor4us 

void _glColor4usv (const GLushort *v)
{
    glColor4usv (v) ;
    glCheckError();
}
#undef glColor4usv 
#define glColor4usv  _glColor4usv 

void _glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    glColorMask (red, green, blue, alpha) ;
    glCheckError();
}
#undef glColorMask 
#define glColorMask  _glColorMask 

void _glColorMaterial (GLenum face, GLenum mode)
{
    glColorMaterial (face, mode) ;
    glCheckError();
}
#undef glColorMaterial 
#define glColorMaterial  _glColorMaterial 

void _glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    glColorPointer (size, type, stride, pointer) ;
    glCheckError();
}
#undef glColorPointer 
#define glColorPointer  _glColorPointer 

void _glCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
{
    glCopyPixels (x, y, width, height, type) ;
    glCheckError();
}
#undef glCopyPixels 
#define glCopyPixels  _glCopyPixels 

void _glCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border)
{
    glCopyTexImage1D (target, level, internalFormat, x, y, width, border) ;
    glCheckError();
}
#undef glCopyTexImage1D 
#define glCopyTexImage1D  _glCopyTexImage1D 

void _glCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    glCopyTexImage2D (target, level, internalFormat, x, y, width, height, border) ;
    glCheckError();
}
#undef glCopyTexImage2D 
#define glCopyTexImage2D  _glCopyTexImage2D 

void _glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
    glCopyTexSubImage1D (target, level, xoffset, x, y, width) ;
    glCheckError();
}
#undef glCopyTexSubImage1D 
#define glCopyTexSubImage1D  _glCopyTexSubImage1D 

void _glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    glCopyTexSubImage2D (target, level, xoffset, yoffset, x, y, width, height) ;
    glCheckError();
}
#undef glCopyTexSubImage2D 
#define glCopyTexSubImage2D  _glCopyTexSubImage2D 

void _glCullFace (GLenum mode)
{
    glCullFace (mode) ;
    glCheckError();
}
#undef glCullFace 
#define glCullFace  _glCullFace 

void _glDeleteLists (GLuint list, GLsizei range)
{
    glDeleteLists (list, range) ;
    glCheckError();
}
#undef glDeleteLists 
#define glDeleteLists  _glDeleteLists 

void _glDeleteTextures (GLsizei n, const GLuint *textures)
{
    glDeleteTextures (n, textures) ;
    glCheckError();
}
#undef glDeleteTextures 
#define glDeleteTextures  _glDeleteTextures 

void _glDepthFunc (GLenum func)
{
    glDepthFunc (func) ;
    glCheckError();
}
#undef glDepthFunc 
#define glDepthFunc  _glDepthFunc 

void _glDepthMask (GLboolean flag)
{
    glDepthMask (flag) ;
    glCheckError();
}
#undef glDepthMask 
#define glDepthMask  _glDepthMask 

void _glDepthRange (GLclampd zNear, GLclampd zFar)
{
    glDepthRange (zNear, zFar) ;
    glCheckError();
}
#undef glDepthRange 
#define glDepthRange  _glDepthRange 

void _glDisable (GLenum cap)
{
    glDisable (cap) ;
    glCheckError();
}
#undef glDisable 
#define glDisable  _glDisable 

void _glDisableClientState (GLenum array)
{
    glDisableClientState (array) ;
    glCheckError();
}
#undef glDisableClientState 
#define glDisableClientState  _glDisableClientState 

void _glDrawArrays (GLenum mode, GLint first, GLsizei count)
{
    glDrawArrays (mode, first, count) ;
    glCheckError();
}
#undef glDrawArrays 
#define glDrawArrays  _glDrawArrays 

void _glDrawBuffer (GLenum mode)
{
    glDrawBuffer (mode) ;
    glCheckError();
}
#undef glDrawBuffer 
#define glDrawBuffer  _glDrawBuffer 

void _glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    glDrawElements (mode, count, type, indices) ;
    glCheckError();
}
#undef glDrawElements 
#define glDrawElements  _glDrawElements 

void _glDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    glDrawPixels (width, height, format, type, pixels) ;
    glCheckError();
}
#undef glDrawPixels 
#define glDrawPixels  _glDrawPixels 

void _glEdgeFlag (GLboolean flag)
{
    glEdgeFlag (flag) ;
    glCheckError();
}
#undef glEdgeFlag 
#define glEdgeFlag  _glEdgeFlag 

void _glEdgeFlagPointer (GLsizei stride, const GLvoid *pointer)
{
    glEdgeFlagPointer (stride, pointer) ;
    glCheckError();
}
#undef glEdgeFlagPointer 
#define glEdgeFlagPointer  _glEdgeFlagPointer 

void _glEdgeFlagv (const GLboolean *flag)
{
    glEdgeFlagv (flag) ;
    glCheckError();
}
#undef glEdgeFlagv 
#define glEdgeFlagv  _glEdgeFlagv 

void _glEnable (GLenum cap)
{
    glEnable (cap) ;
    glCheckError();
}
#undef glEnable 
#define glEnable  _glEnable 

void _glEnableClientState (GLenum array)
{
    glEnableClientState (array) ;
    glCheckError();
}
#undef glEnableClientState 
#define glEnableClientState  _glEnableClientState 

void _glEnd (void)
{
    Assert(opengl.in_begin_end);
    glEnd () ;
    opengl.in_begin_end = false;
}
#undef glEnd 
#define glEnd  _glEnd 

void _glEndList (void)
{
    glEndList () ;
    glCheckError();
}
#undef glEndList 
#define glEndList  _glEndList 

void _glEvalCoord1d (GLdouble u)
{
    glEvalCoord1d (u) ;
    glCheckError();
}
#undef glEvalCoord1d 
#define glEvalCoord1d  _glEvalCoord1d 

void _glEvalCoord1dv (const GLdouble *u)
{
    glEvalCoord1dv (u) ;
    glCheckError();
}
#undef glEvalCoord1dv 
#define glEvalCoord1dv  _glEvalCoord1dv 

void _glEvalCoord1f (GLfloat u)
{
    glEvalCoord1f (u) ;
    glCheckError();
}
#undef glEvalCoord1f 
#define glEvalCoord1f  _glEvalCoord1f 

void _glEvalCoord1fv (const GLfloat *u)
{
    glEvalCoord1fv (u) ;
    glCheckError();
}
#undef glEvalCoord1fv 
#define glEvalCoord1fv  _glEvalCoord1fv 

void _glEvalCoord2d (GLdouble u, GLdouble v)
{
    glEvalCoord2d (u, v) ;
    glCheckError();
}
#undef glEvalCoord2d 
#define glEvalCoord2d  _glEvalCoord2d 

void _glEvalCoord2dv (const GLdouble *u)
{
    glEvalCoord2dv (u) ;
    glCheckError();
}
#undef glEvalCoord2dv 
#define glEvalCoord2dv  _glEvalCoord2dv 

void _glEvalCoord2f (GLfloat u, GLfloat v)
{
    glEvalCoord2f (u, v) ;
    glCheckError();
}
#undef glEvalCoord2f 
#define glEvalCoord2f  _glEvalCoord2f 

void _glEvalCoord2fv (const GLfloat *u)
{
    glEvalCoord2fv (u) ;
    glCheckError();
}
#undef glEvalCoord2fv 
#define glEvalCoord2fv  _glEvalCoord2fv 

void _glEvalMesh1 (GLenum mode, GLint i1, GLint i2)
{
    glEvalMesh1 (mode, i1, i2) ;
    glCheckError();
}
#undef glEvalMesh1 
#define glEvalMesh1  _glEvalMesh1 

void _glEvalMesh2 (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
{
    glEvalMesh2 (mode, i1, i2, j1, j2) ;
    glCheckError();
}
#undef glEvalMesh2 
#define glEvalMesh2  _glEvalMesh2 

void _glEvalPoint1 (GLint i)
{
    glEvalPoint1 (i) ;
    glCheckError();
}
#undef glEvalPoint1 
#define glEvalPoint1  _glEvalPoint1 

void _glEvalPoint2 (GLint i, GLint j)
{
    glEvalPoint2 (i, j) ;
    glCheckError();
}
#undef glEvalPoint2 
#define glEvalPoint2  _glEvalPoint2 

void _glFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer)
{
    glFeedbackBuffer (size, type, buffer) ;
    glCheckError();
}
#undef glFeedbackBuffer 
#define glFeedbackBuffer  _glFeedbackBuffer 

void _glFinish (void)
{
    glFinish () ;
    glCheckError();
}
#undef glFinish 
#define glFinish  _glFinish 

void _glFlush (void)
{
    glFlush () ;
    glCheckError();
}
#undef glFlush 
#define glFlush  _glFlush 

void _glFogf (GLenum pname, GLfloat param)
{
    glFogf (pname, param) ;
    glCheckError();
}
#undef glFogf 
#define glFogf  _glFogf 

void _glFogfv (GLenum pname, const GLfloat *params)
{
    glFogfv (pname, params) ;
    glCheckError();
}
#undef glFogfv 
#define glFogfv  _glFogfv 

void _glFogi (GLenum pname, GLint param)
{
    glFogi (pname, param) ;
    glCheckError();
}
#undef glFogi 
#define glFogi  _glFogi 

void _glFogiv (GLenum pname, const GLint *params)
{
    glFogiv (pname, params) ;
    glCheckError();
}
#undef glFogiv 
#define glFogiv  _glFogiv 

void _glFrontFace (GLenum mode)
{
    glFrontFace (mode) ;
    glCheckError();
}
#undef glFrontFace 
#define glFrontFace  _glFrontFace 

void _glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    glFrustum (left, right, bottom, top, zNear, zFar) ;
    glCheckError();
}
#undef glFrustum 
#define glFrustum  _glFrustum 

void _glGenTextures (GLsizei n, GLuint *textures)
{
    glGenTextures (n, textures) ;
    glCheckError();
}
#undef glGenTextures 
#define glGenTextures  _glGenTextures 

void _glGetBooleanv (GLenum pname, GLboolean *params)
{
    glGetBooleanv (pname, params) ;
    glCheckError();
}
#undef glGetBooleanv 
#define glGetBooleanv  _glGetBooleanv 

void _glGetClipPlane (GLenum plane, GLdouble *equation)
{
    glGetClipPlane (plane, equation) ;
    glCheckError();
}
#undef glGetClipPlane 
#define glGetClipPlane  _glGetClipPlane 

void _glGetDoublev (GLenum pname, GLdouble *params)
{
    glGetDoublev (pname, params) ;
    glCheckError();
}
#undef glGetDoublev 
#define glGetDoublev  _glGetDoublev 

void _glGetFloatv (GLenum pname, GLfloat *params)
{
    glGetFloatv (pname, params) ;
    glCheckError();
}
#undef glGetFloatv 
#define glGetFloatv  _glGetFloatv 

void _glGetIntegerv (GLenum pname, GLint *params)
{
    glGetIntegerv (pname, params) ;
    glCheckError();
}
#undef glGetIntegerv 
#define glGetIntegerv  _glGetIntegerv 

void _glGetLightfv (GLenum light, GLenum pname, GLfloat *params)
{
    glGetLightfv (light, pname, params) ;
    glCheckError();
}
#undef glGetLightfv 
#define glGetLightfv  _glGetLightfv 

void _glGetLightiv (GLenum light, GLenum pname, GLint *params)
{
    glGetLightiv (light, pname, params) ;
    glCheckError();
}
#undef glGetLightiv 
#define glGetLightiv  _glGetLightiv 

void _glGetMapdv (GLenum target, GLenum query, GLdouble *v)
{
    glGetMapdv (target, query, v) ;
    glCheckError();
}
#undef glGetMapdv 
#define glGetMapdv  _glGetMapdv 

void _glGetMapfv (GLenum target, GLenum query, GLfloat *v)
{
    glGetMapfv (target, query, v) ;
    glCheckError();
}
#undef glGetMapfv 
#define glGetMapfv  _glGetMapfv 

void _glGetMapiv (GLenum target, GLenum query, GLint *v)
{
    glGetMapiv (target, query, v) ;
    glCheckError();
}
#undef glGetMapiv 
#define glGetMapiv  _glGetMapiv 

void _glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params)
{
    glGetMaterialfv (face, pname, params) ;
    glCheckError();
}
#undef glGetMaterialfv 
#define glGetMaterialfv  _glGetMaterialfv 

void _glGetMaterialiv (GLenum face, GLenum pname, GLint *params)
{
    glGetMaterialiv (face, pname, params) ;
    glCheckError();
}
#undef glGetMaterialiv 
#define glGetMaterialiv  _glGetMaterialiv 

void _glGetPixelMapfv (GLenum map, GLfloat *values)
{
    glGetPixelMapfv (map, values) ;
    glCheckError();
}
#undef glGetPixelMapfv 
#define glGetPixelMapfv  _glGetPixelMapfv 

void _glGetPixelMapuiv (GLenum map, GLuint *values)
{
    glGetPixelMapuiv (map, values) ;
    glCheckError();
}
#undef glGetPixelMapuiv 
#define glGetPixelMapuiv  _glGetPixelMapuiv 

void _glGetPixelMapusv (GLenum map, GLushort *values)
{
    glGetPixelMapusv (map, values) ;
    glCheckError();
}
#undef glGetPixelMapusv 
#define glGetPixelMapusv  _glGetPixelMapusv 

void _glGetPointerv (GLenum pname, GLvoid* *params)
{
    glGetPointerv (pname, params) ;
    glCheckError();
}
#undef glGetPointerv 
#define glGetPointerv  _glGetPointerv 

void _glGetPolygonStipple (GLubyte *mask)
{
    glGetPolygonStipple (mask) ;
    glCheckError();
}
#undef glGetPolygonStipple 
#define glGetPolygonStipple  _glGetPolygonStipple 

void _glGetTexEnvfv (GLenum target, GLenum pname, GLfloat *params)
{
    glGetTexEnvfv (target, pname, params) ;
    glCheckError();
}
#undef glGetTexEnvfv 
#define glGetTexEnvfv  _glGetTexEnvfv 

void _glGetTexEnviv (GLenum target, GLenum pname, GLint *params)
{
    glGetTexEnviv (target, pname, params) ;
    glCheckError();
}
#undef glGetTexEnviv 
#define glGetTexEnviv  _glGetTexEnviv 

void _glGetTexGendv (GLenum coord, GLenum pname, GLdouble *params)
{
    glGetTexGendv (coord, pname, params) ;
    glCheckError();
}
#undef glGetTexGendv 
#define glGetTexGendv  _glGetTexGendv 

void _glGetTexGenfv (GLenum coord, GLenum pname, GLfloat *params)
{
    glGetTexGenfv (coord, pname, params) ;
    glCheckError();
}
#undef glGetTexGenfv 
#define glGetTexGenfv  _glGetTexGenfv 

void _glGetTexGeniv (GLenum coord, GLenum pname, GLint *params)
{
    glGetTexGeniv (coord, pname, params) ;
    glCheckError();
}
#undef glGetTexGeniv 
#define glGetTexGeniv  _glGetTexGeniv 

void _glGetTexImage (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
{
    glGetTexImage (target, level, format, type, pixels) ;
    glCheckError();
}
#undef glGetTexImage 
#define glGetTexImage  _glGetTexImage 

void _glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params)
{
    glGetTexLevelParameterfv (target, level, pname, params) ;
    glCheckError();
}
#undef glGetTexLevelParameterfv 
#define glGetTexLevelParameterfv  _glGetTexLevelParameterfv 

void _glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params)
{
    glGetTexLevelParameteriv (target, level, pname, params) ;
    glCheckError();
}
#undef glGetTexLevelParameteriv 
#define glGetTexLevelParameteriv  _glGetTexLevelParameteriv 

void _glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params)
{
    glGetTexParameterfv (target, pname, params) ;
    glCheckError();
}
#undef glGetTexParameterfv 
#define glGetTexParameterfv  _glGetTexParameterfv 

void _glGetTexParameteriv (GLenum target, GLenum pname, GLint *params)
{
    glGetTexParameteriv (target, pname, params) ;
    glCheckError();
}
#undef glGetTexParameteriv 
#define glGetTexParameteriv  _glGetTexParameteriv 

void _glHint (GLenum target, GLenum mode)
{
    glHint (target, mode) ;
    glCheckError();
}
#undef glHint 
#define glHint  _glHint 

void _glIndexMask (GLuint mask)
{
    glIndexMask (mask) ;
    glCheckError();
}
#undef glIndexMask 
#define glIndexMask  _glIndexMask 

void _glIndexPointer (GLenum type, GLsizei stride, const GLvoid *pointer)
{
    glIndexPointer (type, stride, pointer) ;
    glCheckError();
}
#undef glIndexPointer 
#define glIndexPointer  _glIndexPointer 

void _glIndexd (GLdouble c)
{
    glIndexd (c) ;
    glCheckError();
}
#undef glIndexd 
#define glIndexd  _glIndexd 

void _glIndexdv (const GLdouble *c)
{
    glIndexdv (c) ;
    glCheckError();
}
#undef glIndexdv 
#define glIndexdv  _glIndexdv 

void _glIndexf (GLfloat c)
{
    glIndexf (c) ;
    glCheckError();
}
#undef glIndexf 
#define glIndexf  _glIndexf 

void _glIndexfv (const GLfloat *c)
{
    glIndexfv (c) ;
    glCheckError();
}
#undef glIndexfv 
#define glIndexfv  _glIndexfv 

void _glIndexi (GLint c)
{
    glIndexi (c) ;
    glCheckError();
}
#undef glIndexi 
#define glIndexi  _glIndexi 

void _glIndexiv (const GLint *c)
{
    glIndexiv (c) ;
    glCheckError();
}
#undef glIndexiv 
#define glIndexiv  _glIndexiv 

void _glIndexs (GLshort c)
{
    glIndexs (c) ;
    glCheckError();
}
#undef glIndexs 
#define glIndexs  _glIndexs 

void _glIndexsv (const GLshort *c)
{
    glIndexsv (c) ;
    glCheckError();
}
#undef glIndexsv 
#define glIndexsv  _glIndexsv 

void _glIndexub (GLubyte c)
{
    glIndexub (c) ;
    glCheckError();
}
#undef glIndexub 
#define glIndexub  _glIndexub 

void _glIndexubv (const GLubyte *c)
{
    glIndexubv (c) ;
    glCheckError();
}
#undef glIndexubv 
#define glIndexubv  _glIndexubv 

void _glInitNames (void)
{
    glInitNames () ;
    glCheckError();
}
#undef glInitNames 
#define glInitNames  _glInitNames 

void _glInterleavedArrays (GLenum format, GLsizei stride, const GLvoid *pointer)
{
    glInterleavedArrays (format, stride, pointer) ;
    glCheckError();
}
#undef glInterleavedArrays 
#define glInterleavedArrays  _glInterleavedArrays 

void _glLightModelf (GLenum pname, GLfloat param)
{
    glLightModelf (pname, param) ;
    glCheckError();
}
#undef glLightModelf 
#define glLightModelf  _glLightModelf 

void _glLightModelfv (GLenum pname, const GLfloat *params)
{
    glLightModelfv (pname, params) ;
    glCheckError();
}
#undef glLightModelfv 
#define glLightModelfv  _glLightModelfv 

void _glLightModeli (GLenum pname, GLint param)
{
    glLightModeli (pname, param) ;
    glCheckError();
}
#undef glLightModeli 
#define glLightModeli  _glLightModeli 

void _glLightModeliv (GLenum pname, const GLint *params)
{
    glLightModeliv (pname, params) ;
    glCheckError();
}
#undef glLightModeliv 
#define glLightModeliv  _glLightModeliv 

void _glLightf (GLenum light, GLenum pname, GLfloat param)
{
    glLightf (light, pname, param) ;
    glCheckError();
}
#undef glLightf 
#define glLightf  _glLightf 

void _glLightfv (GLenum light, GLenum pname, const GLfloat *params)
{
    glLightfv (light, pname, params) ;
    glCheckError();
}
#undef glLightfv 
#define glLightfv  _glLightfv 

void _glLighti (GLenum light, GLenum pname, GLint param)
{
    glLighti (light, pname, param) ;
    glCheckError();
}
#undef glLighti 
#define glLighti  _glLighti 

void _glLightiv (GLenum light, GLenum pname, const GLint *params)
{
    glLightiv (light, pname, params) ;
    glCheckError();
}
#undef glLightiv 
#define glLightiv  _glLightiv 

void _glLineStipple (GLint factor, GLushort pattern)
{
    glLineStipple (factor, pattern) ;
    glCheckError();
}
#undef glLineStipple 
#define glLineStipple  _glLineStipple 

void _glLineWidth (GLfloat width)
{
    glLineWidth (width) ;
    glCheckError();
}
#undef glLineWidth 
#define glLineWidth  _glLineWidth 

void _glListBase (GLuint base)
{
    glListBase (base) ;
    glCheckError();
}
#undef glListBase 
#define glListBase  _glListBase 

void _glLoadIdentity (void)
{
    glLoadIdentity () ;
    glCheckError();
}
#undef glLoadIdentity 
#define glLoadIdentity  _glLoadIdentity 

void _glLoadMatrixd (const GLdouble *m)
{
    glLoadMatrixd (m) ;
    glCheckError();
}
#undef glLoadMatrixd 
#define glLoadMatrixd  _glLoadMatrixd 

void _glLoadMatrixf (const GLfloat *m)
{
    glLoadMatrixf (m) ;
    glCheckError();
}
#undef glLoadMatrixf 
#define glLoadMatrixf  _glLoadMatrixf 

void _glLoadName (GLuint name)
{
    glLoadName (name) ;
    glCheckError();
}
#undef glLoadName 
#define glLoadName  _glLoadName 

void _glLogicOp (GLenum opcode)
{
    glLogicOp (opcode) ;
    glCheckError();
}
#undef glLogicOp 
#define glLogicOp  _glLogicOp 

void _glMap1d (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points)
{
    glMap1d (target, u1, u2, stride, order, points) ;
    glCheckError();
}
#undef glMap1d 
#define glMap1d  _glMap1d 

void _glMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points)
{
    glMap1f (target, u1, u2, stride, order, points) ;
    glCheckError();
}
#undef glMap1f 
#define glMap1f  _glMap1f 

void _glMap2d (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points)
{
    glMap2d (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points) ;
    glCheckError();
}
#undef glMap2d 
#define glMap2d  _glMap2d 

void _glMap2f (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points)
{
    glMap2f (target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points) ;
    glCheckError();
}
#undef glMap2f 
#define glMap2f  _glMap2f 

void _glMapGrid1d (GLint un, GLdouble u1, GLdouble u2)
{
    glMapGrid1d (un, u1, u2) ;
    glCheckError();
}
#undef glMapGrid1d 
#define glMapGrid1d  _glMapGrid1d 

void _glMapGrid1f (GLint un, GLfloat u1, GLfloat u2)
{
    glMapGrid1f (un, u1, u2) ;
    glCheckError();
}
#undef glMapGrid1f 
#define glMapGrid1f  _glMapGrid1f 

void _glMapGrid2d (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2)
{
    glMapGrid2d (un, u1, u2, vn, v1, v2) ;
    glCheckError();
}
#undef glMapGrid2d 
#define glMapGrid2d  _glMapGrid2d 

void _glMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2)
{
    glMapGrid2f (un, u1, u2, vn, v1, v2) ;
    glCheckError();
}
#undef glMapGrid2f 
#define glMapGrid2f  _glMapGrid2f 

void _glMaterialf (GLenum face, GLenum pname, GLfloat param)
{
    glMaterialf (face, pname, param) ;
    glCheckError();
}
#undef glMaterialf 
#define glMaterialf  _glMaterialf 

void _glMaterialfv (GLenum face, GLenum pname, const GLfloat *params)
{
    glMaterialfv (face, pname, params) ;
    glCheckError();
}
#undef glMaterialfv 
#define glMaterialfv  _glMaterialfv 

void _glMateriali (GLenum face, GLenum pname, GLint param)
{
    glMateriali (face, pname, param) ;
    glCheckError();
}
#undef glMateriali 
#define glMateriali  _glMateriali 

void _glMaterialiv (GLenum face, GLenum pname, const GLint *params)
{
    glMaterialiv (face, pname, params) ;
    glCheckError();
}
#undef glMaterialiv 
#define glMaterialiv  _glMaterialiv 

void _glMatrixMode (GLenum mode)
{
    glMatrixMode (mode) ;
    glCheckError();
}
#undef glMatrixMode 
#define glMatrixMode  _glMatrixMode 

void _glMultMatrixd (const GLdouble *m)
{
    glMultMatrixd (m) ;
    glCheckError();
}
#undef glMultMatrixd 
#define glMultMatrixd  _glMultMatrixd 

void _glMultMatrixf (const GLfloat *m)
{
    glMultMatrixf (m) ;
    glCheckError();
}
#undef glMultMatrixf 
#define glMultMatrixf  _glMultMatrixf 

void _glNewList (GLuint list, GLenum mode)
{
    glNewList (list, mode) ;
    glCheckError();
}
#undef glNewList 
#define glNewList  _glNewList 

void _glNormal3b (GLbyte nx, GLbyte ny, GLbyte nz)
{
    glNormal3b (nx, ny, nz) ;
    glCheckError();
}
#undef glNormal3b 
#define glNormal3b  _glNormal3b 

void _glNormal3bv (const GLbyte *v)
{
    glNormal3bv (v) ;
    glCheckError();
}
#undef glNormal3bv 
#define glNormal3bv  _glNormal3bv 

void _glNormal3d (GLdouble nx, GLdouble ny, GLdouble nz)
{
    glNormal3d (nx, ny, nz) ;
    glCheckError();
}
#undef glNormal3d 
#define glNormal3d  _glNormal3d 

void _glNormal3dv (const GLdouble *v)
{
    glNormal3dv (v) ;
    glCheckError();
}
#undef glNormal3dv 
#define glNormal3dv  _glNormal3dv 

void _glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz)
{
    glNormal3f (nx, ny, nz) ;
    glCheckError();
}
#undef glNormal3f 
#define glNormal3f  _glNormal3f 

void _glNormal3fv (const GLfloat *v)
{
    glNormal3fv (v) ;
    glCheckError();
}
#undef glNormal3fv 
#define glNormal3fv  _glNormal3fv 

void _glNormal3i (GLint nx, GLint ny, GLint nz)
{
    glNormal3i (nx, ny, nz) ;
    glCheckError();
}
#undef glNormal3i 
#define glNormal3i  _glNormal3i 

void _glNormal3iv (const GLint *v)
{
    glNormal3iv (v) ;
    glCheckError();
}
#undef glNormal3iv 
#define glNormal3iv  _glNormal3iv 

void _glNormal3s (GLshort nx, GLshort ny, GLshort nz)
{
    glNormal3s (nx, ny, nz) ;
    glCheckError();
}
#undef glNormal3s 
#define glNormal3s  _glNormal3s 

void _glNormal3sv (const GLshort *v)
{
    glNormal3sv (v) ;
    glCheckError();
}
#undef glNormal3sv 
#define glNormal3sv  _glNormal3sv 

void _glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer)
{
    glNormalPointer (type, stride, pointer) ;
    glCheckError();
}
#undef glNormalPointer 
#define glNormalPointer  _glNormalPointer 

void _glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    glOrtho (left, right, bottom, top, zNear, zFar) ;
    glCheckError();
}
#undef glOrtho 
#define glOrtho  _glOrtho 

void _glPassThrough (GLfloat token)
{
    glPassThrough (token) ;
    glCheckError();
}
#undef glPassThrough 
#define glPassThrough  _glPassThrough 

void _glPixelMapfv (GLenum map, GLsizei mapsize, const GLfloat *values)
{
    glPixelMapfv (map, mapsize, values) ;
    glCheckError();
}
#undef glPixelMapfv 
#define glPixelMapfv  _glPixelMapfv 

void _glPixelMapuiv (GLenum map, GLsizei mapsize, const GLuint *values)
{
    glPixelMapuiv (map, mapsize, values) ;
    glCheckError();
}
#undef glPixelMapuiv 
#define glPixelMapuiv  _glPixelMapuiv 

void _glPixelMapusv (GLenum map, GLsizei mapsize, const GLushort *values)
{
    glPixelMapusv (map, mapsize, values) ;
    glCheckError();
}
#undef glPixelMapusv 
#define glPixelMapusv  _glPixelMapusv 

void _glPixelStoref (GLenum pname, GLfloat param)
{
    glPixelStoref (pname, param) ;
    glCheckError();
}
#undef glPixelStoref 
#define glPixelStoref  _glPixelStoref 

void _glPixelStorei (GLenum pname, GLint param)
{
    glPixelStorei (pname, param) ;
    glCheckError();
}
#undef glPixelStorei 
#define glPixelStorei  _glPixelStorei 

void _glPixelTransferf (GLenum pname, GLfloat param)
{
    glPixelTransferf (pname, param) ;
    glCheckError();
}
#undef glPixelTransferf 
#define glPixelTransferf  _glPixelTransferf 

void _glPixelTransferi (GLenum pname, GLint param)
{
    glPixelTransferi (pname, param) ;
    glCheckError();
}
#undef glPixelTransferi 
#define glPixelTransferi  _glPixelTransferi 

void _glPixelZoom (GLfloat xfactor, GLfloat yfactor)
{
    glPixelZoom (xfactor, yfactor) ;
    glCheckError();
}
#undef glPixelZoom 
#define glPixelZoom  _glPixelZoom 

void _glPointSize (GLfloat size)
{
    glPointSize (size) ;
    glCheckError();
}
#undef glPointSize 
#define glPointSize  _glPointSize 

void _glPolygonMode (GLenum face, GLenum mode)
{
    glPolygonMode (face, mode) ;
    glCheckError();
}
#undef glPolygonMode 
#define glPolygonMode  _glPolygonMode 

void _glPolygonOffset (GLfloat factor, GLfloat units)
{
    glPolygonOffset (factor, units) ;
    glCheckError();
}
#undef glPolygonOffset 
#define glPolygonOffset  _glPolygonOffset 

void _glPolygonStipple (const GLubyte *mask)
{
    glPolygonStipple (mask) ;
    glCheckError();
}
#undef glPolygonStipple 
#define glPolygonStipple  _glPolygonStipple 

void _glPopAttrib (void)
{
    glPopAttrib () ;
    glCheckError();
}
#undef glPopAttrib 
#define glPopAttrib  _glPopAttrib 

void _glPopClientAttrib (void)
{
    glPopClientAttrib () ;
    glCheckError();
}
#undef glPopClientAttrib 
#define glPopClientAttrib  _glPopClientAttrib 

void _glPopMatrix (void)
{
    glPopMatrix () ;
    glCheckError();
}
#undef glPopMatrix 
#define glPopMatrix  _glPopMatrix 

void _glPopName (void)
{
    glPopName () ;
    glCheckError();
}
#undef glPopName 
#define glPopName  _glPopName 

void _glPrioritizeTextures (GLsizei n, const GLuint *textures, const GLclampf *priorities)
{
    glPrioritizeTextures (n, textures, priorities) ;
    glCheckError();
}
#undef glPrioritizeTextures 
#define glPrioritizeTextures  _glPrioritizeTextures 

void _glPushAttrib (GLbitfield mask)
{
    glPushAttrib (mask) ;
    glCheckError();
}
#undef glPushAttrib 
#define glPushAttrib  _glPushAttrib 

void _glPushClientAttrib (GLbitfield mask)
{
    glPushClientAttrib (mask) ;
    glCheckError();
}
#undef glPushClientAttrib 
#define glPushClientAttrib  _glPushClientAttrib 

void _glPushMatrix (void)
{
    glPushMatrix () ;
    glCheckError();
}
#undef glPushMatrix 
#define glPushMatrix  _glPushMatrix 

void _glPushName (GLuint name)
{
    glPushName (name) ;
    glCheckError();
}
#undef glPushName 
#define glPushName  _glPushName 

void _glRasterPos2d (GLdouble x, GLdouble y)
{
    glRasterPos2d (x, y) ;
    glCheckError();
}
#undef glRasterPos2d 
#define glRasterPos2d  _glRasterPos2d 

void _glRasterPos2dv (const GLdouble *v)
{
    glRasterPos2dv (v) ;
    glCheckError();
}
#undef glRasterPos2dv 
#define glRasterPos2dv  _glRasterPos2dv 

void _glRasterPos2f (GLfloat x, GLfloat y)
{
    glRasterPos2f (x, y) ;
    glCheckError();
}
#undef glRasterPos2f 
#define glRasterPos2f  _glRasterPos2f 

void _glRasterPos2fv (const GLfloat *v)
{
    glRasterPos2fv (v) ;
    glCheckError();
}
#undef glRasterPos2fv 
#define glRasterPos2fv  _glRasterPos2fv 

void _glRasterPos2i (GLint x, GLint y)
{
    glRasterPos2i (x, y) ;
    glCheckError();
}
#undef glRasterPos2i 
#define glRasterPos2i  _glRasterPos2i 

void _glRasterPos2iv (const GLint *v)
{
    glRasterPos2iv (v) ;
    glCheckError();
}
#undef glRasterPos2iv 
#define glRasterPos2iv  _glRasterPos2iv 

void _glRasterPos2s (GLshort x, GLshort y)
{
    glRasterPos2s (x, y) ;
    glCheckError();
}
#undef glRasterPos2s 
#define glRasterPos2s  _glRasterPos2s 

void _glRasterPos2sv (const GLshort *v)
{
    glRasterPos2sv (v) ;
    glCheckError();
}
#undef glRasterPos2sv 
#define glRasterPos2sv  _glRasterPos2sv 

void _glRasterPos3d (GLdouble x, GLdouble y, GLdouble z)
{
    glRasterPos3d (x, y, z) ;
    glCheckError();
}
#undef glRasterPos3d 
#define glRasterPos3d  _glRasterPos3d 

void _glRasterPos3dv (const GLdouble *v)
{
    glRasterPos3dv (v) ;
    glCheckError();
}
#undef glRasterPos3dv 
#define glRasterPos3dv  _glRasterPos3dv 

void _glRasterPos3f (GLfloat x, GLfloat y, GLfloat z)
{
    glRasterPos3f (x, y, z) ;
    glCheckError();
}
#undef glRasterPos3f 
#define glRasterPos3f  _glRasterPos3f 

void _glRasterPos3fv (const GLfloat *v)
{
    glRasterPos3fv (v) ;
    glCheckError();
}
#undef glRasterPos3fv 
#define glRasterPos3fv  _glRasterPos3fv 

void _glRasterPos3i (GLint x, GLint y, GLint z)
{
    glRasterPos3i (x, y, z) ;
    glCheckError();
}
#undef glRasterPos3i 
#define glRasterPos3i  _glRasterPos3i 

void _glRasterPos3iv (const GLint *v)
{
    glRasterPos3iv (v) ;
    glCheckError();
}
#undef glRasterPos3iv 
#define glRasterPos3iv  _glRasterPos3iv 

void _glRasterPos3s (GLshort x, GLshort y, GLshort z)
{
    glRasterPos3s (x, y, z) ;
    glCheckError();
}
#undef glRasterPos3s 
#define glRasterPos3s  _glRasterPos3s 

void _glRasterPos3sv (const GLshort *v)
{
    glRasterPos3sv (v) ;
    glCheckError();
}
#undef glRasterPos3sv 
#define glRasterPos3sv  _glRasterPos3sv 

void _glRasterPos4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
    glRasterPos4d (x, y, z, w) ;
    glCheckError();
}
#undef glRasterPos4d 
#define glRasterPos4d  _glRasterPos4d 

void _glRasterPos4dv (const GLdouble *v)
{
    glRasterPos4dv (v) ;
    glCheckError();
}
#undef glRasterPos4dv 
#define glRasterPos4dv  _glRasterPos4dv 

void _glRasterPos4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glRasterPos4f (x, y, z, w) ;
    glCheckError();
}
#undef glRasterPos4f 
#define glRasterPos4f  _glRasterPos4f 

void _glRasterPos4fv (const GLfloat *v)
{
    glRasterPos4fv (v) ;
    glCheckError();
}
#undef glRasterPos4fv 
#define glRasterPos4fv  _glRasterPos4fv 

void _glRasterPos4i (GLint x, GLint y, GLint z, GLint w)
{
    glRasterPos4i (x, y, z, w) ;
    glCheckError();
}
#undef glRasterPos4i 
#define glRasterPos4i  _glRasterPos4i 

void _glRasterPos4iv (const GLint *v)
{
    glRasterPos4iv (v) ;
    glCheckError();
}
#undef glRasterPos4iv 
#define glRasterPos4iv  _glRasterPos4iv 

void _glRasterPos4s (GLshort x, GLshort y, GLshort z, GLshort w)
{
    glRasterPos4s (x, y, z, w) ;
    glCheckError();
}
#undef glRasterPos4s 
#define glRasterPos4s  _glRasterPos4s 

void _glRasterPos4sv (const GLshort *v)
{
    glRasterPos4sv (v) ;
    glCheckError();
}
#undef glRasterPos4sv 
#define glRasterPos4sv  _glRasterPos4sv 

void _glReadBuffer (GLenum mode)
{
    glReadBuffer (mode) ;
    glCheckError();
}
#undef glReadBuffer 
#define glReadBuffer  _glReadBuffer 

void _glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
    glReadPixels (x, y, width, height, format, type, pixels) ;
    glCheckError();
}
#undef glReadPixels 
#define glReadPixels  _glReadPixels 

void _glRectd (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
    glRectd (x1, y1, x2, y2) ;
    glCheckError();
}
#undef glRectd 
#define glRectd  _glRectd 

void _glRectdv (const GLdouble *v1, const GLdouble *v2)
{
    glRectdv (v1, v2) ;
    glCheckError();
}
#undef glRectdv 
#define glRectdv  _glRectdv 

void _glRectf (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    glRectf (x1, y1, x2, y2) ;
    glCheckError();
}
#undef glRectf 
#define glRectf  _glRectf 

void _glRectfv (const GLfloat *v1, const GLfloat *v2)
{
    glRectfv (v1, v2) ;
    glCheckError();
}
#undef glRectfv 
#define glRectfv  _glRectfv 

void _glRecti (GLint x1, GLint y1, GLint x2, GLint y2)
{
    glRecti (x1, y1, x2, y2) ;
    glCheckError();
}
#undef glRecti 
#define glRecti  _glRecti 

void _glRectiv (const GLint *v1, const GLint *v2)
{
    glRectiv (v1, v2) ;
    glCheckError();
}
#undef glRectiv 
#define glRectiv  _glRectiv 

void _glRects (GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
    glRects (x1, y1, x2, y2) ;
    glCheckError();
}
#undef glRects 
#define glRects  _glRects 

void _glRectsv (const GLshort *v1, const GLshort *v2)
{
    glRectsv (v1, v2) ;
    glCheckError();
}
#undef glRectsv 
#define glRectsv  _glRectsv 

void _glRotated (GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    glRotated (angle, x, y, z) ;
    glCheckError();
}
#undef glRotated 
#define glRotated  _glRotated 

void _glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    glRotatef (angle, x, y, z) ;
    glCheckError();
}
#undef glRotatef 
#define glRotatef  _glRotatef 

void _glScaled (GLdouble x, GLdouble y, GLdouble z)
{
    glScaled (x, y, z) ;
    glCheckError();
}
#undef glScaled 
#define glScaled  _glScaled 

void _glScalef (GLfloat x, GLfloat y, GLfloat z)
{
    glScalef (x, y, z) ;
    glCheckError();
}
#undef glScalef 
#define glScalef  _glScalef 

void _glScissor (GLint x, GLint y, GLsizei width, GLsizei height)
{
    glScissor (x, y, width, height) ;
    glCheckError();
}
#undef glScissor 
#define glScissor  _glScissor 

void _glSelectBuffer (GLsizei size, GLuint *buffer)
{
    glSelectBuffer (size, buffer) ;
    glCheckError();
}
#undef glSelectBuffer 
#define glSelectBuffer  _glSelectBuffer 

void _glShadeModel (GLenum mode)
{
    glShadeModel (mode) ;
    glCheckError();
}
#undef glShadeModel 
#define glShadeModel  _glShadeModel 

void _glStencilFunc (GLenum func, GLint ref, GLuint mask)
{
    glStencilFunc (func, ref, mask) ;
    glCheckError();
}
#undef glStencilFunc 
#define glStencilFunc  _glStencilFunc 

void _glStencilMask (GLuint mask)
{
    glStencilMask (mask) ;
    glCheckError();
}
#undef glStencilMask 
#define glStencilMask  _glStencilMask 

void _glStencilOp (GLenum fail, GLenum zfail, GLenum zpass)
{
    glStencilOp (fail, zfail, zpass) ;
    glCheckError();
}
#undef glStencilOp 
#define glStencilOp  _glStencilOp 

void _glTexCoord1d (GLdouble s)
{
    glTexCoord1d (s) ;
    glCheckError();
}
#undef glTexCoord1d 
#define glTexCoord1d  _glTexCoord1d 

void _glTexCoord1dv (const GLdouble *v)
{
    glTexCoord1dv (v) ;
    glCheckError();
}
#undef glTexCoord1dv 
#define glTexCoord1dv  _glTexCoord1dv 

void _glTexCoord1f (GLfloat s)
{
    glTexCoord1f (s) ;
    glCheckError();
}
#undef glTexCoord1f 
#define glTexCoord1f  _glTexCoord1f 

void _glTexCoord1fv (const GLfloat *v)
{
    glTexCoord1fv (v) ;
    glCheckError();
}
#undef glTexCoord1fv 
#define glTexCoord1fv  _glTexCoord1fv 

void _glTexCoord1i (GLint s)
{
    glTexCoord1i (s) ;
    glCheckError();
}
#undef glTexCoord1i 
#define glTexCoord1i  _glTexCoord1i 

void _glTexCoord1iv (const GLint *v)
{
    glTexCoord1iv (v) ;
    glCheckError();
}
#undef glTexCoord1iv 
#define glTexCoord1iv  _glTexCoord1iv 

void _glTexCoord1s (GLshort s)
{
    glTexCoord1s (s) ;
    glCheckError();
}
#undef glTexCoord1s 
#define glTexCoord1s  _glTexCoord1s 

void _glTexCoord1sv (const GLshort *v)
{
    glTexCoord1sv (v) ;
    glCheckError();
}
#undef glTexCoord1sv 
#define glTexCoord1sv  _glTexCoord1sv 

void _glTexCoord2d (GLdouble s, GLdouble t)
{
    glTexCoord2d (s, t) ;
    glCheckError();
}
#undef glTexCoord2d 
#define glTexCoord2d  _glTexCoord2d 

void _glTexCoord2dv (const GLdouble *v)
{
    glTexCoord2dv (v) ;
    glCheckError();
}
#undef glTexCoord2dv 
#define glTexCoord2dv  _glTexCoord2dv 

void _glTexCoord2f (GLfloat s, GLfloat t)
{
    glTexCoord2f (s, t) ;
    glCheckError();
}
#undef glTexCoord2f 
#define glTexCoord2f  _glTexCoord2f 

void _glTexCoord2fv (const GLfloat *v)
{
    glTexCoord2fv (v) ;
    glCheckError();
}
#undef glTexCoord2fv 
#define glTexCoord2fv  _glTexCoord2fv 

void _glTexCoord2i (GLint s, GLint t)
{
    glTexCoord2i (s, t) ;
    glCheckError();
}
#undef glTexCoord2i 
#define glTexCoord2i  _glTexCoord2i 

void _glTexCoord2iv (const GLint *v)
{
    glTexCoord2iv (v) ;
    glCheckError();
}
#undef glTexCoord2iv 
#define glTexCoord2iv  _glTexCoord2iv 

void _glTexCoord2s (GLshort s, GLshort t)
{
    glTexCoord2s (s, t) ;
    glCheckError();
}
#undef glTexCoord2s 
#define glTexCoord2s  _glTexCoord2s 

void _glTexCoord2sv (const GLshort *v)
{
    glTexCoord2sv (v) ;
    glCheckError();
}
#undef glTexCoord2sv 
#define glTexCoord2sv  _glTexCoord2sv 

void _glTexCoord3d (GLdouble s, GLdouble t, GLdouble r)
{
    glTexCoord3d (s, t, r) ;
    glCheckError();
}
#undef glTexCoord3d 
#define glTexCoord3d  _glTexCoord3d 

void _glTexCoord3dv (const GLdouble *v)
{
    glTexCoord3dv (v) ;
    glCheckError();
}
#undef glTexCoord3dv 
#define glTexCoord3dv  _glTexCoord3dv 

void _glTexCoord3f (GLfloat s, GLfloat t, GLfloat r)
{
    glTexCoord3f (s, t, r) ;
    glCheckError();
}
#undef glTexCoord3f 
#define glTexCoord3f  _glTexCoord3f 

void _glTexCoord3fv (const GLfloat *v)
{
    glTexCoord3fv (v) ;
    glCheckError();
}
#undef glTexCoord3fv 
#define glTexCoord3fv  _glTexCoord3fv 

void _glTexCoord3i (GLint s, GLint t, GLint r)
{
    glTexCoord3i (s, t, r) ;
    glCheckError();
}
#undef glTexCoord3i 
#define glTexCoord3i  _glTexCoord3i 

void _glTexCoord3iv (const GLint *v)
{
    glTexCoord3iv (v) ;
    glCheckError();
}
#undef glTexCoord3iv 
#define glTexCoord3iv  _glTexCoord3iv 

void _glTexCoord3s (GLshort s, GLshort t, GLshort r)
{
    glTexCoord3s (s, t, r) ;
    glCheckError();
}
#undef glTexCoord3s 
#define glTexCoord3s  _glTexCoord3s 

void _glTexCoord3sv (const GLshort *v)
{
    glTexCoord3sv (v) ;
    glCheckError();
}
#undef glTexCoord3sv 
#define glTexCoord3sv  _glTexCoord3sv 

void _glTexCoord4d (GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
    glTexCoord4d (s, t, r, q) ;
    glCheckError();
}
#undef glTexCoord4d 
#define glTexCoord4d  _glTexCoord4d 

void _glTexCoord4dv (const GLdouble *v)
{
    glTexCoord4dv (v) ;
    glCheckError();
}
#undef glTexCoord4dv 
#define glTexCoord4dv  _glTexCoord4dv 

void _glTexCoord4f (GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
    glTexCoord4f (s, t, r, q) ;
    glCheckError();
}
#undef glTexCoord4f 
#define glTexCoord4f  _glTexCoord4f 

void _glTexCoord4fv (const GLfloat *v)
{
    glTexCoord4fv (v) ;
    glCheckError();
}
#undef glTexCoord4fv 
#define glTexCoord4fv  _glTexCoord4fv 

void _glTexCoord4i (GLint s, GLint t, GLint r, GLint q)
{
    glTexCoord4i (s, t, r, q) ;
    glCheckError();
}
#undef glTexCoord4i 
#define glTexCoord4i  _glTexCoord4i 

void _glTexCoord4iv (const GLint *v)
{
    glTexCoord4iv (v) ;
    glCheckError();
}
#undef glTexCoord4iv 
#define glTexCoord4iv  _glTexCoord4iv 

void _glTexCoord4s (GLshort s, GLshort t, GLshort r, GLshort q)
{
    glTexCoord4s (s, t, r, q) ;
    glCheckError();
}
#undef glTexCoord4s 
#define glTexCoord4s  _glTexCoord4s 

void _glTexCoord4sv (const GLshort *v)
{
    glTexCoord4sv (v) ;
    glCheckError();
}
#undef glTexCoord4sv 
#define glTexCoord4sv  _glTexCoord4sv 

void _glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    glTexCoordPointer (size, type, stride, pointer) ;
    glCheckError();
}
#undef glTexCoordPointer 
#define glTexCoordPointer  _glTexCoordPointer 

void _glTexEnvf (GLenum target, GLenum pname, GLfloat param)
{
    glTexEnvf (target, pname, param) ;
    glCheckError();
}
#undef glTexEnvf 
#define glTexEnvf  _glTexEnvf 

void _glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params)
{
    glTexEnvfv (target, pname, params) ;
    glCheckError();
}
#undef glTexEnvfv 
#define glTexEnvfv  _glTexEnvfv 

void _glTexEnvi (GLenum target, GLenum pname, GLint param)
{
    glTexEnvi (target, pname, param) ;
    glCheckError();
}
#undef glTexEnvi 
#define glTexEnvi  _glTexEnvi 

void _glTexEnviv (GLenum target, GLenum pname, const GLint *params)
{
    glTexEnviv (target, pname, params) ;
    glCheckError();
}
#undef glTexEnviv 
#define glTexEnviv  _glTexEnviv 

void _glTexGend (GLenum coord, GLenum pname, GLdouble param)
{
    glTexGend (coord, pname, param) ;
    glCheckError();
}
#undef glTexGend 
#define glTexGend  _glTexGend 

void _glTexGendv (GLenum coord, GLenum pname, const GLdouble *params)
{
    glTexGendv (coord, pname, params) ;
    glCheckError();
}
#undef glTexGendv 
#define glTexGendv  _glTexGendv 

void _glTexGenf (GLenum coord, GLenum pname, GLfloat param)
{
    glTexGenf (coord, pname, param) ;
    glCheckError();
}
#undef glTexGenf 
#define glTexGenf  _glTexGenf 

void _glTexGenfv (GLenum coord, GLenum pname, const GLfloat *params)
{
    glTexGenfv (coord, pname, params) ;
    glCheckError();
}
#undef glTexGenfv 
#define glTexGenfv  _glTexGenfv 

void _glTexGeni (GLenum coord, GLenum pname, GLint param)
{
    glTexGeni (coord, pname, param) ;
    glCheckError();
}
#undef glTexGeni 
#define glTexGeni  _glTexGeni 

void _glTexGeniv (GLenum coord, GLenum pname, const GLint *params)
{
    glTexGeniv (coord, pname, params) ;
    glCheckError();
}
#undef glTexGeniv 
#define glTexGeniv  _glTexGeniv 

void _glTexImage1D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    glTexImage1D (target, level, internalformat, width, border, format, type, pixels) ;
    glCheckError();
}
#undef glTexImage1D 
#define glTexImage1D  _glTexImage1D 

void _glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    glTexImage2D (target, level, internalformat, width, height, border, format, type, pixels) ;
    glCheckError();
}
#undef glTexImage2D 
#define glTexImage2D  _glTexImage2D 

void _glTexParameterf (GLenum target, GLenum pname, GLfloat param)
{
    glTexParameterf (target, pname, param) ;
    glCheckError();
}
#undef glTexParameterf 
#define glTexParameterf  _glTexParameterf 

void _glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params)
{
    glTexParameterfv (target, pname, params) ;
    glCheckError();
}
#undef glTexParameterfv 
#define glTexParameterfv  _glTexParameterfv 

void _glTexParameteri (GLenum target, GLenum pname, GLint param)
{
    glTexParameteri (target, pname, param) ;
    glCheckError();
}
#undef glTexParameteri 
#define glTexParameteri  _glTexParameteri 

void _glTexParameteriv (GLenum target, GLenum pname, const GLint *params)
{
    glTexParameteriv (target, pname, params) ;
    glCheckError();
}
#undef glTexParameteriv 
#define glTexParameteriv  _glTexParameteriv 

void _glTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels)
{
    glTexSubImage1D (target, level, xoffset, width, format, type, pixels) ;
    glCheckError();
}
#undef glTexSubImage1D 
#define glTexSubImage1D  _glTexSubImage1D 

void _glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    glTexSubImage2D (target, level, xoffset, yoffset, width, height, format, type, pixels) ;
    glCheckError();
}
#undef glTexSubImage2D 
#define glTexSubImage2D  _glTexSubImage2D 

void _glTranslated (GLdouble x, GLdouble y, GLdouble z)
{
    glTranslated (x, y, z) ;
    glCheckError();
}
#undef glTranslated 
#define glTranslated  _glTranslated 

void _glTranslatef (GLfloat x, GLfloat y, GLfloat z)
{
    glTranslatef (x, y, z) ;
    glCheckError();
}
#undef glTranslatef 
#define glTranslatef  _glTranslatef 

void _glVertex2d (GLdouble x, GLdouble y)
{
    glVertex2d (x, y) ;
    glCheckError();
}
#undef glVertex2d 
#define glVertex2d  _glVertex2d 

void _glVertex2dv (const GLdouble *v)
{
    glVertex2dv (v) ;
    glCheckError();
}
#undef glVertex2dv 
#define glVertex2dv  _glVertex2dv 

void _glVertex2f (GLfloat x, GLfloat y)
{
    glVertex2f (x, y) ;
    glCheckError();
}
#undef glVertex2f 
#define glVertex2f  _glVertex2f 

void _glVertex2fv (const GLfloat *v)
{
    glVertex2fv (v) ;
    glCheckError();
}
#undef glVertex2fv 
#define glVertex2fv  _glVertex2fv 

void _glVertex2i (GLint x, GLint y)
{
    glVertex2i (x, y) ;
    glCheckError();
}
#undef glVertex2i 
#define glVertex2i  _glVertex2i 

void _glVertex2iv (const GLint *v)
{
    glVertex2iv (v) ;
    glCheckError();
}
#undef glVertex2iv 
#define glVertex2iv  _glVertex2iv 

void _glVertex2s (GLshort x, GLshort y)
{
    glVertex2s (x, y) ;
    glCheckError();
}
#undef glVertex2s 
#define glVertex2s  _glVertex2s 

void _glVertex2sv (const GLshort *v)
{
    glVertex2sv (v) ;
    glCheckError();
}
#undef glVertex2sv 
#define glVertex2sv  _glVertex2sv 

void _glVertex3d (GLdouble x, GLdouble y, GLdouble z)
{
    glVertex3d (x, y, z) ;
    glCheckError();
}
#undef glVertex3d 
#define glVertex3d  _glVertex3d 

void _glVertex3dv (const GLdouble *v)
{
    glVertex3dv (v) ;
    glCheckError();
}
#undef glVertex3dv 
#define glVertex3dv  _glVertex3dv 

void _glVertex3f (GLfloat x, GLfloat y, GLfloat z)
{
    glVertex3f (x, y, z) ;
    glCheckError();
}
#undef glVertex3f 
#define glVertex3f  _glVertex3f 

void _glVertex3fv (const GLfloat *v)
{
    glVertex3fv (v) ;
    glCheckError();
}
#undef glVertex3fv 
#define glVertex3fv  _glVertex3fv 

void _glVertex3i (GLint x, GLint y, GLint z)
{
    glVertex3i (x, y, z) ;
    glCheckError();
}
#undef glVertex3i 
#define glVertex3i  _glVertex3i 

void _glVertex3iv (const GLint *v)
{
    glVertex3iv (v) ;
    glCheckError();
}
#undef glVertex3iv 
#define glVertex3iv  _glVertex3iv 

void _glVertex3s (GLshort x, GLshort y, GLshort z)
{
    glVertex3s (x, y, z) ;
    glCheckError();
}
#undef glVertex3s 
#define glVertex3s  _glVertex3s 

void _glVertex3sv (const GLshort *v)
{
    glVertex3sv (v) ;
    glCheckError();
}
#undef glVertex3sv 
#define glVertex3sv  _glVertex3sv 

void _glVertex4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
    glVertex4d (x, y, z, w) ;
    glCheckError();
}
#undef glVertex4d 
#define glVertex4d  _glVertex4d 

void _glVertex4dv (const GLdouble *v)
{
    glVertex4dv (v) ;
    glCheckError();
}
#undef glVertex4dv 
#define glVertex4dv  _glVertex4dv 

void _glVertex4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    glVertex4f (x, y, z, w) ;
    glCheckError();
}
#undef glVertex4f 
#define glVertex4f  _glVertex4f 

void _glVertex4fv (const GLfloat *v)
{
    glVertex4fv (v) ;
    glCheckError();
}
#undef glVertex4fv 
#define glVertex4fv  _glVertex4fv 

void _glVertex4i (GLint x, GLint y, GLint z, GLint w)
{
    glVertex4i (x, y, z, w) ;
    glCheckError();
}
#undef glVertex4i 
#define glVertex4i  _glVertex4i 

void _glVertex4iv (const GLint *v)
{
    glVertex4iv (v) ;
    glCheckError();
}
#undef glVertex4iv 
#define glVertex4iv  _glVertex4iv 

void _glVertex4s (GLshort x, GLshort y, GLshort z, GLshort w)
{
    glVertex4s (x, y, z, w) ;
    glCheckError();
}
#undef glVertex4s 
#define glVertex4s  _glVertex4s 

void _glVertex4sv (const GLshort *v)
{
    glVertex4sv (v) ;
    glCheckError();
}
#undef glVertex4sv 
#define glVertex4sv  _glVertex4sv 

void _glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    glVertexPointer (size, type, stride, pointer) ;
    glCheckError();
}
#undef glVertexPointer 
#define glVertexPointer  _glVertexPointer 

void _glViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
    glViewport (x, y, width, height) ;
    glCheckError();
}
#undef glViewport 
#define glViewport  _glViewport 

#endif
