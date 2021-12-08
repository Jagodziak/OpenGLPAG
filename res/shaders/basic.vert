#version 330 core 
layout(location = 0) in vec3 aPos;      
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexcoord;
layout(location = 3) in vec3 aOffset;

out vec2 Texcoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model; 
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Texcoord = aTexcoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    vec4 instancePos = vec4(aPos + aOffset, 1.0);
    FragPos = vec3(model * instancePos);
    gl_Position = projection * view * model * instancePos; 
}
