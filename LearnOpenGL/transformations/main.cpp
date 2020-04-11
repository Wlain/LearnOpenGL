//
//  main.cpp
//  transformations
//
//  Created by william on 2020/4/11.
//  Copyright © 2020 william. All rights reserved.
//

#include "fileSystem.hpp"
#include "shader.hpp"
#include "base.h"

void framebuffer_size_call(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *windows);

// setttings
const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;

enum AttribLocation{
    Positions,
    TexCoords,
    NumAttribs
};

enum UniformLocation{
    NumUniforms
};

GLint uniforms[NumUniforms];
GLint attribs[NumAttribs];

// stores the alpha of texture
float mixValue = 0.2f;

int main(int argc, const char * argv[]) {
    
    // glfw:initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 设置主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 设置次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //  uncomment this statement to fix compilation on OS X
    
    // glfw: window creation
    GLFWwindow* window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "William-CreateWindow", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_call);
    
    // glad: load all OpenGL function pointers
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Fail to initialize the GLAD" << std::endl;
        return -1;
    }
    Shader shader("shaders/transform.vert", "shaders/transform.frag");
    
    float vertices[] = {
        // position         // texCoords
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,// top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f // top left
        
    };
    unsigned short indices[] = {
        1, 2, 0, 3
    };
    GLuint vbo, vao, ebo;
    attribs[Positions] = glGetAttribLocation(shader.getProgram(), "a_position");
    attribs[TexCoords] = glGetAttribLocation(shader.getProgram(), "a_texCoord");
    // mac 4.0 以后需要绑定vao
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attribs[Positions]);
    glVertexAttribPointer(attribs[Positions], 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attribs[TexCoords]);
    glVertexAttribPointer(attribs[TexCoords], 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // load and create a texture0
    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    GLubyte *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    // load and create a texture1
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // 告诉opengl需要使用哪个纹理单元
    shader.use();
    shader.setInt("u_texture0", 0);
    shader.setInt("u_texture1", 1);
    
        // render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        shader.setFloat("u_mixValue", mixValue);
        glm::mat4 mvpMatrix = glm::mat4(1.0f);
        mvpMatrix = glm::translate(mvpMatrix, glm::vec3(0.5f, -0.5f, 0.0f));
        mvpMatrix = glm::rotate(mvpMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setMatrix4("u_mvpMatrix", mvpMatrix);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
        mvpMatrix = glm::mat4(1.0f);
        mvpMatrix = glm::translate(mvpMatrix, glm::vec3(-0.5f, 0.5f, 0.0f));
        float sacleAmount = sin(glfwGetTime());
        mvpMatrix = glm::scale(mvpMatrix, glm::vec3(sacleAmount, sacleAmount, sacleAmount));
        shader.setMatrix4("u_mvpMatrix", mvpMatrix);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
        // input
        processInput(window);
        // glfw:swap buffer and poll IO events(keys pressed/released, mouse move etc.)
        glfwSwapBuffers(window);
        glfwPollEvents(); // 检查有没有触发什么事件
    }
    
    // glfw:terminate,
    glfwTerminate();
    return 0;
}

void framebuffer_size_call(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *windows)
{
    if (glfwGetKey(windows, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(windows, true);
    }
    
    if (glfwGetKey(windows, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.01f;
        mixValue = fmin(1.0, mixValue);
    }
    
    if (glfwGetKey(windows, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.01f;
        mixValue = fmax(0.0, mixValue);
    }
}
