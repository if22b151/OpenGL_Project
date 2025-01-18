#pragma once
#include "Renderer.h"

#include <vector>

class Sphere
{
private:
    float m_radius;
    int m_sectorcount;
    int m_stackcount;
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

private:
    void generateVertices();
    void generateIndices();

public:
    Sphere(float radius, int sectorcount, int stackcount);
    ~Sphere();

    inline const std::vector<float>& GetVertices() const { return m_vertices; }
    inline const std::vector<unsigned int>& GetIndices() const { return m_indices; }

};