#version 330 core
layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec2 aTexCoords;

struct HeightMap {
	int width;
	int length;
};

//uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
//uniform HeightMap heightmap;


//out vec2 TexCoords;

void main() {
	gl_Position = projection * view * vec4(aPos, 1.0);    //TexCoords = aTexCoords;
}