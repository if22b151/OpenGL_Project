#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

out vec2 v_TexCoord;
out vec3 v_FragPos;
out mat3 v_TBN;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat3 u_NormalMatrix; // Transpose of the inverse of the model matrix

void main() 
{
    // Transform position to clip space
    gl_Position = u_MVP * vec4(position, 1.0);
    
    // Pass the texture coordinates
    v_TexCoord = texCoord;
    
    // Compute the fragment position in world space
    v_FragPos = vec3(u_Model * vec4(position, 1.0));
    
    // Calculate TBN matrix for normal mapping
    vec3 T = normalize(u_NormalMatrix * tangent);
    vec3 N = normalize(u_NormalMatrix * normal);
    vec3 B = cross(N, T);
    v_TBN = mat3(T, B, N);
}

#shader fragment
#version 430 core

layout (location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_FragPos;
in mat3 v_TBN;

uniform sampler2D u_Texture;      // Diffuse texture
uniform sampler2D u_NormalMap;    // Normal map
uniform vec3 u_LightPos;          // Sun position (world space)
uniform vec3 u_CameraPos;         // Camera position (world space)
uniform vec3 u_LightColor;        // Light color

void main()
{
    // Sample the normal map and convert it to [-1, 1] range
    vec3 normal = texture(u_NormalMap, v_TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // Transform normal from tangent space to world space
    vec3 worldNormal = normalize(v_TBN * normal);
    
    // Compute lighting
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(worldNormal, lightDir), 0.0); // Diffuse component
    
    // Specular lighting
    vec3 viewDir = normalize(u_CameraPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, worldNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // Shininess factor
    
    // Combine components
    vec3 ambient = 0.1 * u_LightColor; // Ambient lighting
    vec3 diffuse = diff * u_LightColor;
    vec3 specular = spec * u_LightColor;
    
    vec3 finalColor = (ambient + diffuse + specular) * texture(u_Texture, v_TexCoord).rgb;
    color = vec4(finalColor, 1.0);
}
