#ifndef CHUNK_H
#define CHUNK_H


#include <algorithm>
#include <mutex>
#include <bitset>

#include "Perlin.h"
#include "noise1234.h"
#include "Texture.h"
#include "util.h"
#include "Block.h"
#include "EBO.h"
#include "VBO.h"
#include "VAO.h"
#include "BlockCoords.h"




class Chunk {

public:

	Chunk(int xPos, int zPos);
	void InitChunk();
	void SetChunkBlocks(int , int);
	void RemoveBlock(int, int, int);
	void SetFaceTextureCoord(blockCoords& side, int corner, verts* vertice);
	void SetChunkIndices();
	void SetBlockState(int x, int y, int z, bool state);
	bool CheckBit(int x, int z, int y);
	void SetCulledFaces();
	void DestroyChunkEnd();
	void LoadChunk();
	void RenderChunk();
	void UnbindBuffers();
	void ChunkFirstLoad();
	void UpdateRenderStatus(glm::vec3& playerPos, GLuint renderDist);

	bool CheckNeighborCull(int xPos, int zPos, int xIdx, int zIdx, int yIdx, Direction dir);
	bool GetBlockState(int x, int y, int z, Direction dir);

	inline bool ChunkIsActive() { return m_IsActive; };
	inline void DeActiveChunk() { m_IsActive = false; };
	inline void ActivateChunk() { m_IsActive = true; };
	inline int* GetHeights() { return &m_HeightMap[0][0]; }
	inline bool Loaded() { return m_Loaded; };


private:

	static std::mutex m_ChunkMutex;
	std::atomic<bool> m_FirstLoad;

	bool m_Loaded;

	indexCoords* m_ChunkIndices;
	verts* m_CulledChunkVerts;

	// we will have x,z,y, then 24 verts per block
	verts m_AllChunkVerts[CHUNK_SIZE][CHUNK_SIZE][MAX_LEVELS][24];

	// we will set a block to true if it is solid, this will help for culling
	std::bitset<MAX_LEVELS> m_BlockStates[CHUNK_SIZE][CHUNK_SIZE];

	int m_HeightMap[CHUNK_SIZE][CHUNK_SIZE];

	bool m_IsActive;
	GLuint m_NumBlocks;
	GLuint m_NumVerts;

	int m_ChunkX;
	int m_ChunkZ;

	// num indicee struct elements, not actual indvidual uint values
	GLuint m_NumIndices;

	VAO m_VAO;
	EBO m_EBO;
	VBO m_VBO;



};



#endif // 
