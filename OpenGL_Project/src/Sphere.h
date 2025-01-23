#pragma once
#include "Renderer.h"
#include "Texture.h"

#include <vector>

class Sphere
{
private:
    float m_radius;
    int m_sectorcount;
    int m_stackcount;
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

    float m_orbitalradius;
    float m_orbitalspeed;

private:
    //NOTE: Change generateVertices() and generateIndices() to public because now we generate vertices everytime we create a new sphere
    //but we want to generate them only once and then use them for all spheres
    

public:
    Texture texture;
    Texture normalMap;

public:
    Sphere(float radius, int sectorcount, int stackcount, const std::string& texturepath, float orbitalradius = 0.0f, float orbitalspeed = 0.0f, const std::string& normalpath = "");
    ~Sphere();

    const std::vector<float>& generateVertices();
    const std::vector<unsigned int>& generateIndices();


    //Getters
    inline const std::vector<float>& GetVertices() const { return m_vertices; }
    inline const std::vector<unsigned int>& GetIndices() const { return m_indices; }
    inline const glm::vec3 GetPosition() const { return m_position; }
    inline const glm::vec3 GetRotation() const { return m_rotation; }
    inline const glm::vec3 GetScale() const { return m_scale; }
    inline const float GetOrbitalRadius() const { return m_orbitalradius; }
    inline const float GetOrbitalSpeed() const { return m_orbitalspeed; }
    //Setters
    inline void SetPosition(const glm::vec3& position) { m_position = position; }
    inline void SetRotation(const glm::vec3& rotation) { m_rotation = rotation; }
    inline void SetScale(const glm::vec3& scale) { m_scale = scale; }


};