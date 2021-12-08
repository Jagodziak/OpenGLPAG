#version 330 core 
out vec4 FragColor; 

in vec2 Texcoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexture; 

uniform vec3 viewPos;

uniform vec3 ambient;

uniform vec3 directionalLight0Dir;
uniform vec3 directionalLight0Color;

uniform vec3 pointLight0Pos;
uniform vec3 pointLight0Color;

uniform vec3 pointLight1Pos;
uniform vec3 pointLight1Color;

float specularStrength = 0.5;

vec3 directionalLight(vec3 lightDir, vec3 lightColor, vec3 norm)
{
    lightDir = normalize(-lightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;  

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = max(dot(viewDir, reflectDir), 0.0);
    vec3 specular = lightColor * spec;  
        
    return diffuse + specular;
}

vec3 pointLight(vec3 lightPos, vec3 lightColor, vec3 norm)
{
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    return diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 directionalLight0 = directionalLight(directionalLight0Dir, directionalLight0Color, norm);
    vec3 pointLight0 = pointLight(pointLight0Pos, pointLight0Color, norm);
    vec3 pointLight1 = pointLight(pointLight1Pos, pointLight1Color, norm);

    FragColor = texture(ourTexture, Texcoord) * vec4(ambient + directionalLight0 + pointLight0 + pointLight1, 1.0); 
}