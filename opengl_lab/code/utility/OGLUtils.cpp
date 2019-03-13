#include "gl/OGLIncludes.h"
#include "utility/OGLUtils.h"

//////////////////////////////////////////////////////////////////////////////
//           General Utils                                                  //
//////////////////////////////////////////////////////////////////////////////

bool Utils::glError(std::string msg, bool haltOnError) {
  // check OpenGL error
  GLenum err;
  bool err_occured = false;
  while ((err = glGetError()) != GL_NO_ERROR) {
    err_occured = true;
    if (msg == "") {
      std::cerr << "OpenGL error : " << err << std::endl;
    }
    else {
      std::cerr << "OpenGL error ( " << msg << " ) : " << err << "  " << gluErrorString(err)<<  std::endl;
    }
    if (haltOnError) throw std::runtime_error("An OpenGL Error Occured");
  }
  return err_occured;
}

//////////////////////////////////////////////////////////////////////////////
//           SimpleColoredTriangle                                          //
//////////////////////////////////////////////////////////////////////////////
 
Utils::SimpleColoredTriangle::SimpleColoredTriangle(OGLShader* shader):shader(shader)
{
  static GLfloat verticies[] = {  -0.5,-0.5,0.0,
                                  0.5,-0.5,0.0,
			          0.0,0.5,0.0};
  static GLint faces[] = {0,1,2};
  
  static GLfloat colors[] =    {  1.0,0.0,0.0,
                                  0.0,1.0,0.0,
			          0.0,0.0,1.0};
				  
  GLfloat vdata [6*3];
  std::copy(verticies,verticies+ 3*3, vdata);
  std::copy(colors,colors+ 3*3, vdata+ 3*3);
  
  
  
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);
  
  glGenBuffers(1,&vbo);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  
  glBufferData(GL_ARRAY_BUFFER,6*3*sizeof(GLfloat), vdata, GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0, (void*) 0);
  glEnableVertexAttribArray(0);
  
  glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,0, (void*) (3* 3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  glGenBuffers(1,&ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3* sizeof(GLint), faces, GL_STATIC_DRAW);
  
  // unbind buffers //
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Utils::SimpleColoredTriangle::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
    shader->bindShader();
    glBindVertexArray(vao);
    
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT, (void*) 0);
    glBindVertexArray(0);
    shader->unbindShader();
}


void Utils::SimpleColoredTriangle::update()
{
  //do nothing!
}

//////////////////////////////////////////////////////////////////////////////
//           SimpleColorShader                                              //
//////////////////////////////////////////////////////////////////////////////


Utils::SimpleColorShader::SimpleColorShader(std::string shaderName):OGLShader(shaderName)
{
  bool retVal = loadVertexShaderFromCode("#version 330\n\
                            layout(location = 0) in vec3 vertex;\n\
                            layout(location = 1) in vec3 vcolor;\n\
                            out vec3 vertexColor;\n\
                            out vec4 color;\n\
                            \n\
                            void main() {\n\
                              vertexColor = vcolor;\n\
                              gl_Position = vec4(vertex, 1.0);\n\
                            }\n\
                            ");
  assert(retVal);
  
  
  retVal = loadFragmentShaderFromCode("#version 330\n\
                              in vec3 vertexColor;\n\
                              out vec4 color;\n\
                              void main() {\n\
                                color = vec4(vertexColor, 1.0);  \n\
                              }\n\
                              ");
  assert(retVal);
  
  
  linkShader();
  
  // set address of fragment color output //
  glBindFragDataLocation(shaderProgram, 0, "color");
}
