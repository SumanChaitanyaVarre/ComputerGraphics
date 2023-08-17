#include "shader.hpp"



void Shader::use()
{
  glUseProgram(m_programId);
}

void Shader::queryUniforms()
{
  int n_ind = 0;
  glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &n_ind);
  std::cout << n_ind << "\n";
  for (int i = 0; i < n_ind; i++)
  {
    char name[100];
    glGetActiveUniformName(m_programId, i, 100, NULL, name);
    std::cout << name << "\n";
  }
}


void Shader::setUniformBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setUniformInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setUniformFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setUniformVec2(const std::string& name, const glm::vec2& value) const
{
  glUniform2fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

// ------------------------------------------------------------------------
void Shader::setUniformVec3(const std::string& name, const glm::vec3& value) const
{
  GLint loc;
  loc = glGetUniformLocation(m_programId, name.c_str());
    // throw std::runtime_error("Cannot find uniform: " + name);
  glUniform3fv(loc, 1, &value[0]);
}

// ------------------------------------------------------------------------
void Shader::setUniformVec4(const std::string& name, const glm::vec4& value) const
{
  glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

// ------------------------------------------------------------------------
void Shader::setUniformMat2(const std::string& name, const glm::mat2& mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setUniformMat3(const std::string& name, const glm::mat3& mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  
}
// ------------------------------------------------------------------------
void Shader::setUniformMat4(const std::string& name, const glm::mat4& mat) const
{
  GLint loc;
  if (loc = glGetUniformLocation(m_programId, name.c_str()) == -1)
    throw std::runtime_error("Cannot find uniform: " + name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

Shader Shader::createShader(const std::string& vertex_path, const std::string& fragment_path)
{
  std::vector<GLuint> shaderList;
  shaderList.push_back(LoadShaderGL(GL_VERTEX_SHADER, vertex_path));
  shaderList.push_back(LoadShaderGL(GL_FRAGMENT_SHADER, fragment_path));

  Shader shader;

  shader.m_programId = CreateProgramGL(shaderList);
  return shader;
}

GLuint Shader::LoadShaderGL(GLenum eShaderType, const std::string &strFilename)
{
  std::ifstream shaderFile(strFilename.c_str());
  if (!shaderFile.is_open())
    throw std::runtime_error("Cannot find file: " + strFilename);
   
  std::stringstream shaderData;
  shaderData << shaderFile.rdbuf();
  shaderFile.close();
    
  try
    {
return CreateShaderGL(eShaderType, shaderData.str());
    }
  catch(std::exception &e)
    {
std::cerr<<e.what()<<std::endl;
throw;
    }
}
  
GLuint Shader::CreateShaderGL(GLenum eShaderType, const std::string &strShaderFile)
{
 GLuint shader = glCreateShader(eShaderType);
 const char *strFileData = strShaderFile.c_str();
 glShaderSource(shader, 1, &strFileData, NULL);
    
 glCompileShader(shader);
    
 GLint status;
 glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
 if (status == GL_FALSE)
  {
   GLint infoLogLength;
   glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	
   GLchar *strInfoLog = new GLchar[infoLogLength + 1];
   glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
	
   const char *strShaderType = NULL;
   switch(eShaderType)
	   {
	   case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
	   case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
	   case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
	   }
	
   std::cerr<<"Compile failure in "<<strShaderType<<" shader:"<<std::endl<<strInfoLog<<std::endl;
   delete[] strInfoLog;
  }
    
 return shader;
}
  
GLuint Shader::CreateProgramGL(const std::vector<GLuint> &shaderList)
{
  GLuint program = glCreateProgram();
    
  for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    glAttachShader(program, shaderList[iLoop]);
    
  glLinkProgram(program);
    
  GLint status;
  glGetProgramiv (program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
    {
GLint infoLogLength;
glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	
GLchar *strInfoLog = new GLchar[infoLogLength + 1];
glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
std::cerr<<"GLSL Linker failure: "<<strInfoLog<<std::endl;
delete[] strInfoLog;
    }
    
  for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    glDetachShader(program, shaderList[iLoop]);
    
  return program;
}
