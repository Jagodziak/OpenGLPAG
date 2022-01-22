#version 330 core 
layout(location = 0) in vec3 aPos;      
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexcoord;
layout(location = 3) in mat4 aInstanceMat;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 modelInstance = model * aInstanceMat;
    Normal = mat3(transpose(inverse(modelInstance))) * aNormal;
    vec4 instancePos = modelInstance * vec4(aPos, 1.0);
    Position = vec3(instancePos);
    gl_Position = projection * view * instancePos; 
}
