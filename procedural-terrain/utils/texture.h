#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>


class Texture {
public:
	Texture() {}
	unsigned int generate2DArray(std::vector<float> data, int width, int height) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, data.data());

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
        return textureID;
	}
};

#endif