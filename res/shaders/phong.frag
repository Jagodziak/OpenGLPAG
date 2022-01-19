#version 330 core 
out vec4 FragColor; 

//diffuse - lambert
//specular - phong
in vec2 Texcoord;
in vec3 Normal;
in vec3 FragPos; //pixel in world

uniform sampler2D ourTexture; 

uniform vec3 viewPos; 

uniform vec3 ambient; //ambient color

uniform vec3 directionalLight0Dir; 
uniform vec3 directionalLight0Color;

uniform vec3 pointLight0Pos;
uniform vec3 pointLight0Color;

uniform vec3 spotLight0Pos;
uniform vec3 spotLight0Dir;
uniform vec3 spotLight0Color;
uniform float spotLight0Cutoff; //cos

uniform vec3 spotLight1Pos;
uniform vec3 spotLight1Dir;
uniform vec3 spotLight1Color;
uniform float spotLight1Cutoff;

float specularStrength = 0.5; 

// Constants for attenuation - sta³e wyt³umienia œwiat³a 
const float constant = 1.0;
const float linear = 0.007;
const float quadratic = 0.0002;

vec3 directionalLight(vec3 lightDir, vec3 lightColor, vec3 norm)
{
   //diffuse
    lightDir = normalize(-lightDir); 
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = lightColor * diff;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
    vec3 specular = specularStrength * lightColor * spec;  
        
    return diffuse + specular;
}

vec3 pointLight(vec3 lightPos, vec3 lightColor, vec3 norm)
{
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 16.0);
    vec3 specular = specularStrength * spec * lightColor;

    // attenuation
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    return (diffuse + specular) * attenuation;
}

vec3 spotLight(vec3 lightPos, vec3 lightVector, vec3 lightColor, float cutoff, vec3 norm)
{
    vec3 lightDir = normalize(lightPos - FragPos);
    float theta = dot(lightDir, normalize(-lightVector));

    vec3 result = vec3(0.0, 0.0, 0.0);
    if (theta > cutoff)
    {
        result = pointLight(lightPos, lightColor, norm);
    }
    return result;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 directionalLight0 = directionalLight(directionalLight0Dir, directionalLight0Color, norm);
    vec3 pointLight0 = pointLight(pointLight0Pos, pointLight0Color, norm);
    vec3 spotLight0 = spotLight(spotLight0Pos, spotLight0Dir, spotLight0Color, spotLight0Cutoff, norm);
    vec3 spotLight1 = spotLight(spotLight1Pos, spotLight1Dir, spotLight1Color, spotLight1Cutoff, norm);

    FragColor = texture(ourTexture, Texcoord) * vec4(ambient + directionalLight0 + pointLight0 + spotLight0 + spotLight1, 1.0); 
}