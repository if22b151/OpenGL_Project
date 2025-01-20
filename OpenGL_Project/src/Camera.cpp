#include "Camera.h"

Camera::Camera(glm::vec3 startposition)
    : m_position(startposition), m_horizontalAngle(-90.0f), m_verticalAngle(0.0f), 
    m_movementSpeed(8.0f), m_mouseSensitivity(0.05f), m_firstmouse(true), 
    m_lastX(1240.0f / 2.0f), m_lastY(960.0f / 2.0f)
{
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // Calculate new front vector based on rotation angles
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(m_horizontalAngle)) * cos(glm::radians(m_verticalAngle));
    newFront.y = sin(glm::radians(m_verticalAngle));
    newFront.z = sin(glm::radians(m_horizontalAngle)) * cos(glm::radians(m_verticalAngle));

    m_front = glm::normalize(newFront);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}


void Camera::processKeyboardInput(GLFWwindow* window, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_position += m_front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_position -= m_front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_position -= m_right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_position += m_right * velocity;

    bool currentrotationkeystate = glfwGetKey(window, GLFW_KEY_R);
    if (currentrotationkeystate && !lastrotationKeyState) {
        rotationlocked = !rotationlocked;
        
        //if rotation is unlocked no subtle jump into camera position
        if (!rotationlocked) {
            glfwGetCursorPos(window, &m_lastX, &m_lastY);
        }
    }
    lastrotationKeyState = currentrotationkeystate;
}

void Camera::processMouseInput(GLFWwindow* window, float deltaTime)
{
    if (rotationlocked) 
        return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (m_firstmouse) {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstmouse = false;
        return;
    }

    float xoffset = xpos - m_lastX;

    // Reversed: y coordinates go bottom to top
    float yoffset = m_lastY - ypos;  

    m_lastX = xpos;
    m_lastY = ypos;

    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_horizontalAngle += xoffset;
    m_verticalAngle += yoffset;

    // Constrain vertical angle to prevent flipping
    if (m_verticalAngle > 89.0f)
        m_verticalAngle = 89.0f;
    if (m_verticalAngle < -89.0f)
        m_verticalAngle = -89.0f;

    updateCameraVectors();
}

bool Camera::isRotationLocked() const 
{
    return rotationlocked;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}
