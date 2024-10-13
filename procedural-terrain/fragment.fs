#version 330 core
out vec4 FragColor;

in vec2 TexCoords;  // Make sure you're passing texture coordinates

uniform sampler2D heightmapTexture;  // Your heightmap texture

void main() {
    // Sample the heightmap texture using the passed texture coordinates
    float heightValue = texture(heightmapTexture, TexCoords).r;
    
    // Render as grayscale or adjust based on heightValue
    FragColor = vec4(vec3(heightValue), 1.0);
}