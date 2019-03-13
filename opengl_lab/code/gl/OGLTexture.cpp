#include "OGLTexture.h"

#include "gl/OGLIncludes.h"
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include "utility/opencv.h"
#include <iostream>

#include "utility/OGLUtils.h"

#include "utility/Variant.h"

OGLTexture::OGLTexture(int width, int height, GLuint internalFormat, GLuint inputFormat, GLuint 
textureDataType):
width(width), height(height), channels(0), gpuonly(true), internalFormat(internalFormat),
textureDataType(textureDataType),inputFormat(inputFormat) {
  setupTexture();
}

void OGLTexture::setupTexture() {
  texture = 0;
  //TODO generate a texture and store its id in the membervaribale texture
  glGenTextures(1, &texture);
  
  //TODO Bind Texture
  glBindTexture(GL_TEXTURE_2D, texture);
  Utils::glError("Preparing texture unit");
  
  // setup a 2d Texture in this id and upload data from cv::Mat img
  // The data from the cv::Mat is available through the ptr() method
  // If this texture is marked gpuonly use a nullpointer for the upload data (we will use this feature in a later exercise)
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 
	                      0, inputFormat, textureDataType, (!gpuonly)?img.ptr():nullptr );
  
  Utils::glError("Creating texture");

  // Make sure you set the Wrapping Modes (both directions) and mini-/magnification  filters for this texture
  // if you forget to set these it is vendor dependant if you see anything (some vendors have defaults in place which let you forget these)
  //Set filters otherwise it is vendor dependant if we see anything
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  Utils::glError("Configurating Texture"); 
  //Leave a clean state (and prevent rouge calls to texture parameter changing methods from touching our texture)
  glBindTexture(GL_TEXTURE_2D, 0); 
}

OGLTexture::OGLTexture(std::string filename):gpuonly(false)
{
  //load the image as cv::Mat img (member variable defined in the header) and check that a 
    //here miss some comments i think......................................
  img = cv::imread(filename, 1);  //loads the image in the BGR format
  if (img.data == nullptr) {
    throw std::runtime_error("Texture "+filename+" not found");
  }
  
  // readup on the pixelspace in OpenCV and OpenGL (you should be able to explain the following line)
  //OpenGL addresses images with flipped coordinates on the y axis 
  cv::flip(img,img,0);
  
  // set member variables for width,height, channels and select the correct internalFormat and inputFormat based on the number of channels
  // your should a least support RGB and RGBA. 
  // Note that OpenCV stores BGR so you have to adapt the format accordingly 
  width = img.cols;
  height = img.rows;
  channels = img.channels();
  
  if (channels == 3) {
    internalFormat = GL_RGB;
    inputFormat = GL_BGR;
  }
  else if (channels == 4) {
    internalFormat = GL_RGBA;
    inputFormat = GL_BGRA;
  }
  else {
    std::cerr << "Incompatible pixelformat" << std::endl;
    throw std::runtime_error("Incompatible pixelformat");
  }
  
  //set the texture data type (unsigned byte is used by OpenCV)
  textureDataType = GL_UNSIGNED_BYTE;
  //here definde the parameter for function glTexImage2D
  width = img.cols;
  height = img.rows;
  channels = img.channels();
  if(channels == 3){
      internalFormat = GL_RGB;
      inputFormat = GL_BGR;
  }
  else if(channels == 4){
      internalFormat = GL_RGBA;
      inputFormat = GL_BGRA;
  }

  //call the OpenGL Part of the setup routine 
  setupTexture();
}



int OGLTexture::getNumberOfChannels() {
  assert(!gpuonly); 
  return channels;
}

  
void OGLTexture::bindTexture() {
  //bind the texture
  glBindTexture(GL_TEXTURE_2D, texture);
}
  
void OGLTexture::unbindTexture() {
  // unbind the texture
  glBindTexture(GL_TEXTURE_2D, 0);
}
  
int OGLTexture::getWidth() {
  return width; 
}

int OGLTexture::getHeight() {
  return height; 
}
  
GLuint OGLTexture::getGLHandle()
{
  assert(texture > 0);
  return texture;
}



void OGLTexture::setWrapSMode(GLint mode) {
  wrap_s_mode = mode;
}
void OGLTexture::setWrapTMode(GLint mode){
  wrap_t_mode = mode;
}

void OGLTexture::ensureWrappingMode() {
  bindTexture();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_mode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_mode);  
  unbindTexture();
}


void OGLTexture::setFilterMode(GLfloat min_mode, GLfloat max_mode) {
  filter_min_mode = min_mode;
  filter_max_mode = max_mode;
}


void OGLTexture::ensureFilterMode() {
  bindTexture();
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min_mode);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max_mode);  
  unbindTexture();    
}
