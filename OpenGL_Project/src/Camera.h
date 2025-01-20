#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Rotation around Y-axis (left/right)
    float m_horizontalAngle;

    // Rotation around X-axis (up/down)
    float m_verticalAngle;

    // Camera parameters
    float m_movementSpeed;
    float m_mouseSensitivity;

    // Last mouse position for calculating delta
    double m_lastX;
    double m_lastY;
    bool m_firstmouse;

    bool rotationlocked = false;
    bool lastrotationKeyState = false;

    void updateCameraVectors();

public:
    Camera(glm::vec3 startposition = glm::vec3(0.0f, 20.0f, 130.0f));

    void processKeyboardInput(GLFWwindow* window, float deltaTime);
    void processMouseInput(GLFWwindow* window, float deltaTime);
    bool isRotationLocked() const;

    glm::mat4 getViewMatrix() const;

};