//
//  shader.hpp
//  LearnOpenGL
//
//  Created by william on 2020/4/9.
//  Copyright © 2020 william. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include "base.h"

class Shader
{
public:
    Shader() = default;
    ~Shader() = default;
    Shader(const Shader& s) = default;
    Shader(Shader&& a) = default;
    Shader& operator=(const Shader& s) = default;
    Shader& operator=(Shader&& s) = default;
    GLuint getProgram() const;
    Shader(const char* vertexPath, const char* fragmentPath);
    GLuint CreateProgram(GLuint vertShader, GLuint fragShader);
    GLuint CompileShader(GLenum shaderType, const GLchar *src);
    void Use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVector2(const std::string& name, const glm::vec2& value) const;
    void setVector2(const std::string& name, float x, float y) const;
    void setVector3(const std::string& name, const glm::vec3& value) const;
    void setVector3(const std::string& name, float x, float y, float z) const;
    void setVector4(const std::string& name, const glm::vec4& value) const;
    void setVector4(const std::string& name, float x, float y, float z, float w) const;
    void setMatrix2(const std::string& name, const glm::mat2& mat) const;
    void setMatrix3(const std::string& name, const glm::mat3& mat) const;
    void setMatrix4(const std::string& name, const glm::mat4& mat) const;
    
private:
    GLuint program;
};


#endif /* shader_hpp */
