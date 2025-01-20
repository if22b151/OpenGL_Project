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
    float currentFrameTime = (float)glfwGetTime();

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

        Sphere sun(1.0f, 384, 216, "Resources/Textures/Sun2.0.jpg");
        sun.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        sun.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
      
        Sphere mercury(0.3f, 384, 216, "Resources/Textures/Mercury.jpg", 23.0f, 9.87f);
        mercury.SetPosition(glm::vec3(mercury.GetOrbitalRadius(), 0.0f, 0.0f));
        mercury.SetScale(glm::vec3(0.4f, 0.4f, 0.4f));

        Sphere venus(0.7f, 384, 216, "Resources/Textures/Venus.jpg", 32.0f, 8.02f);
        venus.SetPosition(glm::vec3(venus.GetOrbitalRadius(), 0.0f, 0.0f));
        venus.SetScale(glm::vec3(0.7f, 0.7f, 0.7f));

        Sphere earth(0.8f, 384, 216, "Resources/Textures/Earth.jpg", 40.0f, 8.78f);
        earth.SetPosition(glm::vec3(earth.GetOrbitalRadius(), 0.0f, 0.0f));
        earth.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
        auto vertices = earth.GetVertices();
        auto indices = earth.GetIndices();

        Sphere moon(0.3f, 384, 216, "Resources/Textures/Moon.jpg", 2.0f, 9.0f);
        moon.SetPosition(glm::vec3(-40.0f, 0.0f, 2.0f));
        moon.SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
        moon.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

        Sphere mars(0.5f, 384, 216, "Resources/Textures/Mars.jpg", 49.0f, 7.87f);
        mars.SetPosition(glm::vec3(mars.GetOrbitalRadius(), 0.0f, 0.0f));
        mars.SetScale(glm::vec3(0.7f, 0.7f, 0.7f));

        Sphere jupiter(2.0f, 384, 216, "Resources/Textures/Jupiter.jpg", 57.0f, 6.57f);
        jupiter.SetPosition(glm::vec3(jupiter.GetOrbitalRadius(), 0.0f, 0.0f));
        jupiter.SetScale(glm::vec3(1.5f, 1.5f, 1.5f));

        Sphere saturn(1.5f, 384, 216, "Resources/Textures/Saturn.jpg", 62.0f, 5.69f);
        saturn.SetPosition(glm::vec3(saturn.GetOrbitalRadius(), 0.0f, 0.0f));
        saturn.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

        Sphere uranus(1.0f, 384, 216, "Resources/Textures/Uranus.jpg", 74.0f, 4.81f);
        uranus.SetPosition(glm::vec3(uranus.GetOrbitalRadius(), 0.0f, 0.0f));
        uranus.SetScale(glm::vec3(0.8f, 0.8f, 0.8f));

        Sphere neptune(1.0f, 384, 216, "Resources/Textures/Neptun.jpg", 78.0f, 4.03f);
        neptune.SetPosition(glm::vec3(neptune.GetOrbitalRadius(), 0.0f, 0.0f));
        neptune.SetScale(glm::vec3(0.8f, 0.8f, 0.8f));



        //create vertex array
        VertexArray va;

        //create vertex buffer
        VertexBuffer vb(vertices.data(), vertices.size() * sizeof(float));

        VertexBufferLayout layout;

        layout.Push<float>(3);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);


        //create index buffer
        IndexBuffer ib(indices.data(), indices.size());

        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        //create projection matrix, orthographic projection because we are in 2D
        glm::mat4 proj = glm::perspective(glm::radians(50.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 300.0f);

        // Camera position
        glm::vec3 cameraPosition = glm::vec3(0.0f, 20.0f, 150.0f);

        // Looking at the origin
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);

        // Up direction is positive Y
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        //create view matrix, camera appears to be at camera position and looking at target
        glm::mat4 view = glm::lookAt(cameraPosition, target, up);

        float rotationSpeed = 30.0f; 

        // By how much to rotate the object
        float rotationAngle = 0.0f; 


        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 430");

        Renderer renderer;


        while (!glfwWindowShouldClose(window)) {
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            auto deltaTime = getDeltaTime();

            // Rotate the object
            rotationAngle += rotationSpeed * deltaTime;
            if (rotationAngle > 360.0f)
                rotationAngle -= 360.0f;

            float time = (float)glfwGetTime() / 60.0f;

            // Move the object around the sun
            mercury.SetPosition(-(glm::vec3(mercury.GetOrbitalRadius() * cos(-time * mercury.GetOrbitalSpeed()), 0.0f, mercury.GetOrbitalRadius() * sin(-time * mercury.GetOrbitalSpeed()))));
            venus.SetPosition(glm::vec3(venus.GetOrbitalRadius() * cos(-time * venus.GetOrbitalSpeed()), 0.0f, venus.GetOrbitalRadius() * sin(-time * venus.GetOrbitalSpeed())));
            earth.SetPosition(glm::vec3(earth.GetOrbitalRadius() * cos(-time * earth.GetOrbitalSpeed()), 0.0f, earth.GetOrbitalRadius() * sin(-time * earth.GetOrbitalSpeed())));
            mars.SetPosition(-(glm::vec3(mars.GetOrbitalRadius() * cos(-time * mars.GetOrbitalSpeed()), 0.0f, mars.GetOrbitalRadius() * sin(-time * mars.GetOrbitalSpeed()))));
            jupiter.SetPosition(-(glm::vec3(jupiter.GetOrbitalRadius() * cos(-time * jupiter.GetOrbitalSpeed()), 0.0f, jupiter.GetOrbitalRadius() * sin(-time * jupiter.GetOrbitalSpeed()))));
            saturn.SetPosition(glm::vec3(saturn.GetOrbitalRadius() * cos(-time * saturn.GetOrbitalSpeed()), 0.0f, saturn.GetOrbitalRadius() * sin(-time * saturn.GetOrbitalSpeed())));
            uranus.SetPosition(-(glm::vec3(uranus.GetOrbitalRadius() * cos(-time * uranus.GetOrbitalSpeed()), 0.0f, uranus.GetOrbitalRadius() * sin(-time * uranus.GetOrbitalSpeed()))));
            neptune.SetPosition(glm::vec3(neptune.GetOrbitalRadius() * cos(-time * neptune.GetOrbitalSpeed()), 0.0f, neptune.GetOrbitalRadius() * sin(-time * neptune.GetOrbitalSpeed())));

            // Update moon's position relative to Earth
            moon.SetPosition(earth.GetPosition() + glm::vec3(moon.GetOrbitalRadius() * cos(time * moon.GetOrbitalSpeed()), 0.0f, moon.GetOrbitalRadius() * sin(time * moon.GetOrbitalSpeed())));

            // Rotate the object around its own axis
            sun.SetRotation(glm::vec3(0.0f, rotationAngle, 0.0f));
            earth.SetRotation(glm::vec3(0.0f, rotationAngle, 0.0f));
            mars.SetRotation(glm::vec3(0.0f, rotationAngle, 0.0f));
            venus.SetRotation(glm::vec3(0.0f, rotationAngle, 0.0f));
            mercury.SetRotation(glm::vec3(0.0f, rotationAngle, 0.0f));
            jupiter.SetRotation(glm::vec3(0.0f, rotationAngle, 0.0f));
            saturn.SetRotation(glm::vec3(0.0f, rotationAngle, 0.0f));
            uranus.SetRotation(glm::vec3(rotationAngle, 0.0f, 0.0f));
            neptune.SetRotation(glm::vec3(0.0f, rotationAngle, 0.0f));

            // Draw the objects
            {
                ImGui::Begin("Solar System");
                ImGui::Text("Mercury");
                ImGui::Text("Venus");
                ImGui::Text("Earth");
                ImGui::Text("Moon");
                ImGui::Text("Mars");
                ImGui::Text("Jupiter");
                ImGui::Text("Saturn");
                ImGui::Text("Uranus");
                ImGui::Text("Neptune");
                ImGui::End();
            }
            //Draw Sun
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, sun.GetPosition());
                model = glm::rotate(model, glm::radians(sun.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, sun.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                sun.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //Draw mercury
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, mercury.GetPosition());
                model = glm::rotate(model, glm::radians(mercury.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, mercury.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                mercury.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //Draw venus
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, venus.GetPosition());
                model = glm::rotate(model, glm::radians(venus.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, venus.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                venus.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //Draw earth
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, earth.GetPosition());
                model = glm::rotate(model, glm::radians(earth.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, earth.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                earth.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //Draw moon
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, moon.GetPosition());
                model = glm::scale(model, moon.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                moon.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //Draw mars
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, mars.GetPosition());
                model = glm::rotate(model, glm::radians(mars.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, mars.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                mars.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //Draw Jupiter
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, jupiter.GetPosition());
                model = glm::rotate(model, glm::radians(jupiter.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, jupiter.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                jupiter.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //Draw Saturn
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, saturn.GetPosition());
                model = glm::rotate(model, glm::radians(saturn.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, saturn.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                saturn.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //Draw Uranus
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, uranus.GetPosition());
                model = glm::rotate(model, glm::radians(uranus.GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::scale(model, uranus.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                uranus.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //Draw Neptune
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, neptune.GetPosition());
                model = glm::rotate(model, glm::radians(neptune.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, neptune.GetScale());
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                neptune.texture.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }


            {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            glfwSwapBuffers(window);

            glfwPollEvents();
        }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}