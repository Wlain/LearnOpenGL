//
//  main.cpp
//  triangle_vbo
//
//  Created by william on 2020/4/9.
//  Copyright © 2020 william. All rights reserved.
//

#include "base.h"
#include "shader.hpp"

void framebuffer_size_call(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *windows);

// setttings
const unsigned int SRC_WIDTH = 800;
const unsigned int SRC_HEIGHT = 600;
GLint programLocation = -1;

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
    Shader shader("shaders/triangle.vert", "shaders/triangle.frag");
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f, // right
        0.0f,  0.5f, 0.0f  // top
    };
    GLuint vbo, vao;
    // mac 4.0 以后需要绑定vao
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    programLocation = glGetAttribLocation(shader.getProgram(), "a_position");
    glEnableVertexAttribArray(programLocation);
    glVertexAttribPointer(programLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glUseProgram(shader.getProgram());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(vao);
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLERROR_CHECK();
        glUseProgram(shader.getProgram());
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
}
