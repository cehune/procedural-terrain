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

    float fade(float t) {
        return (6 * std::pow(t, 5)) - (15 * std::pow(t, 4)) + (10 * std::pow(t, 3));
    }

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

        // Create the vector from the angle
        glm::vec2 v;
        v.x = sin(random);
        v.y = cos(random);

        return v;
    }

    float dotProductGradient(int ix, int iy, float x, float y) {
        glm::vec2 cornerGradientVector = randomGradient(ix, iy);

        float dist_x = x - static_cast<float>(ix);
        float dist_y = y - static_cast<float>(iy);
        glm::vec2 displacementVector = glm::vec2(dist_x, dist_y);

        return glm::dot(displacementVector, cornerGradientVector);
    }

public:
    Perlin() {
        std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed for randomness
    }

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
};
