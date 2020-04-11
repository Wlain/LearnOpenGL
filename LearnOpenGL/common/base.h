//
//  base.h
//  LearnOpenGL
//
//  Created by william on 2020/4/9.
//  Copyright © 2020 william. All rights reserved.
//

#ifndef base_h
#define base_h

#include "glm.hpp"
#include "ext.hpp"
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdarg>
#include <math.h>
#include <cstring>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <cassert>
#include <sys/time.h>

    // 安全释放文件
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { if (x) delete (x); (x) = NULL; }    //定义安全释放函数
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) delete [] (x); (x) = NULL; }    //定义安全释放函数
#endif
#ifndef SAFE_FREE
#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}
#endif

#define MATH_TOLERANCE              2e-37f
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_PI                     3.14159265



#define GLERROR_CHECK() \
{ \
GLenum glError = glGetError(); \
if (glError != GL_NO_ERROR) {\
printf("glGetError() = %i (0x%.8x) in filename = %s, line  = %i\n", glError, glError, __FILE__ , __LINE__); \
} \
}

#endif /* base_h */
