#include <GL/glew.h>
#include <vector>
#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

class Texture
{
private:
 int width=0, height=0, n_channels=0;
 GLuint texture_ID;

public:
 Texture(std::string filename);
 GLuint getTextureID();
 void FreeTexture();
};

class Cubemap
{
private:
 int width=0, height=0, n_channels=0;
 GLuint texture_ID;
 const std::vector<std::string> faces = {
  "posx.jpg",
  "negx.jpg",
  "posy.jpg",
  "negy.jpg",
  "posz.jpg",
  "negz.jpg"
 };

public:
 Cubemap(std::string cubemap_dir);
 GLuint getTextureID();
 void FreeTexture();
};




#endif
