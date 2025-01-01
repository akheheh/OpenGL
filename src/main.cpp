#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLEnv/GLEnv.h>
#include <Shader/shader.h>
#include <STB/stb_image.h>
/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/

struct VIEW
{
    int WIDTH;
    int HEIGHT;
    float *clearColor;
};

float clearColorArray[] = {0.0f, 0.0f, 0.0f, 1.0f};
auto GLView = VIEW{1920, 1080, clearColorArray};

auto [WIDTH, HEIGHT, CLEAR_COLOR] = GLView;

int main()
{
    GLFWwindow *window = createOpenGLEnv(1920, 1080, "GL");

    // Make a shader program
    Shader a_Shader("vertex_shader.glsl", "fragment_shader.glsl");

    // Texture coordinates are the subspace [0, 1] x [0, 1]
    // Therefore, given any texture image, the bottom left is at (0, 0) and the top right is at (1, 1)
    // Since we want to map this texture to a triangle, we only need to give it 3 texture coordinates
    // The coordinates refer to the TEXTURE, not the triangle, so OpenGL interpolates the texture

    // Texture sampling can be done in many ways, we tell OpenGL how to sample the textures
    // Load a texture with the stbi library
    int width, height, nrChannels;
    // stbi_load(image source, width, height, number of color channels)
    unsigned char *data = stbi_load("./images/container.jpg", &width, &height, &nrChannels, 0);
    // Create a GL texture object
    GLuint texture;
    // glGenTextures(how many texture objects, array to store the texture object ID references)
    glGenTextures(1, &texture);
    // Bind the texture object as the currently active texture object
    glBindTexture(GL_TEXTURE_2D, texture);
    // Generate a texture: load texture data and create mipmaps

    /*
    glTexImage2D(
    texture target,
    level of detail from the mipmap structure: 0 is the base image level, n is the nth reduction aka 0 is highest LOD, n is lowest LOD,
    Format to store the texture
    width of texture,
    height of texture,
    border: always 0,
    format of source image,
    data type of source image,
    the actual image data
    )

    */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap generates all mipmaps for the currently active texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Cleanup
    stbi_image_free(data);

    float vertices[] = {
        // Positions         //Colors            //Texture coordinates
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f}; // top left

    GLuint indexBuffer[] = {
        0, 1, 3,
        1, 2, 3};

    // Create and bind a VAO, create, bind and load a buffer, set up attribute configurations
    GLuint VAO,
        VBO, EBO;
    GLuint colorBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &colorBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Call it once, resize callback will call it again with resize dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        GLGeneralSetup(window, CLEAR_COLOR);

        // Draw the triangle
        a_Shader.use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}