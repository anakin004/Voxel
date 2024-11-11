#ifndef BLOCK_H
#define BLOCK_H


// some structures to represent certain data structures in  blocks and shaders
// these could be declared in a seperate header file for better readability
// but since they are all related to "blocks" I wanted to keep them all in one place

struct vertCoord {
	float x, y, z;
};

struct textCoord {
	float x, y;
};

struct normal {
	float x, y, z;
};


struct verts {
	vertCoord vec;
	textCoord textPos;
	unsigned char faceIndex;
};

struct blockCoords {
	textCoord topLeft;
	textCoord topRight;
	textCoord botLeft;
	textCoord botRight;
};


struct blockCoordGroup {
	blockCoords top;
	blockCoords side;
	blockCoords bottom;
};


struct indexCoords {
	unsigned int a, b, c;
};


class Block
{
public:

	Block(int sideX, int sideY, int topX, int topY, int botX, int botY, int spriteSize, float textureWidth);


	inline blockCoordGroup getBlockCoords() const
	{
		return m_Block;
	}

private:

	blockCoordGroup m_Block;


};






#endif