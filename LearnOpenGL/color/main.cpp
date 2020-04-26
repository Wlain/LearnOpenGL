//
//  main.cpp
//  color
//
//  Created by william on 2020/4/12.
//  Copyright © 2020 william. All rights reserved.
//

#include "fileSystem.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "base.h"

#define CUBE_POSITION_LOCATION 0
#define CUBE_TEXCOORD_LOCATION 1
#define CUBE_NORMAL_LOCATION   2

#define LIGHT_POSITION_LOCATION 0
#define LIGHT_TEXCOORD_LOCATION 1

void framebuffer_size_call(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* windows);
GLuint loadTexture(const char* path);

// setttings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

// mvp matrix
glm::mat4 projectionMatrix = glm::mat4(1.0f);
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::mat4(1.0f);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// lighting
glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);

int main(int argc, const char * argv[]) {
    
    // glfw:initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 设置主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 设置次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //  uncomment this statement to fix compilation on OS X
    
    // glfw: window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "William-Light", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_call);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // glad: load all OpenGL function pointers
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Fail to initialize the GLAD" << std::endl;
        return -1;
    }
    // shader
    Shader lightingShader("shaders/multipleLights.vert", "shaders/multipleLights.frag");
    Shader lampShader("shaders/light.vert", "shaders/light.frag");
    
    float vertices[] = {
        // position        // texCoord  // normal
         0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top right front
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right front
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left front
        -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left front
        
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top right back
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom right back
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom left back
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top left back
        
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top right back
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top right front
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top left front
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top left back
        
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // bottom right front
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom right back
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom left back
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // bottom left front
        
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // top left front
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom left front
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom left back
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // top left back
        
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top right back
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right back
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right front
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f  // top right front
    };
    unsigned short indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    // positions all containers
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
    
    // position of point light
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    
    GLuint cubeVao, lightVao, vbo, ebo;
    // mac 4.0 以后需要绑定vao
    glGenVertexArrays(1, &cubeVao);
    glGenVertexArrays(1, &lightVao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    
    glBindVertexArray(cubeVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(CUBE_POSITION_LOCATION);
    glVertexAttribPointer(CUBE_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(CUBE_TEXCOORD_LOCATION);
    glVertexAttribPointer(CUBE_TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(CUBE_NORMAL_LOCATION);
    glVertexAttribPointer(CUBE_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(LIGHT_POSITION_LOCATION);
    glVertexAttribPointer(LIGHT_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    
    // 打开深度测试
    glEnable(GL_DEPTH_TEST);
    
    GLuint diffuseMap = loadTexture(FileSystem::getPath("resources/textures/container2.png").c_str());
    GLuint specularMap = loadTexture(FileSystem::getPath("resources/textures/container2_specular.png").c_str());
    GLuint emissionMap = loadTexture(FileSystem::getPath("resources/textures/matrix.jpg").c_str());
    lightingShader.use();
    lightingShader.setInt("u_material.diffuse", 0);
    lightingShader.setInt("u_material.specular", 1);
    lightingShader.setInt("u_material.emission", 2);
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
        lightingShader.use();
        lightingShader.setVector3("u_eyePosotion", camera.getCameraPosition());
        lightingShader.setFloat("u_material.shininess", 64.0f);
        
        // directional light
        lightingShader.setVector3("u_dirLight.direction", camera.mFront);
        lightingShader.setVector3("u_dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVector3("u_dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVector3("u_dirLight.specular", 0.4f, 0.5f, 0.5f);
        
        // point light 1
        lightingShader.setVector3("u_pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVector3("u_pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVector3("u_pointLights[0].diffuse", 0.08f, 0.08f, 0.08f);
        lightingShader.setVector3("u_pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("u_pointLights[0].constant", 1.0f);
        lightingShader.setFloat("u_pointLights[0].linear", 0.09f);
        lightingShader.setFloat("u_pointLights[0].quadratic", 0.032f);
        
        // point light 2
        lightingShader.setVector3("u_pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVector3("u_pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVector3("u_pointLights[1].diffuse", 0.08f, 0.08f, 0.08f);
        lightingShader.setVector3("u_pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("u_pointLights[1].constant", 1.0f);
        lightingShader.setFloat("u_pointLights[1].linear", 0.09f);
        lightingShader.setFloat("u_pointLights[1].quadratic", 0.032f);
        
        // point light 3
        lightingShader.setVector3("u_pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVector3("u_pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVector3("u_pointLights[2].diffuse", 0.08f, 0.08f, 0.08f);
        lightingShader.setVector3("u_pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("u_pointLights[2].constant", 1.0f);
        lightingShader.setFloat("u_pointLights[2].linear", 0.09f);
        lightingShader.setFloat("u_pointLights[2].quadratic", 0.032f);
        
        // point light 4
        lightingShader.setVector3("u_pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVector3("u_pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVector3("u_pointLights[3].diffuse", 0.08f, 0.08f, 0.08f);
        lightingShader.setVector3("u_pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("u_pointLights[3].constant", 1.0f);
        lightingShader.setFloat("u_pointLights[3].linear", 0.09f);
        lightingShader.setFloat("u_pointLights[3].quadratic", 0.032f);
        
        // spotLight
        lightingShader.setVector3("u_spotlight.position", camera.mPosition);
        lightingShader.setVector3("u_spotlight.direction", camera.mFront);
        lightingShader.setVector3("u_spotlight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVector3("u_spotlight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVector3("u_spotlight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("u_spotlight.constant", 1.0f);
        lightingShader.setFloat("u_spotlight", 0.09f);
        lightingShader.setFloat("u_spotlight.quadratic", 0.032f);
        lightingShader.setFloat("u_spotlight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("u_spotlight.outCutOff", glm::cos(glm::radians(15.0f)));

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap);
        
        // camera transformation
        viewMatrix = camera.getViewMatrix();
        lightingShader.setMatrix4("u_viewMatrix", viewMatrix);
        projectionMatrix = glm::perspective(glm::radians(camera.mZoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMatrix4("u_projectionMatrix", projectionMatrix);
        glBindVertexArray(cubeVao);
        for (GLuint i = 0; i < 10; ++i) {
            modelMatrix = glm::mat4(1.0);
            modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            float angle = 20.0f * i;
            modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMatrix4("u_modelMatrix", modelMatrix);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
        }
        // draw the lamp object
        lampShader.use();
        lampShader.setMatrix4("u_projectionMatrix", projectionMatrix);
        lampShader.setMatrix4("u_viewMatrix", viewMatrix);
        
        glBindVertexArray(lightVao);
        for (GLuint i = 0; i < 4; ++i) {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, pointLightPositions[i]);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
            lampShader.setMatrix4("u_modelMatrix", modelMatrix);
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
    glDeleteVertexArrays(1, &cubeVao);
    glDeleteVertexArrays(1, &lightVao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    // glfw:terminate
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
        glfwSetWindowShouldClose(windows, true);
    if (glfwGetKey(windows, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(windows, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(windows, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(windows, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}


GLuint loadTexture(const char* path)
{
    // load and create a texture0
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    GLubyte *data = stbi_load(path, &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(true);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    return textureID;
}
