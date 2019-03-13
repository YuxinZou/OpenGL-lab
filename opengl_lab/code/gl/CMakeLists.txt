add_library(cg2gl STATIC oglcontext/GLFWContext.cpp 
                         OGLContext.cpp 
			 
                         FreeCameraControler.cpp
                         
                         ViewportFreeCameraControler.cpp
                         
                         OGLShader.cpp
                         
                         OGLLightSource.cpp
                         
                         OGLViewport.cpp
                         
                         oglshader/SimpleFileShader.cpp 
                         
                         oglobject/OGLMeshObject.cpp
                         
                         oglobject/OGLColoredMeshObject.cpp 
                         
                         oglobject/OGLNormalMeshObject.cpp 
                         
                         oglobject/OGLSceneGraphNode.cpp
                         
                         oglobject/OGLSGRotationalAnimator.cpp
                         
                         oglobject/OGLRelativeTransform.cpp
                         oglobject/OGLWireBox.cpp
                         
                         oglobject/OGLGrid.cpp
                         
                         oglobject/OGLTexturedMeshObject.cpp
                         
                         
                         oglobject/OGLTexturedMeshObject2.cpp
                         
                         OGLTexture.cpp
                         
                         
                         OGLFramebufferRenderPass.cpp
                         )
