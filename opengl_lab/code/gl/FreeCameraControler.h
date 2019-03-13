#ifndef FREECAMERACONTROLER_H
#define FREECAMERACONTROLER_H
#include "gl/oglobject/OGLSceneGraphNode.h"
#include "gl/oglcontext/GLFWContext.h"

class FreeCameraControler: public OGLSceneGraphNode, public GLFWInputEventHandler, public GLFWReshapeEventHandler {
protected:
    glm::vec3 cameraPosition;
    glm::mat4 projectionMatrix;

    float near, far, openAngle, aspect;
    
    float theta,phi, speed;
    float lastTheta, lastPhi, mouseSpeed;
    
    int startX, startY;
    
    int m_width;
    int m_height;
    
    bool rightMouseButton;
    
    void updateModelView();
public:
    FreeCameraControler(glm::vec3 position, float theta = 0, float phi = 0, float moveSpeed = 1.f, float mouseSpeed =  0.01f);
    
    virtual void mouseMoveEvent(double x, double y) override;
    virtual void keyboardEvent(int key, int scancode, int action, int mods) override;
    virtual void mouseButtonEvent(int button, int action, int mods) override;
    
    virtual void framebufferSizeCallback(int width, int height) override;
    
    void resetOrientation(float theta = 0.0f, float phi = 0.0f);
    void resetIntrinsics(float angle = 45.0f, float aspect = 1.0f, float near = 0.1f, float far = 50.0f);
    virtual void render(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;
    
    glm::mat4 getProjectionMat(void);
    glm::mat4 getModelViewMat(void);
    glm::vec3 getCameraPosition(void);
    
    virtual glm::mat4 getModelTransform() override;
    

};

#endif
