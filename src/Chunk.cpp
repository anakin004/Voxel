#include "Chunk.h"
#include "World.h"

std::mutex Chunk::m_ChunkMutex;

Chunk::Chunk(int xOffset, int zOffset) :
	m_IsActive(false), m_VAO(), m_VBO(), m_EBO(),
	m_NumBlocks(0),
	m_NumIndices(0),
	m_NumVerts(0),
	m_ChunkX(xOffset), m_ChunkZ(zOffset),
	m_Loaded(false),
	m_ChunkIndices(nullptr),
	m_CulledChunkVerts(nullptr),

	// all bits will be initialized to 0
	m_BlockStates()
{

	// we do all the side faces then top then bottom in the BaseBlock declaration
	const int vertsPerBlock = sizeof(BaseBlock) / sizeof(BaseBlock[0]);
	m_CulledChunkVerts = new verts[(vertsPerBlock * CHUNK_SIZE*CHUNK_SIZE*2 + MAX_LEVELS*CHUNK_SIZE * 4)];


	const int indicesPerBlock = sizeof(BaseBlockIndices) / sizeof(BaseBlockIndices[0]);
	m_ChunkIndices = new indexCoords[indicesPerBlock * CHUNK_SIZE * CHUNK_SIZE * MAX_LEVELS];


	m_FirstLoad.store(false);

	m_VAO.LinkAttrib(m_VBO, 0, 3, GL_FLOAT, sizeof(verts), (void*)0);
	m_VAO.LinkAttrib(m_VBO, 1, 2, GL_FLOAT, sizeof(verts), (void*)(offsetof(verts, textPos)));
	m_VAO.LinkAttrib(m_VBO, 2, 1, GL_BYTE, sizeof(verts), (void*)(offsetof(verts, faceIndex)));

	UnbindBuffers();
}



void Chunk::InitChunk()
{
	SetChunkBlocks(m_ChunkX, m_ChunkZ);
	SetCulledFaces();
	SetChunkIndices();

	m_FirstLoad.store(true);
}

void Chunk::ChunkFirstLoad()
{


	if (m_FirstLoad.load())
	{
		m_Loaded = true;
		LoadChunk();
	}

}

void Chunk::SetBlockState(int x, int y, int z, bool state)
{
	if (state) 
		m_BlockStates[x][z].set(y);
	
	else
		m_BlockStates[x][z].reset(y);
}


// this is a little shorthand I made when we 
// want to cull faces and we only add the necesary faces to a new array when rendering
const indexCoords face[4]{
	{0, 1, 2},  // f. left tri
	{1, 2, 3},  // f. right tri
	{4, 5, 6},  // ls. left tri
	{5, 6, 7},  // ls. right tri
};

void Chunk::SetChunkIndices()
{

 	const int indicesPerBlock = sizeof(BaseBlockIndices)/sizeof(BaseBlockIndices[0]);
	const int vertsPerBlock = sizeof(BaseBlock) / sizeof(BaseBlock[0]);

	// reset if chunk is dirty
	m_NumIndices = 0;

	for (GLuint i = 0; i < m_NumVerts; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			indexCoords temp = face[k];

			temp.a += i << 3;
			temp.b += i << 3;
			temp.c += i << 3;

			m_ChunkIndices[i*4 + k] = temp;
			m_NumIndices += 1;
		}
	}
}

void Chunk::RemoveBlock(int x, int y, int z)
{

	// Mark the block as removed by resetting its state
	m_BlockStates[x][z].reset(y);

	// Now update the chunk's faces and indices to reflect the block removal
	SetCulledFaces();
	SetChunkIndices();

	LoadChunk();
	
}



void Chunk::LoadChunk()
{

 	m_VBO.LoadBuffer(m_CulledChunkVerts, m_NumVerts);
 	m_EBO.LoadBuffer(m_ChunkIndices, m_NumIndices);
}

void Chunk::UnbindBuffers()
{
	m_VAO.Unbind();
 	m_VBO.Unbind();
 	m_EBO.Unbind();
}



void Chunk::SetFaceTextureCoord(blockCoords& coord, int corner, verts* vertice)
{

	switch (corner)
	{
	case BOTTOM_LEFT:
		vertice->textPos.x = coord.botLeft.x;
		vertice->textPos.y = coord.botLeft.y;
		break;

	case BOTTOM_RIGHT:
		vertice->textPos.x = coord.botRight.x;
		vertice->textPos.y = coord.botRight.y;
		break;

	case TOP_LEFT:
		vertice->textPos.x = coord.topLeft.x;
		vertice->textPos.y = coord.topLeft.y;
		break;

	case TOP_RIGHT:
		vertice->textPos.x = coord.topRight.x;
		vertice->textPos.y = coord.topRight.y;
		break;

	}
}

void Chunk::RenderChunk()
{
	if (m_IsActive) {
		m_VAO.Bind();
		m_VBO.Bind();
		m_EBO.Bind();
		// wire mesh 
	   //GlCall(glDrawElements(GL_LINES, m_NumIndices* 3, GL_UNSIGNED_INT, 0));
	   // actual blocks
		GlCall(glDrawElements(GL_TRIANGLES, m_NumIndices * 3, GL_UNSIGNED_INT, 0));
	}
}



static inline Chunk* GetNeighborChunk(int x, int z) { return World::GetChunk(x / 16, z / 16); }


bool Chunk::CheckBit(int x, int y, int z)
{
	bool solid = m_BlockStates[x][z].test(y);
	
	return solid;
}

// dir will be the face we are checking we want to cull an edge block, if the face 
// is north facing, it means we need to  z + 15
// if its south facing then its  z - 15
// left facing - x + 15, right facing x - 15;
bool Chunk::GetBlockState(int x, int y, int z, Direction dir)
{
	if (y < 0)
		return false;


	bool isSolid = false;

	switch (dir) {

	case NORTH:
		isSolid = CheckBit(x, z - CHUNK_SIZE + 1, y);
		return isSolid;
	case SOUTH:
		isSolid = CheckBit(x, z + CHUNK_SIZE - 1, y);
		return isSolid;
	case EAST:
		isSolid = CheckBit(x - CHUNK_SIZE + 1, z, y+1);
		return isSolid;
	case WEST:
		isSolid = CheckBit(x + CHUNK_SIZE - 1, z, y+1);
		return isSolid;

		// if its a top block or bottom block we dont want to cull it
	default:
		return false;
	}
}


bool Chunk::CheckNeighborCull(int xPos, int zPos, int xIdx, int zIdx, int yIdx, Direction dir)
{
	Chunk* neighbor = GetNeighborChunk(xPos, zPos);

	// we dont cull if neighbor doesnt exist
	if (neighbor == nullptr)
		return false;

	bool isSolid = neighbor->GetBlockState(xIdx, yIdx, zIdx, dir);

	// possibly cull if neighbor exists and there is a block next to block we want to check
	return isSolid;
}


void Chunk::SetCulledFaces()
{
	const int vertsPerBlock = sizeof(BaseBlock) / sizeof(BaseBlock[0]);

	const int numFaces = 6;
	const int vertsPerFace = 4;

	// in the case we want to reset culled faces we reset numverts
	m_NumVerts = 0;


	for (int x = 0; x < CHUNK_SIZE; x++) {

		for (int z = 0; z < CHUNK_SIZE; z++) {

			int height = std::max(0,m_HeightMap[x][z]);

		

			for (int y = 0; y <= height + (-MAX_DEPTH); y++)
			{

				if (!m_BlockStates[x][z].test(y))
					continue;

				Direction dir = NORTH;
				for (int k = 0; k < numFaces; k++)
				{

					

					bool cull = false;

					verts* curVert = &m_AllChunkVerts[x][z][y][k* vertsPerFace];

					int wx = DIRECTION_VEC[k].x + x;
					int wy = DIRECTION_VEC[k].y + y;
					int wz = DIRECTION_VEC[k].z + z;

					// check bounds
					if ((wx >= 0 && wy >= 0 && wz >= 0) && (wx < CHUNK_SIZE && wz < CHUNK_SIZE && wy < (MAX_LEVELS)))
					{
						// if its air block then we want to put the vert into the culled chunk verts
						if (m_BlockStates[wx][wz].test(wy)) {
							cull = true;
						}

					}

					
					// else check neighboring chunk
					else
					{
						//cull = CheckNeighborCull(wx + m_ChunkX, wz + m_ChunkZ, wx, wz, wy, dir);
					}



					if (!cull)
					{
						for (int v = 0; v < vertsPerFace ; v++)
						{
							verts& curVert = m_AllChunkVerts[x][z][y][ k * vertsPerFace + v];
							m_CulledChunkVerts[m_NumVerts++] = curVert;
						}
					}

					dir = static_cast<Direction>(dir + 1);

				}
			}
		}
	}
}


void Chunk::SetChunkBlocks(int offsetX, int offsetZ)
{

	

	const int vertsPerBlock = sizeof(BaseBlock) / sizeof(BaseBlock[0]);

	int index = 0;


	blockCoordGroup block = GrassCoords;


	for (int x = 0; x < CHUNK_SIZE; x++) {

		for (int z = 0; z < CHUNK_SIZE; z++) {

			float height = generatePerlinNoise(x + offsetX, z + offsetZ) + 5;

			m_HeightMap[x][z] = (int)height;

			int top = 0;

			if (height < 0)
				top = height + (-MAX_DEPTH) +  (-height);
			
			else
				top = height + (-MAX_DEPTH);


			int level = 0;

			for(int y = 0 ; y <= top ; y++) {

				int corner = BOTTOM_LEFT;
				int dir = NORTH;

				

				// if we are below sea level to begin with
				if (height < 0)
				{
					if (level >= top + height )
						block = WaterCoords;
					else if (level >= top + height - 3)
						block = DirtCoords;
					else
						block = StoneCoords;
				}

				else
				{
					if (level == top  )
						block = GrassCoords;
					else if (level >= top - 2)
						block = DirtCoords;
					else
						block = StoneCoords;
				}

				level++;

				for (int k = 0; k < vertsPerBlock; k++)
				{

					float worldX = BaseBlock[k].vec.x + (float)x + (float)offsetX;
					float worldZ = BaseBlock[k].vec.z + (float)z + (float)offsetZ;

					index = vertsPerBlock * m_NumBlocks + k;


					m_AllChunkVerts[x][z][y][k] = { {worldX, BaseBlock[k].vec.y + y , worldZ},\
							{BaseBlock[k].textPos.x, BaseBlock[k].textPos.y},\
							BaseBlock[k].faceIndex };

					switch (dir)
					{


					case UP:
						SetFaceTextureCoord(block.top, corner, &m_AllChunkVerts[x][z][y][k]);
						break;

					case DOWN:
						SetFaceTextureCoord(block.bottom, corner, &m_AllChunkVerts[x][z][y][k]);
						break;

					default:
						SetFaceTextureCoord(block.side, corner, &m_AllChunkVerts[x][z][y][k]);
						break;

					}

					corner = (corner + 1) % 4;


					if ((k + 1) % 4 == 0) 
						dir++;

					
				}

				m_BlockStates[x][z].set(y);
				m_NumBlocks++;

			}
		}
	}

}


void Chunk::UpdateRenderStatus(glm::vec3 &playerPos, GLuint renderDist)
{

	int x = playerPos.x / CHUNK_SIZE - m_ChunkX / CHUNK_SIZE;
	int z = playerPos.z / CHUNK_SIZE - m_ChunkZ / CHUNK_SIZE;

	double dist = sqrt(x * x + z * z);

	if (dist > renderDist)
		m_IsActive = false;
	else 
		m_IsActive = true;
	
}

// this is to destroy chunk at the end when we exit, not when we leave render dist
void Chunk::DestroyChunkEnd()
{
	delete[] m_CulledChunkVerts;
	delete[] m_ChunkIndices;

	// buffer objects have default desutrctors
}

