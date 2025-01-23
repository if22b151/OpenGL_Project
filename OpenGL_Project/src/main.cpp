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
#include "Camera.h"

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

void DrawSphere(Renderer& renderer, Shader& shader, const Sphere& sphere, const glm::mat4& viewProj, const VertexArray& va, const IndexBuffer& ib) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), sphere.GetPosition());
    model = glm::rotate(model, glm::radians(sphere.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, sphere.GetScale());
    glm::mat4 mvp = viewProj * model;
    shader.Bind();
    shader.SetUniformMat4f("u_Model", model);
    shader.SetUniformMat4f("u_MVP", mvp);
    sphere.texture.Bind();
    renderer.Draw(va, ib, shader);
}

void DrawSphereWithNormalMapping(Renderer& renderer, Shader& normalShader, const Sphere& sphere, const glm::mat4& viewProj, const VertexArray& va, const IndexBuffer& ib) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), sphere.GetPosition());
    model = glm::rotate(model, glm::radians(sphere.GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, sphere.GetScale());
    glm::mat4 mvp = viewProj * model;

    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
    normalShader.Bind();
    sphere.texture.Bind(0);
    sphere.normalMap.Bind(1);
    normalShader.SetUniformMat4f("u_MVP", mvp);
    normalShader.SetUniformMat4f("u_Model", model);
    normalShader.SetUniformMat3f("u_NormalMatrix", normalMatrix);
    renderer.Draw(va, ib, normalShader);
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

    //vertices for light source
    float lightcubeVertices[] = {

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,

         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f,  0.5f,

         -0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f
    };

    unsigned int lightIndices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Back face
        5, 4, 7,
        7, 6, 5,
        // Left face
        4, 0, 3,
        3, 7, 4,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Bottom face
        4, 5, 1,
        1, 0, 4,
        // Top face
        3, 2, 6,
        6, 7, 3
    };

    VertexArray lightcubeVA;
    VertexBuffer lightcubeVB(lightcubeVertices, sizeof(lightcubeVertices));
    IndexBuffer lightcubeIB(lightIndices, sizeof(lightIndices) / sizeof(unsigned int));

    VertexBufferLayout lightLayout;
    lightLayout.Push<float>(3);
    lightcubeVA.AddBuffer(lightcubeVB, lightLayout);

    lightcubeVA.Unbind();
    lightcubeVB.Unbind();

    //shader
    Shader shader("Resources/Shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Shader sunShader("Resources/Shaders/sun.shader");
    sunShader.Bind();
    sunShader.SetUniform1i("u_Texture", 0);

    Shader normalShader("Resources/Shaders/normal.shader");
    normalShader.Bind();
    normalShader.SetUniform1i("u_Texture", 0);
    normalShader.SetUniform1i("u_NormalMap", 1);

    Shader lightCubeShader("Resources/Shaders/LightCube.shader");

    Sphere sun(1.0f, 384, 216, "Resources/Textures/Sun.jpg");
    sun.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    sun.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));

    Sphere mercury(0.3f, 384, 216, "Resources/Textures/Mercury.jpg", 23.0f, 9.87f, "Resources/Textures/NormalMapMercury.png");
    mercury.SetPosition(glm::vec3(mercury.GetOrbitalRadius(), 0.0f, 0.0f));
    mercury.SetScale(glm::vec3(0.4f, 0.4f, 0.4f));

    Sphere venus(0.7f, 384, 216, "Resources/Textures/Venus.jpg", 32.0f, 8.02f);
    venus.SetPosition(glm::vec3(venus.GetOrbitalRadius(), 0.0f, 0.0f));
    venus.SetScale(glm::vec3(0.7f, 0.7f, 0.7f));

    Sphere earth(0.8f, 384, 216, "Resources/Textures/Earth.jpg", 40.0f, 8.78f, "Resources/Textures/NormalMapEarth.png");
    earth.SetPosition(glm::vec3(earth.GetOrbitalRadius(), 0.0f, 0.0f));
    earth.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    auto vertices = earth.GetVertices();
    auto indices = earth.GetIndices();

    Sphere moon(0.3f, 384, 216, "Resources/Textures/Moon.jpg", 2.0f, 9.0f, "Resources/Textures/NormalMapMoon.png");
    moon.SetScale(glm::vec3(0.4f, 0.4f, 0.4f));
    moon.SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

    Sphere mars(0.5f, 384, 216, "Resources/Textures/Mars.jpg", 45.0f, 7.87f, "Resources/Textures/NormalMapMars.png");
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
    VertexBuffer vb(vertices.data(), (unsigned int)vertices.size() * sizeof(float));

    VertexBufferLayout layout;

    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);


    //create index buffer
    IndexBuffer ib(indices.data(), (unsigned int)indices.size());

    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    //create projection matrix, orthographic projection because we are in 2D
    glm::mat4 proj = glm::perspective(glm::radians(50.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 300.0f);


    float rotationSpeed = 30.0f;

    // By how much to rotate the object
    float rotationAngle = 0.0f;


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    Camera camera;

    Renderer renderer;

    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
    float Intensity = 500.0f; 
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f) * Intensity;
    shader.Bind();
    shader.SetUniformVec3f("light.position", lightPos);

    shader.SetUniformVec3f("material.ambient", 0.1f, 0.1f, 0.1f);
    shader.SetUniformVec3f("material.diffuse", 0.5f, 0.5f, 0.5f);
    shader.SetUniformVec3f("material.specular", 0.5f, 0.5f, 0.5f);
    shader.SetUniform1f("material.shininess", 32.0f);

    glm::vec3 ambientColor = lightColor * glm::vec3(0.1f);
    glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f);
    shader.SetUniformVec3f("light.ambient", ambientColor);
    shader.SetUniformVec3f("light.diffuse", diffuseColor);
    shader.SetUniformVec3f("light.specular", 1.0f, 1.0f, 1.0f);
    shader.SetUniform1f("light.constant", 1.0f);
    shader.SetUniform1f("light.linear", 0.7f);
    shader.SetUniform1f("light.quadratic", 0.032f);
    
    
    normalShader.Bind();
    normalShader.SetUniformVec3f("light.position", lightPos);

    normalShader.SetUniformVec3f("material.ambient", 0.1f, 0.1f, 0.1f);
    normalShader.SetUniformVec3f("material.diffuse", 0.5f, 0.5f, 0.5f);
    normalShader.SetUniformVec3f("material.specular", 0.5f, 0.5f, 0.5f);
    normalShader.SetUniform1f("material.shininess", 32.0f);

    normalShader.SetUniformVec3f("light.ambient", ambientColor);
    normalShader.SetUniformVec3f("light.diffuse", diffuseColor);
    normalShader.SetUniformVec3f("light.specular", 1.0f, 1.0f, 1.0f);
    normalShader.SetUniform1f("light.constant", 1.0f);
    normalShader.SetUniform1f("light.linear", 0.7f);
    normalShader.SetUniform1f("light.quadratic", 0.032f);normalShader.Bind();

    sunShader.Bind();
    sunShader.SetUniformVec3f("light.position", lightPos);

    sunShader.SetUniformVec3f("material.ambient", 0.1f, 0.1f, 0.1f);
    sunShader.SetUniformVec3f("material.diffuse", 0.5f, 0.5f, 0.5f);
    sunShader.SetUniformVec3f("material.specular", 0.5f, 0.5f, 0.5f);
    sunShader.SetUniform1f("material.shininess", 32.0f);
    
    sunShader.SetUniformVec3f("light.ambient", ambientColor);
    sunShader.SetUniformVec3f("light.diffuse", diffuseColor);
    sunShader.SetUniformVec3f("light.specular", 1.0f, 1.0f, 1.0f);
    sunShader.SetUniform1f("light.constant", 1.0f);
    sunShader.SetUniform1f("light.linear", 0.7f);
    sunShader.SetUniform1f("light.quadratic", 0.032f);




    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto deltaTime = getDeltaTime();

        if (camera.isRotationLocked()) {
            ImGui::Text("Camera Rotation: Locked (Press R to unlock)");
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            ImGui::Text("Camera Rotation: Unlocked (Press R to lock)");
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        camera.processKeyboardInput(window, deltaTime);
        camera.processMouseInput(window, deltaTime);

        glm::mat4 viewProj = proj * camera.getViewMatrix(); 

        shader.Bind();
        shader.SetUniformVec3f("u_viewPos", camera.getPosition());
        normalShader.SetUniformVec3f("u_viewPos", camera.getPosition());

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

        DrawSphere(renderer, sunShader, sun, viewProj, va, ib);
        DrawSphereWithNormalMapping(renderer, normalShader, mercury, viewProj, va, ib);
        DrawSphere(renderer, shader, venus, viewProj, va, ib);
        DrawSphereWithNormalMapping(renderer, normalShader, earth, viewProj, va, ib);
        DrawSphereWithNormalMapping(renderer, normalShader, moon, viewProj, va, ib);
        DrawSphereWithNormalMapping(renderer, normalShader, mars, viewProj, va, ib);
        DrawSphere(renderer, shader, jupiter, viewProj, va, ib);
        DrawSphere(renderer, shader, saturn, viewProj, va, ib);
        DrawSphere(renderer, shader, uranus, viewProj, va, ib);
        DrawSphere(renderer, shader, neptune, viewProj, va, ib);


        //Draw LightCube
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            glm::mat4 mvp = viewProj * model;
            lightCubeShader.Bind();
            lightCubeShader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(lightcubeVA, lightcubeIB, lightCubeShader);
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