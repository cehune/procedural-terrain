#include <cmath>
#include <glm/glm.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Perlin {
private:
    float cerp(float n0, float n1, float a) {
        return (n1 - n0) * (3.0f - a * 2.0f) * a * a + n0;
    }

    float fade(float t) { // Changed to float
        return (6 * std::pow(t, 5)) - (15 * std::pow(t, 4)) + (10 * std::pow(t, 3));
    }

    glm::vec2 randomGradient(int ix, int iy) {
        // Simple hash based on input coordinates
        unsigned hash = ix * 73856093 ^ iy * 19349663;
        hash = (hash >> 13) ^ hash; // Randomize the hash further

        // Map hash to [0, 2*Pi]
        float random = static_cast<float>(hash) * (M_PI * 2.0f / static_cast<float>(~0u));
        std::cout << random << std::endl;

        // Create the vector from the angle
        glm::vec2 v;
        v.x = sin(random);
        v.y = cos(random);

        return glm::normalize(v);
   
    }

    float dotProductGradient(int ix, int iy, float x, float y) {
        glm::vec2 cornerGradientVector = randomGradient(ix, iy);

        float dist_x = x - (float)ix;
        float dist_y = y - (float)iy;

        glm::vec2 displacementVector = glm::vec2(dist_x, dist_y);

        return glm::dot(displacementVector, cornerGradientVector);
    }

public:
    Perlin() {
        std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed for randomness
    }

    float noise(float x, float y) {
        int x0 = (int)x, y0 = (int)y, x1 = x0 + 1, y1 = y0 + 1;

        // Interpolation weight is distance from each corner
        float phi_x = x - (float)x0;
        float phi_y = y - (float)y0;

        // Compute gradients
        float n00 = dotProductGradient(x0, y0, x, y); // Bottom-left
        float n10 = dotProductGradient(x1, y0, x, y); // Bottom-right
        float n01 = dotProductGradient(x0, y1, x, y); // Top-left
        float n11 = dotProductGradient(x1, y1, x, y); // Top-right
        // Fade the interpolation weights
        float fadeX = fade(phi_x); // Fade for the x direction
        float fadeY = fade(phi_y); // Fade for the y direction

        float interpolatedX0 = cerp(n00, n10, fadeX);
        float interpolatedX1 = cerp(n01, n11, fadeX);

        return cerp(interpolatedX0, interpolatedX1, fadeY);
    }
};
