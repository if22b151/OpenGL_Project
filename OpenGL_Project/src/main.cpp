#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Sphere.h"

#define WINDOW_WIDTH 1240   
#define WINDOW_HEIGHT 960


float lastFrameTime = 0.0f;

// Function to calculate and return delta time
float getDeltaTime() {
    // Get the current time
    float currentFrameTime = glfwGetTime();

    // Calculate delta time (time difference between frames)
    float deltaTime = currentFrameTime - lastFrameTime;

    // Update lastFrameTime for the next frame
    lastFrameTime = currentFrameTime;

    return deltaTime;
}


int main() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL_Project", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    {
        //float positions[] = {
        //    -50.0f, -50.0f, 0.0f, 0.0f, //0
        //     50.0f, -50.0f, 1.0f, 0.0f, //1
        //     50.0f, 50.0f, 1.0f, 1.0f, //2
        //    -50.0f, 50.0f, 0.0f, 1.0f  //3
        //};

        ////use index buffer to draw something without repeating vertices
        //unsigned int indices[] = {
        //    0, 1, 2,
        //    2, 3, 0
        //};

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(Renderer::GLDebugMessage, 0);
        
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //shader
        Shader shader("Resources/Shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);

        //Texture
        Texture texture("Resources/Textures/Earth1920x1080.jpg");
        texture.Bind();
        

        Sphere sphere(1.0f, 384, 216);

        auto vertices = sphere.GetVertices();

        //create vertex array
        VertexArray va;

        //create vertex buffer
        VertexBuffer vb(vertices.data(), vertices.size() * sizeof(float));

        VertexBufferLayout layout;

        layout.Push<float>(3);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);


        auto indices = sphere.GetIndices();
        //create index buffer
        IndexBuffer ib(indices.data(), indices.size());

        //create projection matrix, orthographic projection because we are in 2D
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
       
        // Camera at (0, 0, 3)
        glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 4.0f);
        
        // Looking at the origin
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);       
        
        // Up direction is positive Y
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);            

        //create view matrix, camera appears to be at camera position and looking at target
        glm::mat4 view = glm::lookAt(cameraPosition, target, up);
        
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 430");

        //glm::vec3 translationA(200, 200, 0);
        //glm::vec3 translationB(400, 200, 0);
        auto objectposition = glm::vec3(0.0f, 0.0f, 0.0f);
        /*float rotationX = glm::radians(0.0f);
        float rotationY = glm::radians(0.0f);
        float rotationZ = glm::radians(0.0f);
        auto rotationAxisx = glm::vec3(1.0f, 0.0f, 0.0f);
        auto rotationAxisy = glm::vec3(0.0f, 1.0f, 0.0f);
        auto rotationAxisz = glm::vec3(1.0f, 0.0f, 0.0f);*/
        auto scale = glm::vec3(1.0f, 1.0f, 1.0f);

        float rotationSpeed = 30.0f; // Rotation speed in degrees per second
        float rotationAngle = 0.0f; // Current rotation angle in degrees

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

           
          /*  {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }  
            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }*/

            auto deltaTime = getDeltaTime();

            // Rotate the object
            rotationAngle += rotationSpeed * deltaTime;
            if (rotationAngle > 360.0f)
                rotationAngle -= 360.0f;

            //Draw a sphere
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, objectposition);
                /*model = glm::rotate(model, rotationX, rotationAxisx);
                model = glm::rotate(model, rotationY, rotationAxisy);
                model = glm::rotate(model, rotationZ, rotationAxisz);*/
                model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, scale);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }


            {
                //imgui for changing rotation of object on all axis with three angles
              /*ImGui::SliderAngle("Rotation X", &rotationX, -180.0f, 180.0f);
                ImGui::SliderAngle("Rotation Y", &rotationY, -180.0f, 180.0f);
                ImGui::SliderAngle("Rotation Z", &rotationZ, -180.0f, 180.0f);*/
            

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}