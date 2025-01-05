#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLEnv/GLEnv.h>
#include <Shader/shader.h>
#include <STB/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
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
    GLFWwindow *window = createOpenGLEnv(WIDTH, HEIGHT, "GL");

    // Make a shader program
    Shader a_Shader("vertex_shader.glsl", "fragment_shader.glsl");
    // Call it once, resize callback will call it again with resize dimensions

    // Make a VAO, VBO, and index buffer object aka Element Buffer Object aka EBO
    GLuint VAO, VBO, indexBuffer;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex data: vertex coordinates, vertex colors, texture coordinates
    /*float vertexData[] = {
        // Positions         //Colors            //Texture coordinates
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f}; // top left
    */
    float vertexData[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
    GLuint indexArray[] = {
        0, 1, 3,
        1, 2, 3};

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

    // Load our texture
    int width, height, colorChannels;
    // Flip image if texture coordinate origin is at the top does not match
    stbi_set_flip_vertically_on_load(true);
    unsigned char *textureData = stbi_load("./images/beach_min.jpg", &width, &height, &colorChannels, 0);

    // Make a GL texture object
    GLuint textureObj;
    glGenTextures(1, &textureObj);

    // Using multiple textures
    // Texture unit: the location value of a texture
    // OpenGL supports at least 16 texture units
    // The default active texture unit value is 0
    // First activate a texture unit
    glActiveTexture(GL_TEXTURE0);

    // Bind to GL_TEXURE_2D
    /*After activating a texture unit, glBindTexture() will bind the
    texture object to the currently active texture unit*/
    // For example, it binds GL_TEXTURE0 to the textureObj texture object
    glBindTexture(GL_TEXTURE_2D, textureObj);

    // load texture data to the texture object binding

    // If texture is RGBA aka 4 color channels, use GL_RGBA, otherwise use GL_RGB
    if (colorChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    }

    // Make a mipmap for the texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Texture filtering
    float borderColor[] = {0.2f, 0.77f, 1.0f, 1.0f};
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load another texture
    // First, a new texture object
    GLuint textureObj2;
    glGenTextures(1, &textureObj2);
    // Activate another texture unit, this time unit 1
    glActiveTexture(GL_TEXTURE1);
    // Bind the new active texture
    glBindTexture(GL_TEXTURE_2D, textureObj2);

    // Get the texture source data
    unsigned char *textureData2 = stbi_load("./images/tile.jpg", &width, &height, &colorChannels, 0);

    // Load texture data to the texture object binding
    if (colorChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData2);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData2);
    }
    // Set texture wrapping for each axis of a texture: clamp to edge for S, mirror repeat for T
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // If we want to use GL_CLAMP_TO_BORDER, then use glTextParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // float color[]{0.0f, 0.0f, 1.0f, 0.0f};
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    // texture cleanup
    stbi_image_free(textureData);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Z-buffer to track what goes in front of what
    // OpenGL automatically makes a Z-buffer
    // We have to turn on depth testing for OpenGL to know what goes in front of what

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    // Clear the depth buffer
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    a_Shader.use();
    int timeLoc = glGetUniformLocation(a_Shader.ID, "time");
    // Tell OpenGL which texture unit each texture sampler belongs to
    // Can also use the shader class setInt() but use glGetUniformLocation() for now
    // For example, a_Shader.setInt("texture1", 0)
    // Read as: glUniform1i(getLocationOfFirstSampler, set it to texture unit 0)

    glUniform1i(glGetUniformLocation(a_Shader.ID, "texture1"), 0);
    // Likewise, glUniform1i(getLocationOfSecondSampler, set it to texture unit 1)
    glUniform1i(glGetUniformLocation(a_Shader.ID, "texture2"), 1);
    // a_Shader.setInt("texture1,", 0);
    // a_Shader.setInt("texture2", 1);

    // Matrix transformations
    // a 3D vector in homogeneous 4D coordinates
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // a translation matrix which is initialized as an identity matrix, again 3D in homogeneous 4D
    glm::mat4 translate = glm::mat4(1.0f);
    // Set the translate matrix to translate x and y by 1
    translate = glm::translate(translate, glm::vec3(1.0f, 1.0f, 0.0f));
    // Apply the translation matrix to the vector
    vec = translate * vec;
    std::cout << vec.x << vec.y << vec.z << "\n";
    double mouseX, mouseY;
    double normalizedX, normalizedY;

    // Model matrix, rotate the object so it's like it's on a surface
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // View matrix, move it slightly back so it's in view
    // Equivalently, move the scene forwards
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // Projection matrix, use perspective projection
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(-45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    while (!glfwWindowShouldClose(window))
    {
        mouseX = 0;
        mouseY = 0;
        processInput(window);
        GLGeneralSetup(window, CLEAR_COLOR);
        glfwGetCursorPos(window, &mouseX, &mouseY);
        normalizedX = -1.0 + 2.0 * mouseX / (double)WIDTH;
        normalizedY = 1.0 - 2.0 * mouseY / (double)HEIGHT;
        // Draw the triangle
        // a_Shader.use();
        // Draw with multiple textures:
        // Activate texture unit, bind texture object
        float time = glfwGetTime();
        // scale factor for the scaling matrix
        // float scaleFactor = (sin(time) + 1.75f) * 0.5f;
        // Scale and rotate
        glm::mat4 rotateScale = glm::mat4(1.0f);
        // Make the matrix a rotation matrix.  Rotate by radian equivalent of 90degrees, the second argument is which access to rotate
        // Rotate around the x, y, and z axis
        // Modifying for animation rotation
        rotateScale = glm::translate(rotateScale, glm::vec3(-1.0f * normalizedX, -1.0f * normalizedY, 0.0));
        rotateScale = glm::rotate(rotateScale, time, glm::vec3(0, 0, 1));
        // Scale
        // Apply a scale matrix to the matrix (can apply to previously applied matrices)
        // The second argument is a vector of scaling factors for each axis.
        // Uniform scaling by scale factor
        // rotateScale = glm::scale(rotateScale, glm::vec3(scaleFactor, scaleFactor, 1.0));

        // Get the transform uniform and update the value to use the transform matrix
        GLuint transformLoc = glGetUniformLocation(a_Shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(rotateScale));

        GLuint modelLoc = glGetUniformLocation(a_Shader.ID, "model");
        // Uniform of type 4d float matrix
        /*glUniformMatrix4v(
            uniform_location,
            how many matrices to send,
            Transpose the matrix boolean,
            Matrix data to send: use GLM function value_ptr(matrix) to send the matrix data to OpenGL
        )*/
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        GLuint viewLoc = glGetUniformLocation(a_Shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projectionLoc = glGetUniformLocation(a_Shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        // Updating time uniform for texture animation
        glUniform1f(timeLoc, time);
        // Activate and bind texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureObj);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureObj2);
        // Note, VAOs manage vertex data, textures are not vertex data, no need to bind VAO before textures
        glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}