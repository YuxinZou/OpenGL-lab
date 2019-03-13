#include "gl/OGLIncludes.h"


#include "gl/oglcontext/GLFWContext.h"
#include "gl/OGLContext.h"
#include "gl/oglobject/OGLMeshObject.h"
#include "gl/oglobject/OGLColoredMeshObject.h"
#include "gl/oglobject/OGLSGRotationalAnimator.h"
#include "gl/oglobject/OGLWireBox.h"
#include "gl/oglobject/OGLRelativeTransform.h"
#include "utility/OGLUtils.h"
#include "utility/OBJWavefrontFile.h"
#include "gl/oglshader/SimpleFileShader.h"

#include "gl/FreeCameraControler.h"
#include "gl/OGLViewport.h"
#include "gl/ViewportFreeCameraControler.h"
#include <iostream>

#include "utility/Variant.h"

class ActivateClippingNode: public OGLSceneGraphNode {

public:
    ActivateClippingNode(OGLSceneGraphNode* parent = nullptr, OGLObject* child = nullptr): 
    OGLSceneGraphNode(glm::mat4(1.f),parent,child){}
    
virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override{
  glEnable(GL_CLIP_DISTANCE0);
  OGLSceneGraphNode::render(projection, view, model);
  glDisable(GL_CLIP_DISTANCE0);
}  
}; 

class ClippedOGLColoredMeshObject : public OGLColoredMeshObject {
  protected: 
  OGLSceneGraphNode* frustumTransformReference;
  GLint frustumTransformPosition;
  public: 
    ClippedOGLColoredMeshObject(OGLShader* shader, 
				const std::vector< Vertex > vertices,
				const std::vector< Color > colors, 
				const std::vector< Face > faces,  const int verticesPerFace = 3): 
				  OGLColoredMeshObject(shader,vertices,colors,faces,verticesPerFace), 
                                  frustumTransformReference(nullptr){
				    getShaderUniforms(); //This will get parent classes' uniforms multiple times (its still fast but not nice)
				  }


    virtual void uploadUniforms(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override {
      OGLColoredMeshObject::uploadUniforms(projection, view, model);
      glUniformMatrix4fv(frustumTransformPosition, 1, GL_FALSE, glm::value_ptr(projection*frustumTransformReference->getTransform()));
    }
    
    void setFrustumTransformReference(OGLSceneGraphNode* frustumTransformReference) {
      this->frustumTransformReference= frustumTransformReference ; 
    }
    
    virtual void getShaderUniforms() override {
      OGLColoredMeshObject::getShaderUniforms();
      frustumTransformPosition = glGetUniformLocation(shader->getId(), "frustumTransform");
    }
};

class ApplyProjectionMatrixFromCamera: public OGLSceneGraphNode {
protected:
  FreeCameraControler* cam;
  bool inverted;
public:
ApplyProjectionMatrixFromCamera(OGLSceneGraphNode* parent, FreeCameraControler* cam, bool invert):OGLSceneGraphNode(glm::mat4(1.f),parent),cam(cam),inverted(invert) {}
virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override {
  glm::mat4 localTransform;
  if (inverted) {
    localTransform = model*glm::inverse(cam->getProjectionMat());
  }
  else {
    localTransform = model*(cam->getProjectionMat());
  }
  for (OGLObject* object: childNodes) {
    object->render(projection, view, localTransform);
  }
}

glm::mat4 getTransform() override {
  if (parent == nullptr) {
    return glm::inverse(cam->getProjectionMat());
  }
  else {
    glm::mat4 parentT = parent->getTransform();
    return parentT*glm::inverse(cam->getProjectionMat());
  }
}


};

OGLColoredMeshObject drawGrid( int count_X, int count_Z,float spacingx, float spacingz){

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<std::vector<int>> lines;

    GLfloat widthX_half = (count_X - 1) * spacingx / 2;
    GLfloat widthZ_half = (count_Z - 1) * spacingz / 2;

    glm::vec3 real_left_bottom = glm::vec3(-widthX_half , 0.f , widthZ_half);

    for(GLint num = 0; num < count_Z; num++){
        glm::vec3 temp_Z = real_left_bottom + glm::vec3(0.f,0.f,-1.f) * num * spacingz;
        vertices.push_back(temp_Z);
        vertices.push_back(temp_Z + glm::vec3(2.f,0.f,0.f) * widthX_half);
        colors.push_back(glm::vec3(1.f,1.f,1.f));
        colors.push_back(glm::vec3(1.f,1.f,1.f));
        lines.push_back(std::vector<int>({2 * num,2 * num + 1}));

    }
    for(GLint num = 0; num < count_X; num++){
        glm::vec3 temp_X = real_left_bottom + glm::vec3(1.f,0.f,0.f) * num * spacingx;
        vertices.push_back(temp_X);
        vertices.push_back(temp_X + glm::vec3(0.f,0.f,-2.f) * widthZ_half);
        colors.push_back(glm::vec3(1.f,1.f,1.f));
        colors.push_back(glm::vec3(1.f,1.f,1.f));
        lines.push_back(std::vector<int>({2 * (num + count_Z ),2 * (num + count_Z ) + 1}));
    }

    OGLColoredMeshObject grid(
                new SimpleFileShader("ColorShaderWireBox",SHADERPATH"modelView2.vert",SHADERPATH"verySimple.frag"),vertices,colors,lines,2);

    return grid;
}
int main(int argc, char **argv) {
    //1. Create an OpenGL context
    GLFWContext  context(argc, argv,1200,400,"Echtzeitcomputergraphik - Aufgabe 5 a");
    
    glEnable(GL_DEPTH_TEST);
    
    
    //2. Create a very simple shader 
    SimpleFileShader shader("FileShader",SHADERPATH"modelView2.vert",SHADERPATH"verySimple.frag");
    //3. Load the models
    //3.1 Load monkey
    Utils::OBJWavefrontFile objfile_monkey(MODELPATH"monkey.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen_monkey(objfile_monkey);
    ClippedOGLColoredMeshObject monkey (&shader, meshgen_monkey.vertices, meshgen_monkey.normals, meshgen_monkey.faces);
    //3.2  Load armadillo
    Utils::OBJWavefrontFile objfile_armadillo(MODELPATH"armadillo.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen_armadillo(objfile_armadillo);
    ClippedOGLColoredMeshObject armadillo (&shader, meshgen_armadillo.vertices, meshgen_armadillo.normals, meshgen_armadillo.faces);
    //3.3  Load bunny
    Utils::OBJWavefrontFile objfile_bunny(MODELPATH"bunny.obj");
    Utils::OBJWavefrontMeshDataGenerator meshgen_bunny(objfile_bunny);
    ClippedOGLColoredMeshObject bunny (&shader, meshgen_bunny.vertices, meshgen_bunny.normals, meshgen_bunny.faces);
    
    
    // create 3 Viewport and add them as objects to the context
    OGLViewport defaultView(0,0,400,400);
    context.addObject(&defaultView);
    OGLViewport cameraSpaceView(400,0,400,400/*,false*/);
    context.addObject(&cameraSpaceView);
    OGLViewport normalizedDeviceSpaceView(800,0,400,400/*,false*/);
    context.addObject(&normalizedDeviceSpaceView);
    
    // setup 3 Camera controlers
    // remember to 
    //  -add the controler to the context as input handler 
    //  -add the cameraControler as a child to the corresponding view 
    ViewportFreeCameraControler camControler0 (glm::vec3(-2.f,4.f,-2.f),defaultView,M_PI+M_PI/4,M_PI/4,0.1f,0.01f);
    camControler0.resetIntrinsics(45.0f, 1.0f, 0.5f, 5.0f);
    context.addInputEventHandler(&camControler0);
    defaultView.addChild(&camControler0);
    
    ViewportFreeCameraControler camControler1 (glm::vec3(-2.f,4.f,-2.f),cameraSpaceView,M_PI+M_PI/4,M_PI/4,0.1f,0.01f);
    context.addInputEventHandler(&camControler1);
    cameraSpaceView.addChild(&camControler1);
    
    ViewportFreeCameraControler camControler2 (glm::vec3(0.f,1.f,3.f),normalizedDeviceSpaceView,0,M_PI/8,0.1f,0.01f);
    context.addInputEventHandler(&camControler2);
    normalizedDeviceSpaceView.addChild(&camControler2);
    
    
    //Helper-Node for the scene (register the scene under viewport0's camera controler)
    OGLSceneGraphNode scene(glm::mat4(1.f),&camControler0);
    //Build a scene 
    glm::mat4 transform  = glm::translate(glm::vec3(0.f,0.f,0.f));
    OGLSceneGraphNode monkey_node(transform,&scene,&monkey);
    
    transform  = glm::rotate((float)(M_PI/2),glm::vec3(0.f,1.f,0.f))*glm::translate(glm::vec3(0.f,0.f,-2.f));
    OGLSceneGraphNode bunny1(transform,&scene,&bunny);
    
    transform  = glm::rotate((float)(-M_PI/2),glm::vec3(0.f,1.f,0.f))*glm::translate(glm::vec3(0.f,0.f,-2.f));
    OGLSceneGraphNode bunny2(transform,&scene,&bunny);

    transform  = glm::rotate((float)(M_PI),glm::vec3(0.f,1.f,0.f))*glm::translate(glm::vec3(0.f,0.f,2.f));
    OGLSceneGraphNode armadillo1(transform,&scene,&armadillo);
    
    transform  = glm::translate(glm::vec3(0.f,0.f,2.f));
    OGLSceneGraphNode armadillo2(transform,&scene,&armadillo);
    
    // Create a wirebox (we will be using this to visualize the frustum)
    OGLWireBox wirebox;

    //OGLColoredMeshObject grid(
    //            new SimpleFileShader("ColorShaderWireBox",SHADERPATH"modelView2.vert",SHADERPATH"verySimple.frag"),vertices,colors,lines,2);
    OGLColoredMeshObject grid = drawGrid(5,3,0.5,1);
    
    //create the content for viewport 2  
    // - visualize the camera frustum of camControler0 with the wirebox
    //   - to do so review the transformations that transform the scene into device space
    // Note: for your convenience Nodes with the following functionalities are available:
    //  - OGLRelativeTransform is a SceneGraph-Node that takes another scenegraph node and applies its transformation as local transformation. 
    //    It also allows to apply the inverse transformation. (see gl/oglobject/OGLRelativeTransform.h)
    //  - ApplyProjectionMatrixFromCamera is a SceneGraph-Node which takes a reference cameraControler and applies its projection matrix as local transformation. 
    //    Again, this node also allows the application of the inverse. 
    //
    // Create node with the correct transformations and add the wirebox as a child to render the frustum into the scene
    
    OGLRelativeTransform showCamControlerMovement(&camControler0, &camControler1, true);
    ApplyProjectionMatrixFromCamera frustumDeform(&showCamControlerMovement,&camControler0,true/*inverted*/);
    
    frustumDeform.addChild(&wirebox);
    camControler1.addChild(&showCamControlerMovement);
    

    //the scene node can be readded as a child to be rendered again  by this viewport 
    camControler1.addChild(&scene);
    
    
    //create the content for viewport 3
    // - visualize the scene inside the normalized device space
    //  In this case the wirebox will remain untouched by any transformation other than the camera controler for this viewport
    // As we use the wirebox as a reference for clipping it has to be set for all parts of the scene (note that for this to be possible a specialized MeshObject was used)
    // 
    // After you figured out the right transformations make sure you wrap the scene node in an ActivateClippingNode to allow the shader to clip geometry outside the 
    // frustum. 
    
    OGLSceneGraphNode invertZ(glm::scale(glm::vec3(1.f,1.f,-1.f)),&camControler2); 
    ApplyProjectionMatrixFromCamera sceneDeform(&invertZ,&camControler0,false); 
    OGLRelativeTransform moeToCamSpace(&camControler0,&sceneDeform,false);  
    ActivateClippingNode clippingNode(&moeToCamSpace,&scene);
    
    
    //Add the wirebox without further transformation
    camControler0.addChild(&grid);
    camControler1.addChild(&grid);
    camControler2.addChild(&wirebox);
    camControler2.addChild(&grid);
    //set the transform for clipping volume
    monkey.setFrustumTransformReference(&camControler2);
    armadillo.setFrustumTransformReference(&camControler2);
    bunny.setFrustumTransformReference(&camControler2);
    
    
    //6. Handoff to the glut renderloop
    context.renderLoop();
    
    //7. Cleanup 
    return EXIT_SUCCESS;

}
