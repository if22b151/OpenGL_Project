#include "Sphere.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

Sphere::Sphere(float radius, int sectorcount, int stackcount, const std::string& texturepath, float orbitalradius, float orbitalspeed, const std::string& normalpath)
    : m_radius(radius), m_sectorcount(sectorcount), m_stackcount(stackcount), texture(texturepath), m_orbitalradius(orbitalradius), m_orbitalspeed(orbitalspeed), normalMap(normalpath)
{
    generateVertices();
    generateIndices();
}

void Sphere::generateVertices()
{
    m_vertices.clear();

    for (int stack = 0; stack <= m_stackcount; ++stack) {

        float phi = glm::pi<float>() * float(stack) / float(m_stackcount);

        float xy = m_radius * sin(phi);

        for (int sector = 0; sector <= m_sectorcount; ++sector) {

            float theta = 2.0f * glm::pi<float>() * float(sector) / float(m_sectorcount);

            float x = m_radius * sin(phi) * cos(theta);  
            float y = m_radius * cos(phi);               
            float z = m_radius * sin(phi) * sin(theta);  

            m_vertices.push_back(x);
            m_vertices.push_back(y);
            m_vertices.push_back(z);

            // UV coordinates
            float u = 1.0f - (sector) / float(m_sectorcount);
            // For the last vertex in each row, force U to 1.0
            if (sector == m_sectorcount) {
                u = 0.0f;
            }
            float v = 1.0f - float(stack) / float(m_stackcount);

            m_vertices.push_back(u);
            m_vertices.push_back(v);

            // Normals
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            m_vertices.push_back(normal.x);
            m_vertices.push_back(normal.y);
            m_vertices.push_back(normal.z);

            // Tangent
            glm::vec3 tangent;
            if (stack == 0 || stack == m_stackcount) {
                tangent = glm::vec3(1.0f, 0.0f, 0.0f); // Arbitrary tangent for poles
            } else {
                tangent = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), normal));
            }
            m_vertices.push_back(tangent.x);
            m_vertices.push_back(tangent.y);
            m_vertices.push_back(tangent.z);
        }
    }
}

void Sphere::generateIndices()
{
    m_indices.clear();

    // Generate the indices to connect the vertices into triangles
    for (int i = 0; i < m_stackcount; ++i) {
        int currentStack = i * (m_sectorcount + 1);   
        int nextStack = (i + 1) * (m_sectorcount + 1); 

        for (int j = 0; j < m_sectorcount; ++j) {
            int current = currentStack + j;
            int next = currentStack + (j + 1);

            int currentNext = nextStack + j;
            int nextNext = nextStack + (j + 1);

            // Create two triangles for each quad
            m_indices.push_back(current);
            m_indices.push_back(currentNext);
            m_indices.push_back(next);
            m_indices.push_back(next);
            m_indices.push_back(currentNext);
            m_indices.push_back(nextNext);
        }
    }
}


Sphere::~Sphere()
{
}
