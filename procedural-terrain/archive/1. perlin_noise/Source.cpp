#include <iostream> // Include the iostream header for input and output
#include "./utils/perlin.hpp"
#include <math.h>
#include "SFML/Graphics.hpp"
int main()
{
    Perlin perlin = Perlin();
    const int windowWidth = 400;
    const int windowHeight = 400;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "Perlin");

    sf::Uint8* pixels = new sf::Uint8[windowWidth * windowHeight * 4];


    const int GRID_SIZE = 400;


    for (int x = 0; x < windowWidth; x++)
    {
        for (int y = 0; y < windowHeight; y++)
        {
            std::cout << x << std::endl;
            int index = (y * windowWidth + x) * 4;


            float val = 0;

            float freq = 1;
            float amp = 1;

            for (int i = 0; i < 12; i++)
            {
                val += perlin.noise(x * freq / GRID_SIZE, y * freq / GRID_SIZE) * amp;

                freq *= 2;
                amp /= 2;

            }

            // Contrast
            val *= 1.2;

            // Clipping
            if (val > 1.0f)
                val = 1.0f;
            else if (val < -1.0f)
                val = -1.0f;

            // Convert 1 to -1 into 255 to 0
            int color = (int)(((val + 1.0f) * 0.5f) * 255);

            // Set pixel color
            pixels[index] = color;
            pixels[index + 1] = color;
            pixels[index + 2] = color;
            pixels[index + 3] = 255;
        }
    }

    sf::Texture texture;
    sf::Sprite sprite;

    texture.create(windowWidth, windowHeight);

    texture.update(pixels);

    sprite.setTexture(texture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);

        window.display();
    }

    return 0;
}