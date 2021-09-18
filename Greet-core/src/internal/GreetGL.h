#pragma once

#include <utils/LogUtils.h>
#include <logging/Log.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>

static void GLClearError()
{
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR);
}

static void GLLogError(const char* glCall, const char* file, int line, bool assert)
{
  bool hasError = false;
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR)
  {
    Greet::Log::Error("OpenGL error(0x%s) at %s:%s using %s", Greet::LogUtils::DecToHex(err), file, line, glCall);
    hasError = true;
  }
  if(hasError)
    abort();
}

#if defined _DEBUG
#define GLCall(x) GLClearError(); x; GLLogError(#x,__FILE__,__LINE__, true)
#else
#define GLCall(x) x
#endif
