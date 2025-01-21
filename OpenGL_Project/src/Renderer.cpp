#include "Renderer.h"

#include <iostream>



void Renderer::Clear() const
{
    // Because of clearing depth buffer, upper and lower part of the sphere rotate in the same direction
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void GLAPIENTRY Renderer::GLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
	    std::cout << "---------------" << std::endl;    
        std::cout << "Debug message (" << id << "): " << message << std::endl;
        std::cout << "Source: ";
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:             std::cout << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Other"; break;
        }
        std::cout << std::endl;

        std::cout << "Type: ";
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Other"; break;
        }
        std::cout << "---------------" << std::endl;
        std::cout << std::endl;
    }
    else if (severity == GL_DEBUG_SEVERITY_HIGH) {
        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " << message << std::endl;
        std::cout << "Source: ";
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:             std::cout << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Other"; break;
        }
        std::cout << std::endl;
        std::cout << "Type: ";
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Other"; break;
        }
        std::cout << "---------------" << std::endl;    
        std::cout << std::endl;
    }
    
}
