#ifndef GLM_INCLUDES_H
#define GLM_INCLUDES_H

#define GLM_ENABLE_EXPERIMENTAL

#ifdef MODERN_GLM
#define GLM_FORCE_SWIZZLE 
#else
#define GLM_SWIZZLE 
#endif

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtx/transform.hpp"
#include <glm/gtc/type_ptr.hpp>
          
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"


#endif