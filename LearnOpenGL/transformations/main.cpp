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
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

// mvp matrix
glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);
// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
         0.5f,  0.5f, 0.5f, 1.0f, 1.0f, // top right front
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom right front
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom left front
        -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, // top left front
        
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top right back
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom right back
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom left back
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // top left back
        
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top right back
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // top right front
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, // top left front
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // top left back
        
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // bottom right front
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // bottom right back
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom left back
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom left front
        
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // top left front
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // bottom left front
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom left back
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // top left back
        
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top right back
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom right back
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // bottom right front
        0.5f,  0.5f,  0.5f, 0.0f, 1.0f  // top right front
    };
    unsigned short indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
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
    
    data = stbi_load(FileSystem::getPath("resources/textures/logo.jpg").c_str(), &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // 打开深度测试
    glEnable(GL_DEPTH_TEST);
    
    // 告诉opengl需要使用哪个纹理单元
    shader.use();
    shader.setInt("u_texture0", 0);
    shader.setInt("u_texture1", 1);
    glViewport(0, 0, width, height);
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SRC_WIDTH/(float)SRC_HEIGHT, 0.1f, 100.0f);
    shader.setMatrix4("u_projectionMatrix", projectionMatrix);
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        processInput(window);
        // render
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        shader.setFloat("u_mixValue", mixValue);
        // camera transformation
        viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMatrix4("u_viewMatrix", viewMatrix);
        
        glBindVertexArray(vao);
        for (unsigned int i = 0; i < 10; ++i)
        {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            float angle = 20.0f * i;
            modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMatrix4("u_modelMatrix", modelMatrix);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
        }
        // 打开线框模式(Wireframe Mode)
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(0);
        glUseProgram(0);
        // glfw:swap buffer and poll IO events(keys pressed/released, mouse move etc.)
        glfwSwapBuffers(window);
        // 检查有没有触发什么事件
        glfwPollEvents();
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
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(windows, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(windows, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(windows, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(windows, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
