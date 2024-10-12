#include <cmath>
#include <glm/glm.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Perlin {
private:
    // Cubic interpolation between n0 and n1 using factor a
    float cerp(float n0, float n1, float a) {
        return (n1 - n0) * (3.0f - a * 2.0f) * a * a + n0;
    }

    // Fade function as defined by Ken Perlin
    float fade(float t) {
        return 6 * t * t * t * t * t - 15 * t * t * t * t + 10 * t * t * t;
    }

    // Generate a random gradient vector based on the grid point
    glm::vec2 randomGradient(int ix, int iy) {
        const unsigned w = 8 * sizeof(unsigned);
        const unsigned s = w / 2;
        unsigned a = ix, b = iy;
        a *= 3284157443;

        b ^= a << s | a >> w - s;
        b *= 1911520717;

        a ^= b << s | b >> w - s;
        a *= 2048419325;
        float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

        glm::vec2 v;
        v.x = sin(random);
        v.y = cos(random);

        return v;
    }

    // Dot product between gradient and distance vector
    float dotProductGradient(int ix, int iy, float x, float y) {
        glm::vec2 cornerGradientVector = randomGradient(ix, iy);

        float dist_x = x - static_cast<float>(ix);
        float dist_y = y - static_cast<float>(iy);

        return dist_x * cornerGradientVector.x + dist_y * cornerGradientVector.y;
    }

public:
    // Constructor
    Perlin() {
        std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed for randomness
    }

    // Perlin noise function
    float noise(float x, float y) {
        int x0 = static_cast<int>(std::floor(x)), y0 = static_cast<int>(std::floor(y));
        int x1 = x0 + 1, y1 = y0 + 1;

        float phi_x = x - static_cast<float>(x0);
        float phi_y = y - static_cast<float>(y0);

        float n00 = dotProductGradient(x0, y0, x, y);
        float n10 = dotProductGradient(x1, y0, x, y);
        float n01 = dotProductGradient(x0, y1, x, y);
        float n11 = dotProductGradient(x1, y1, x, y);

        float fadeX = fade(phi_x);
        float fadeY = fade(phi_y);

        float interpolatedX0 = cerp(n00, n10, fadeX);
        float interpolatedX1 = cerp(n01, n11, fadeX);

        return cerp(interpolatedX0, interpolatedX1, fadeY);
    }

    // Generate heightmap based on Perlin noise
    std::vector<float> generateHeightMap(int width, int height, float grid_size) {
        std::vector<float> textureData(width * height);

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                float val = 0;
                float freq = 1;
                float amp = 1;

                // Octaves to create multiple layers of noise
                for (int i = 0; i < 12; i++) {
                    val += noise(x * freq / grid_size, y * freq / grid_size) * amp;
                    freq *= 2;
                    amp /= 2;
                }

                // Adjust contrast
                val *= 1.2;

                // Clamp value to [-1, 1]
                if (val > 1.0f)
                    val = 1.0f;
                else if (val < -1.0f)
                    val = -1.0f;

                // Normalize to [0, 1] and store in textureData
                textureData[y * width + x] = (val + 1.0f) / 2.0f;
            }
        }
        return textureData;
    }

};
