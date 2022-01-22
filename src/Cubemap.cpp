#include "Cubemap.hpp"

#include <stb_image.h>

Cubemap::Cubemap()
{
	cubemapID = 0;
}

void Cubemap::load(std::string cubemapPath)
{
    int width, height, nrChannels;
    unsigned char* data;

    glGenTextures(1, &cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::string faces[] = { "px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png" };

    for (unsigned int i = 0; i < 6; i++)
    {
        data = stbi_load((cubemapPath + faces[i]).c_str(), &width, &height, &nrChannels, 0);
        GLint format = nrChannels > 3 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    stbi_image_free(data);
}

GLuint Cubemap::getCubemapId()
{
	return cubemapID;
}
