#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	unsigned int shaderProgramID;
	std::string vertexCodeString;
	std::string fragmentCodeString;
public:
	//std::string future shaders

	/*
	* General process:
	1. Retrieve shader files and save to strings
	2. Compile the shaders
	3. Link the shaders
	*/

	Shader(const char* vertexFile, const char* fragmentFile) {
		/*
			Links a vertex and fragment shader to a shader program, and saves that
			program as private variable shaderProgramID

			Inputs
			const char* vertexFile: Path to the vertex shader
			const char* fragmentFile: Path to the fragment shader
		*/

		/* Part 1. Read shader files*/
		// 1.1 Setup variables and streams and confirm they can throw exceptions
		std::ifstream vertexIfstream, fragmentIfstream;
		std::stringstream vertexSstream, fragmentSstream;
		vertexIfstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentIfstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// 1.2 Setup 
		try {
			vertexIfstream.open(vertexFile);
			fragmentIfstream.open(fragmentFile);
			if (!vertexIfstream.is_open() && !fragmentIfstream.is_open()) {
				throw std::runtime_error("Either the vertex or fragment if_streams failed to open");
			}
			vertexSstream << vertexIfstream.rdbuf();
			fragmentSstream << fragmentIfstream.rdbuf();
			vertexCodeString = vertexSstream.str();
			fragmentCodeString = fragmentSstream.str();

			vertexIfstream.close();
			fragmentIfstream.close();
		}
		catch (std::ifstream::failure& e) {
			std::cerr << "Shader file unsuccessfully read!" << std::endl;
			std::cerr << "Exception: " << e.what() << std::endl;
		}

		/* Part 2. Compile the shaders*/
		// 2.1 Setup shader vars
		const char* vertexShaderSource = vertexCodeString.c_str();
		const char* fragmentShaderSource = fragmentCodeString.c_str();
		int success;
		char infoLog[512];

		// 2.2 Compile shaders and check for failure
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) // Vertex
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "Error: Vertex shader linking failed!\n" << infoLog << std::endl;
		}
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) // Fragment
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "Error: Fragment shader linking failed@\n" << infoLog << std::endl;
		}

		/* Part 3. Shader linking*/
		shaderProgramID = glCreateProgram();
		glAttachShader(shaderProgramID, vertexShader);
		glAttachShader(shaderProgramID, fragmentShader);
		glLinkProgram(shaderProgramID);

		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
			std::cout << "Error: Shader linking failed! \n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	/*

	General use utils

	*/

	void use() {
		std::cout << "Using Shader";
		glUseProgram(shaderProgramID);
	}

	void remove() {
		std::cout << "Deleting Shader";
		glDeleteProgram(shaderProgramID);
		shaderProgramID = 0; // just reset to zero
	}

	void setBool(std::string loc, bool value)
	{
		GLint location = validateLocation(loc.c_str());
		glUniform1f(location, int(value));
	}
	// ------------------------------------------------------------------------
	void setInt(std::string loc, int value)
	{
		GLint location = validateLocation(loc.c_str());
		glUniform1f(location, value);
	}
	// ------------------------------------------------------------------------
	void setFloat(std::string loc, float value)
	{
		GLint location = validateLocation(loc.c_str());
		glUniform1f(location, value);
	}

	void glUniformMat4(std::string loc, const glm::mat4& mat, GLsizei count = 1, GLboolean transpose = GL_FALSE) {
		// Sets a Uniform for 4D matrix
		std::cout << "Setting new uniform via custom func: shader.glUniformMat4()\n";
		GLint location = validateLocation(loc.c_str());
		glUniformMatrix4fv(location, count, transpose, glm::value_ptr(mat));
	}

	void glUniformMat4(std::string loc, float x, float y, float z, float w) {
		// Sets a Uniform for 4D matrix
		std::cout << "Setting new uniform via custom func: shader.glUniformMat4()\n";
		GLint location = validateLocation(loc.c_str());
		glUniform4f(location, x, y, z, w);
	}

	void glUniformMat3(std::string loc, const glm::mat3& mat, GLsizei count = 1, GLboolean transpose = GL_FALSE) {
		// Sets a Uniform for 3D matrix
		std::cout << "Setting new uniform via custom func: shader.glUniformMat3()\n";
		GLint location = validateLocation(loc.c_str());
		glUniformMatrix3fv(location, count, transpose, glm::value_ptr(mat));
	}

	void glUniformMat3(std::string loc, float x, float y, float z) {
		// Sets a Uniform for 3D matrix
		std::cout << "Setting new uniform via custom func: shader.glUniformMat3()\n";
		GLint location = validateLocation(loc.c_str());
		glUniform3f(location, x, y, z);
	}

	GLint validateLocation(std::string loc) {
		/* Purely a function for additional logging*/
		GLint location = glGetUniformLocation(shaderProgramID, loc.c_str());
		if (location == -1) {
			std::cerr << "Uniform '" << loc << "' not found in shader program!" << std::endl;
			return -1;
		}
		return location;
	}
};

#endif