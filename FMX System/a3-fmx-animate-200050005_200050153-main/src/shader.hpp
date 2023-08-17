#ifndef _SHADER_UTIL_HPP_
#define _SHADER_UTIL_HPP_
#include <GL/glew.h>

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "glm/glm.hpp"

class Shader
{
private:
	GLuint m_programId;

public:
	static Shader createShader(const std::string &vertex_path, const std::string &fragment_path);
	static GLuint LoadShaderGL(GLenum eShaderType, const std::string &strFilename);
	static GLuint CreateShaderGL(GLenum eShaderType, const std::string &strShaderFile);
	static GLuint CreateProgramGL(const std::vector<GLuint> &shaderList);
	void use();
	void setUniformBool(const std::string &name, bool value) const;
	void setUniformInt(const std::string &name, int value) const;
	void setUniformFloat(const std::string &name, float value) const;
	void setUniformVec2(const std::string &name, const glm::vec2 &value) const;
	void setUniformVec3(const std::string &name, const glm::vec3 &value) const;
	void setUniformVec4(const std::string &name, const glm::vec4 &value) const;
	void setUniformMat2(const std::string &name, const glm::mat2 &mat) const;
	void setUniformMat3(const std::string &name, const glm::mat3 &mat) const;
	void setUniformMat4(const std::string &name, const glm::mat4 &mat) const;

	void queryUniforms();
};

#endif
