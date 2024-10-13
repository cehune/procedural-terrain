#ifndef PERLIN_H
#define PERLIN_H

#include <cmath>
#include <array>
#include <vector>
#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine
#include <chrono>    // for std::chrono::system_clock


class Perlin {
public:
    Perlin() {
        initPermutation(); // Initialize the permutation array in the constructor
    }

    static double noise(double x, double y, double z) {
        int X = static_cast<int>(std::floor(x)) & 255; // FIND UNIT CUBE THAT
        int Y = static_cast<int>(std::floor(y)) & 255; // CONTAINS POINT.
        int Z = static_cast<int>(std::floor(z)) & 255;
        x -= std::floor(x);                             // FIND RELATIVE X,Y,Z
        y -= std::floor(y);                             // OF POINT IN CUBE.
        z -= std::floor(z);
        double u = fade(x),                             // COMPUTE FADE CURVES
            v = fade(y),                             // FOR EACH OF X,Y,Z.
            w = fade(z);
        int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z, // HASH COORDINATES OF
            B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z; // THE 8 CUBE CORNERS

        double gradAA = grad(p[AA], x, y, z);
        double gradBA = grad(p[BA], x - 1, y, z);
        double gradAB = grad(p[AB], x, y - 1, z);
        double gradBB = grad(p[BB], x - 1, y - 1, z);

        double gradAA1 = grad(p[AA + 1], x, y, z - 1);
        double gradBA1 = grad(p[BA + 1], x - 1, y, z - 1);
        double gradAB1 = grad(p[AB + 1], x, y - 1, z - 1);
        double gradBB1 = grad(p[BB + 1], x - 1, y - 1, z - 1);

        // Perform linear interpolations
        double lerpU1 = lerp(u, gradAA, gradBA);
        double lerpU2 = lerp(u, gradAB, gradBB);
        double lerpV1 = lerp(v, lerpU1, lerpU2);

        double lerpU1_1 = lerp(u, gradAA1, gradBA1);
        double lerpU2_1 = lerp(u, gradAB1, gradBB1);
        double lerpV2 = lerp(v, lerpU1_1, lerpU2_1);

        double result = lerp(w, lerpV1, lerpV2);

        return result;
    }

    std::vector<float> generateHeightMap(int width, int length, float grid_size) {
        std::vector<float> textureData;
        float z = 0.5f; // Use a constant z value for a static heightmap

        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                double val = 0.0;
                float freq = 1.0f;
                float amp = 1.0f;

                // Octaves to create multiple layers of noise
                for (int o = 0; o < 12; o++) {
                    val += Perlin::noise(i * freq / grid_size, j * freq / grid_size, z) * amp;
                    freq *= 2.0f;  // Increase frequency
                    amp /= 1.7f;   // Decrease amplitude
                }

                // Adjust contrast
                val *= 1.2;

                // Clamp value to [-1, 1]
                if (val > 1.0f)
                    val = 1.0f;
                else if (val < -1.0f)
                    val = -1.0f;

                // Normalize to [0, 1] and store in textureData
                //.//textureData.push_back(x)
                textureData.push_back(static_cast<float>((val + 1.0) / 2.0));
            }
        }
        return textureData;
    }

    std::vector<float> generateHeightMapIndices(int width, int height, float grid_size) {
        std::vector<float> indices;
        return indices;
    }

private:
    static double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }

    static double grad(int hash, double x, double y, double z) {
        int h = hash & 15; // CONVERT LO 4 BITS OF HASH CODE
        double u = h < 8 ? x : y; // INTO 12 GRADIENT DIRECTIONS.
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    static const int GRADIENT_COUNT = 512;
    static int p[GRADIENT_COUNT * 2];
    static int permutation[256];

    static void initPermutation() {
        for (int i = 0; i < 256; i++) {
            permutation[i] = i;
        }

        // Seed for randomness (using system clock)
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(permutation, permutation + 256, std::default_random_engine(seed));

        // Copy the permutation array twice into p[]
        for (int i = 0; i < 256; i++) {
            p[256 + i] = p[i] = permutation[i];
        }

    }
};

// Initialization of permutation array
int Perlin::permutation[256] = {
    151, 160, 137, 91, 90, 15,
    131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30,
    69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
    234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
    57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168,
    68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231,
    83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46,
    245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73,
    209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116,
    188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
    250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207,
    206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170,
    213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167,
    43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224,
    232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193,
    238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249,
    14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84,
    204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205,
    93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215,
    61, 156, 180
};

#endif