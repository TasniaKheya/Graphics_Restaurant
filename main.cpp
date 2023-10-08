//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void makeT(unsigned int VAO, Shader ourShader, glm::mat4 sm, float a, float b, float c, float d);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawTable(unsigned int VAO, Shader ourShader, glm::mat4 sm);
void drawCHair(unsigned int VAO, Shader ourShader, glm::mat4 sm);
void drawTiles(unsigned int VAO, Shader ourShader, glm::mat4 sm);
void makeTool(unsigned int VAO, Shader ourShader, glm::mat4 sm);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 90.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 0.3;
float scale_Y = 0.3;
float scale_Z = 0.3;
bool fanRotating = false;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

void fanSpinning()
{
    rotateAngle_Y += 45.0 * deltaTime;
    rotateAxis_X = 0.0;
    rotateAxis_Y = 1.0;
    rotateAxis_Z = 0.0;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.3f, 0.8f, 0.5f,
        0.5f, 0.0f, 0.0f, 0.5f, 0.4f, 0.3f,
        0.5f, 0.5f, 0.0f, 0.2f, 0.7f, 0.3f,
        0.0f, 0.5f, 0.0f, 0.6f, 0.2f, 0.8f,
        0.0f, 0.0f, 0.5f, 0.8f, 0.3f, 0.6f,
        0.5f, 0.0f, 0.5f, 0.4f, 0.4f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.3f, 0.6f,
        0.0f, 0.5f, 0.5f, 0.7f, 0.5f, 0.4f
    };*/
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f
    };*/
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    /*unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };*/
    /*float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };*/
    // world space positions of our cubes
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view = basic_camera.createViewMatrix();
        ourShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        float a = 0.0f, b = 0.0f, c = 0.0f, d = 1.0f;
          
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.4f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.2f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }



        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.4f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.2f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.8f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }


        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.4f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.6f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.6f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        a = 1.0f, b = 1.0f, c = 1.0f, d = 1.0f;

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.4f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.2f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }



        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.4f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.2f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.8f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }


        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, -0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.4f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 9; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.6f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.6f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }



        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.3f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, a, b, c, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, 0.0, 0.0, 0.0, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.2f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, 0.0, 0.0, 0.0, d);
        }


        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.0f, -0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, 0.0, 0.0, 0.0, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 0.0f, 0.0f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, 0.0, 0.0, 0.0, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, 0.0, 0.0, 0.0, d);
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.3f, 0.0f, 0.2f));
        makeT(VAO, ourShader, translateMatrix, a, b, c, d);

        for (int i = 0; i < 10; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.4f));
            makeT(VAO, ourShader, translateMatrix, 0.0, 0.0, 0.0, d);
        }




        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, -.7f, .95f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.3f, 1.0f, 0.7f));
        drawCHair(VAO, ourShader, rotateYMatrix* translateMatrix* scaleMatrix);
        
        translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(-0.0, 0.0, 0.0));

        for (int i = 0; i < 2; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f,-2.0f));
            drawCHair(VAO, ourShader, rotateYMatrix * translateMatrix * scaleMatrix);
        }
       

        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(.6f, -0.2f, 1.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.6f, 0.5f));
        drawTable(VAO, ourShader, rotateXMatrix* rotateYMatrix *  translateMatrix*scaleMatrix);

        //translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(-0.0, 0.0, 0.0));

        for (int i = 0; i < 2; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -2.0f));
            drawTable(VAO, ourShader, translateMatrix* scaleMatrix);
        }



       translateMatrix = glm::translate(identityMatrix, glm::vec3(-.90,-.4, -3.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 0.7,10.0));
        model = translateMatrix * scaleMatrix;

        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.9, 0.6f, 0.4f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0, -.0, -3.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8, 0.1, 10.0));
        model = translateMatrix * scaleMatrix;

        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 0.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);




        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3, 1.1, -3.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3, 0.1, 10.0));
        model = translateMatrix * scaleMatrix;

        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3, .50, -3.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3, 0.1, 10.0));
        model = translateMatrix * scaleMatrix;

        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3, .85, -3.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3, 0.1, 10.0));
        model = translateMatrix * scaleMatrix;

        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);




        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.6, -1.1, -5.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(8.0, 7.0, 1.0));
        model = translateMatrix * scaleMatrix;

        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.7, 0.0f, 0.7f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1, 0.5, -2.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6, 0.6, 1.0));
        model = translateMatrix * scaleMatrix;

        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.1, 0.0f, 0.4f, 0.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.3, -1.0, -5.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0, 7.0, 16.0));
        model = translateMatrix * scaleMatrix;

        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.5, 0.0f, 0.5f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.7, -1.0, -4.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, 7.0, 16.0));
        model = translateMatrix * scaleMatrix;

        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.4, 0.0f, 0.4f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
       
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.6,0.2,1.3));

        makeTool(VAO, ourShader,
            translateMatrix);

        for (int i = 0; i < 4; i++)
        {
            translateMatrix = translateMatrix * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.9f));
            makeTool(VAO, ourShader,
                translateMatrix);
        }

       

      
        /*
         translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        */
        // render boxes
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing
        //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = 20.0f * i;
        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    ourShader.setMat4("model", model);

        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void makeT(unsigned int VAO, Shader ourShader, glm::mat4 sm, float a, float b, float c, float d) {
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, -1.0, -0.1));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 0.0, 0.4));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(a, b, c, d));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


}
void drawTiles(unsigned int VAO, Shader ourShader, glm::mat4 sm)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0,-1.0));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5F, 0.0f, 0.5f));
    model = sm  * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.24f, -0.1f, 0.51f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5F, 1.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);




    /*translateMatrix = glm::translate(identityMatrix, glm::vec3(0.24f, -0.1f, 0.51f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5F, 1.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 0.0f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5F, 1.0f, 0.2f));
    model = sm *translateMatrix  * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



     translateMatrix = glm::translate(identityMatrix, glm::vec3(0.75f, -0.1f, 0.51f));
     rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
     scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5F, 1.0f, 0.2f));
     model = sm *translateMatrix  * scaleMatrix;
     ourShader.setMat4("model", model);
     //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
     glBindVertexArray(VAO);
     glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.50f, 0.0f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5F, 1.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.75f, -0.1f, 0.51f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5F, 1.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.0f, 0.0f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5F, 1.0f, 0.2f));
    model = sm *translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4f, 0.0f, -0.3f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5F, 1.0f, 0.2f));
    model = sm * translateMatrix  * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

}
void drawCHair(unsigned int VAO, Shader ourShader, glm::mat4 sm)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, rotateX1Matrix, rotateX2Matrix;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1,- 0.2, -1.1));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 0.6, 1.5));
    model =sm*  translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(1.0f, 0.1f, 0.0f,1.0f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1f, -0.2f, -1.1));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 1.6, 1.5));
    model = sm * translateMatrix * rotateYMatrix *scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.1f, 0.0f, 1.0f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void drawTable(unsigned int VAO, Shader ourShader, glm::mat4 sm)
{

    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, rotateX1Matrix, rotateX2Matrix;
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 0.2f, 2.0f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.9, 0.6f, 0.4f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = sm * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.3f, 0.1f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.3f, 0.1f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.3f, 0.1f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.3f, 0.1f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 0.2f, 2.0f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.3f, 0.1f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = sm * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.3f, 0.1f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.3f, 0.1f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.3f, 0.1f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.5, 0.3f, 0.1f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void makeTool(unsigned int VAO, Shader ourShader, glm::mat4 sm) {
    // Modelling Transformation
  
    /*translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(.06, 0.0, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, -0.9, 0.2));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, -0.5, 0.02));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 0.0, 0.4));
    model = sm *  translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, -1.0, -0.1));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 0.0, 0.4));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.3, 0.4, 1.0, 0.0));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.07, -1.0, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 1.0, 0.1));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(1.0, 1.0, 1.0, 1.0));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, -0.5, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 0.4, 0.4));
    model = sm * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(1.0, 0.0, 1.0, 1.0));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 1;
        else if (rotateAxis_Y) rotateAngle_Y -= 1;
        else rotateAngle_Z -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        if (fanRotating) {
            fanRotating = false;
        }
        else {
            fanRotating = true;
            fanSpinning();
        }

    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.01;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        lookAtX += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        lookAtX -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        lookAtY += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        lookAtY -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        lookAtZ += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        lookAtZ -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(YAW_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(YAW_L, deltaTime);
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
