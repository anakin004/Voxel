#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <array>
#include <stb/stb_image.h>
#include <optional>


#include "shaderClass.h"
#include "util.h"
#include "Block.h"



class Texture
{

private:

	GLuint m_ID;
	std::string m_FilePath;
	unsigned char* m_ImgBytes;
	GLint m_Width, m_Height, m_BPP, m_SpriteSize;
	static std::array<std::optional<Texture>, 4> m_Textures;

	Texture(const std::string& path, int);
	
	inline const GLuint getTextureID() const { return m_ID; };


public:

	static void InitTextures();

	Block GenerateSpriteMesh(int sideX, int sideY, int topX, int topY, int botX, int botY);
	void SetBlock(verts block[], unsigned int size);

	void Bind(GLuint slot = 0) const;
	void Unbind() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }
	inline int getSpriteSize() const { return m_SpriteSize; };
	static std::optional<Texture>& GetTexture(int index);
};




#endif // TEXTURE_H
