#pragma once

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <iostream>
#include "GL\glew.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

class Shader {
	public:
		GLuint shaderProgram;
		void loadShader(std::string vertexShaderFileName, std::string fragmentShaderFileName);
		void useShaderProgram();

	private:
		std::string readShaderFile(std::string fileName);
		void shaderCompileLog(GLuint shaderId);
		void shaderLinkLog(GLuint shaderProgramId);
};

#endif /* Shader_hpp */

