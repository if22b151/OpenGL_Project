#pragma once

#include "Renderer.h"

class Texture {
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    float m_AspectRatio;
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline const int GetWidth() const { return m_Width; }
    inline const int GetHeight() const { return m_Height; }
    inline const float GetAspectRatio() const { return m_AspectRatio; }
};