#include "Block.h"

#include <iostream>


Block::Block(int sideX, int sideY, int topX, int topY, int botX, int botY, int spriteSize, float textureWidth) : m_Block()

{
	//top
	m_Block.top.topLeft.x = (spriteSize * topX) / textureWidth;
	m_Block.top.topLeft.y = (spriteSize * topY + spriteSize) / textureWidth;

	m_Block.top.botLeft.x = spriteSize * topX / textureWidth;
	m_Block.top.botLeft.y = spriteSize * topY / textureWidth;

	m_Block.top.topRight.x = (spriteSize * topX + spriteSize) / textureWidth;
	m_Block.top.topRight.y = (spriteSize * topY + spriteSize) / textureWidth;

	m_Block.top.botRight.x = (spriteSize * topX + spriteSize) / textureWidth;
	m_Block.top.botRight.y = spriteSize * topY / textureWidth;

	//side

	m_Block.side.topLeft.x = (spriteSize * sideX) / textureWidth;
	m_Block.side.topLeft.y = (spriteSize * sideY + spriteSize) / textureWidth;

	m_Block.side.botLeft.x = spriteSize * sideX / textureWidth;
	m_Block.side.botLeft.y = spriteSize * sideY / textureWidth;

	m_Block.side.topRight.x = (spriteSize * sideX + spriteSize) / textureWidth;
	m_Block.side.topRight.y = (spriteSize * sideY + spriteSize) / textureWidth;

	m_Block.side.botRight.x = (spriteSize * sideX + spriteSize) / textureWidth;
	m_Block.side.botRight.y = spriteSize * sideY / textureWidth;

	// bottom

	m_Block.bottom.topLeft.x = (spriteSize * botX) / textureWidth;
	m_Block.bottom.topLeft.y = (spriteSize * botY + spriteSize) / textureWidth;

	m_Block.bottom.botLeft.x = spriteSize * botX / textureWidth;
	m_Block.bottom.botLeft.y = spriteSize * botY / textureWidth;

	m_Block.bottom.topRight.x = (spriteSize * botX + spriteSize) / textureWidth;
	m_Block.bottom.topRight.y = (spriteSize * botY + spriteSize) / textureWidth;

	m_Block.bottom.botRight.x = (spriteSize * botX + spriteSize) / textureWidth;
	m_Block.bottom.botRight.y = spriteSize * botY / textureWidth;


	}
