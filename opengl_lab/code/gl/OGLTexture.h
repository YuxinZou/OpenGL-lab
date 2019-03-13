#ifndef OGLTEXTURE_H
#define OGLTEXTURE_H


#include <GL/glew.h>
#include "utility/opencv_core.h"


class OGLTexture {
protected:
  cv::Mat img; //
  
  int width;
  int height;
  int channels;  //the channel number of the image
  
  bool gpuonly;
  bool viaCV;
  
  GLuint texture;
  
  GLint internalFormat;
  GLint inputFormat;
  GLint textureDataType;
  
  void setupTexture();
  
  unsigned char* data;
  
  GLint wrap_s_mode = GL_REPEAT;
  GLint wrap_t_mode = GL_REPEAT;
  
  GLfloat filter_min_mode = GL_NEAREST;
  GLfloat filter_max_mode = GL_NEAREST;
  
public:
  //Just create a texture; no cpu side data (used later)
  OGLTexture (int width, int height, GLuint internalFormat, GLuint inputFormat, GLuint 
textureDataType);

  OGLTexture(std::string filename);
  
  
  void bindTexture();
  void unbindTexture();
  
  int getWidth();
  int getHeight();
  int getNumberOfChannels();
  
  void uploadToGPU();
  void removeFromGPU();
  GLuint getGLHandle();
  
  void setWrapSMode(GLint mode = GL_REPEAT);
  void setWrapTMode(GLint mode = GL_REPEAT);
  void ensureWrappingMode();
  void setFilterMode(GLfloat min_mode = GL_NEAREST, GLfloat max_mode = GL_NEAREST);
  void ensureFilterMode();
  
};

#endif // TEXTURE_H
