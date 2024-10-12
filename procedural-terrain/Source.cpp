#include <iostream> // Include the iostream header for input and output
#include "./utils/perlin.hpp"
#include <math.h>
#include "SFML/Graphics.hpp"

int main()
{
    Perlin perlin = Perlin();
    const int windowWidth = 10;
    const int windowHeight = 10;
    std::vector<float> textureData;
    textureData = perlin.generateHeightMap(windowWidth, windowHeight, 400);
    return 0;
}