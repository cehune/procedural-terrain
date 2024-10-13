#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <windows.h>

#include <iostream>
#include "./utils/perlin.h"
#include "./utils/texture.h"
#include "./utils/shader.h"
#include <math.h>
#include <vector> // Make sure to include vector
#include "SFML/Graphics.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
//void renderHeightmap(GLuint heightmapTextureID);

// settings
int Perlin::p[Perlin::GRADIENT_COUNT * 2];
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    Perlin perlin = Perlin();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "heightmap", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // load all openGL function pointers with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader("vertex.vs", "fragment.fs");

    float quadVertices[] = {
        // positions     // texCoords
        -0.3f,  0.3f, 0.0f,  0.0f, 1.0f, // top-left
        -0.3f, -0.3f, 0.0f,  0.0f, 0.0f, // bottom-left
         0.3f, -0.3f, 0.0f,  1.0f, 0.0f, // bottom-right
         0.3f,  0.3f, 0.0f,  1.0f, 1.0f  // top-right
    };

    unsigned int quadIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VBO, VAO, EBO;

    // Generate and bind VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::vector<float> textureData = perlin.generateHeightMap(400, 400, 400);
    unsigned int heightmapID = Texture().generate2DArray(textureData, 400, 400);
    

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();

        // Render the heightmap
        glActiveTexture(GL_TEXTURE0); // Activate the texture unit 0
        glBindTexture(GL_TEXTURE_2D, heightmapID); // Bind your heightmap texture

        // Set the sampler2D uniform to point to texture unit 0 in your fragment shader
        shader.setInt("heightmapTexture", 0);
        // Bind the VAO of your quad
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }

    // Cleanup resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


