#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "texture.hpp"

Texture::Texture(std::string filename)
{
 unsigned char* data = stbi_load(&filename[0], &width, &height, &n_channels, 0);

 glGenTextures(1, &texture_ID);
 glBindTexture(GL_TEXTURE_2D, texture_ID);

 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
 if (data)
 {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
 }
 stbi_image_free(data);
}

GLuint Texture::getTextureID()
{
 return texture_ID;
}

void Texture::FreeTexture()
{
 glDeleteTextures(1, &texture_ID); // delete the texture_ID
}

Cubemap::Cubemap(std::string cubemap_dir)
{
 glGenTextures(1, &texture_ID);
 glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ID);
 for (unsigned int i = 0; i < 6; i++)
 {
  unsigned char* data = stbi_load((cubemap_dir + faces[i]).c_str(), &width, &height, &n_channels, 0);
  if (data)
   glTexImage2D(
    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
   );
  else
   std::cout << "Error loading texture" << std::endl;
  stbi_image_free(data);
 }
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

GLuint Cubemap::getTextureID()
{
 return texture_ID;
}

void Cubemap::FreeTexture()
{
 glDeleteTextures(1, &texture_ID);
}
