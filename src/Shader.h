#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

class Shader {

private:

	GLuint m_shaderProgramID;
	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;
	std::string m_vertexShaderSource;
	std::string m_fragmentShaderSource;
	std::string m_vertexShaderFilepath;
	std::string m_fragmentShaderFilepath;

	std::string ReadShaderFile(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream buffer;

		if (!stream.is_open()) 
		{
			std::cout << "Could not open shader: " << filepath << "." << std::endl;
			return "";
		}

		buffer << stream.rdbuf();
		const std::string source = buffer.str();

		return source;
	}

	GLuint CompileVertexShader(const std::string& source)
	{
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER); 
		const GLchar* src = source.c_str();

		glShaderSource(vertexShaderID, 1, &src, nullptr);
		glCompileShader(vertexShaderID);

		int result;
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(vertexShaderID, length, &length, message);
			std::cout << "Failed to compile the:" << m_vertexShaderFilepath << "vertex shader." << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(vertexShaderID);
			return 0;
		}

		return vertexShaderID;
	}

	GLuint CompileFragmentShader(const std::string& source)
	{
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* src = source.c_str();

		glShaderSource(fragmentShaderID, 1, &src, nullptr);
		glCompileShader(fragmentShaderID);

		int result;
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			int length;
			glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(fragmentShaderID, length, &length, message);
			std::cout << "Failed to compile the: " << m_fragmentShaderFilepath << " fragment shader." << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(fragmentShaderID);
			return 0;
		}

		return fragmentShaderID;
	}

	GLuint CreateShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
	{
		GLuint shaderProgram = 0;

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShaderID);
		glAttachShader(shaderProgram, fragmentShaderID);

		glLinkProgram(shaderProgram);
		glValidateProgram(shaderProgram);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		return shaderProgram;
	}

public:

	Shader() {
		m_shaderProgramID = 0;
		m_vertexShaderID = 0;
		m_fragmentShaderID = 0;
		m_vertexShaderSource = "";
		m_fragmentShaderSource = "";
		m_vertexShaderFilepath = "";
		m_fragmentShaderFilepath = "";
	}

	void CreateShader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath){
		m_vertexShaderFilepath = vertexShaderFilepath;
		m_fragmentShaderFilepath = fragmentShaderFilepath;
		m_vertexShaderSource = ReadShaderFile(vertexShaderFilepath);
		m_vertexShaderID = CompileVertexShader(m_vertexShaderSource);
		m_fragmentShaderSource = ReadShaderFile(fragmentShaderFilepath);
		m_fragmentShaderID = CompileFragmentShader(m_fragmentShaderSource);
		m_shaderProgramID = CreateShaderProgram(m_vertexShaderID, m_fragmentShaderID);
	}

	void Use() const
	{
		glUseProgram(m_shaderProgramID);
	}

	void Disable() const
	{
		glUseProgram(0);
	}

	inline GLuint GetId() 
	{ 
		return m_shaderProgramID; 
	}

	void SetTexture(const std::string& name, int value)
	{
		int location = glGetUniformLocation(m_shaderProgramID, name.c_str());

		if (location < 0)
			std::cout << "Could not find uniform " << name << std::endl;

		glUniform1i(location, value);
	}

	void SetFloat(const std::string& name, GLfloat f)
	{
		int location = glGetUniformLocation(m_shaderProgramID, name.c_str());

		if (location < 0)
			std::cout << "Could not find uniform " << name << std::endl;

		glUniform1f(location, f);
	}

	void SetMat4(const std::string& name, glm::mat4 matrix)
	{
		int location = glGetUniformLocation(m_shaderProgramID, name.c_str());

		if (location < 0)
			std::cout << "Could not find uniform " << name << std::endl;

		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
	}

	void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
		int location_R = glGetUniformLocation(m_shaderProgramID, "u_ColorR");
		int location_G = glGetUniformLocation(m_shaderProgramID, "u_ColorG");
		int location_B = glGetUniformLocation(m_shaderProgramID, "u_ColorB");
		int location_A = glGetUniformLocation(m_shaderProgramID, "u_ColorAlpha");

		if (location_R < 0)
			std::cout << "Could not find uniform u_ColorR" << std::endl;

		if (location_G < 0)
			std::cout << "Could not find uniform u_ColorG" << std::endl;

		if (location_B < 0)
			std::cout << "Could not find uniform u_ColorB" << std::endl;

		if (location_A < 0)
			std::cout << "Could not find uniform u_ColorAlpha" << std::endl;

		glUniform1f(location_R, r);
		glUniform1f(location_G, g);
		glUniform1f(location_B, b);
		glUniform1f(location_A, a);
	}

};