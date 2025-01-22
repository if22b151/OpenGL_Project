#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

out vec3 FragPos;    // Fragment position in world space
out vec2 v_TexCoord; // Texture coordinates
out mat3 v_TBN;      // Tangent-Bitangent-Normal matrix

uniform mat4 u_Model;      // Model matrix
uniform mat4 u_MVP;        // Model-View-Projection matrix
uniform mat3 u_NormalMatrix; // Transpose of the inverse of the model matrix

void main() 
{
    // Transform position to clip space
    gl_Position = u_MVP * vec4(position, 1.0);
    
    // Compute the fragment position in world space
    FragPos = vec3(u_Model * vec4(position, 1.0));

    // Pass the texture coordinates
    v_TexCoord = texCoord;

    // Calculate TBN matrix for normal mapping
    vec3 T = normalize(u_NormalMatrix * tangent);
    vec3 N = normalize(u_NormalMatrix * normal);
    vec3 B = cross(N, T);
    v_TBN = mat3(T, B, N);
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;    // Fragment position in world space
in vec2 v_TexCoord; // Texture coordinates
in mat3 v_TBN;      // Tangent-Bitangent-Normal matrix

uniform vec3 u_viewPos;        // Camera position in world space
uniform Material material;     // Material properties
uniform Light light;           // Light properties
uniform sampler2D u_Texture;   // Diffuse texture
uniform sampler2D u_NormalMap; // Normal map

void main()
{
    // Sample the normal map and convert it to [-1, 1] range
    vec3 normal = texture(u_NormalMap, v_TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // Transform normal from tangent space to world space
    vec3 worldNormal = normalize(v_TBN * normal);

    // Compute distance between fragment and light source
    float distance = length(light.position - FragPos);

    // Calculate attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Ambient lighting
    vec3 ambient = light.ambient * material.ambient;
  	
    // Diffuse lighting
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(worldNormal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // Specular lighting
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, worldNormal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // Sum all components and apply texture
    vec3 result = ambient + diffuse + specular;
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = vec4(result, 1.0) * texColor;
}
