#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normals;

out vec3 FragPos;
out vec2 v_TexCoord;
out vec3 out_Normals;

uniform mat4 u_Model;
uniform mat4 u_MVP;

void main() 
{
	FragPos = vec3(u_Model * vec4(position, 1.0));
	out_Normals = mat3(transpose(inverse(u_Model))) * normals;  
	v_TexCoord = texCoord;
    gl_Position = u_MVP * vec4(position, 1.0);
}

#shader fragment
#version 430 core
layout (location = 0) out vec4 color;

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

in vec3 FragPos;  
in vec3 out_Normals;  
in vec2 v_TexCoord;

uniform vec3 u_viewPos;
uniform Material material;
uniform Light light;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);

    // Compute distance between fragment and light source
    float distance = length(light.position - FragPos);

    // Calculate attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(out_Normals);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    // Add emissive term for the sun
    vec3 emissive = vec3(1.0, 0.8, 0.4); // Bright orange-yellow color for the sun
    emissive *= 1.5; // Adjust the brightness of the emission

    // Sum all components to get final color
    vec3 result = ambient + diffuse + specular + emissive;
    color = vec4(result, 1.0) * texColor;
}
