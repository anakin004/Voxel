#ifndef WORLD_H
#define WORLD_H

#include "Light.h"
#include "shaderClass.h"
#include "Thread.h"
#include <unordered_map>
#include <utility>  
#include "Chunk.h"

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
	}
};


class World {



public:

	World(int renderDist, glm::vec3& playerStartPosition );
	~World();

	void AddChunk();
	void Update(glm::vec3& playerPos);
	void HandleChunks(glm::vec3& playerPos);
	void RenderLight();

	//@param divide x and z by 16 before you getchunk, we arent using world coords for mapping
	static Chunk* GetChunk(int x, int z);
	

private:

	static std::vector<Chunk*> m_Chunks;
	static std::unordered_map <std::pair<int, int>, Chunk*, pair_hash> m_ChunkMap;
	
	ThreadPool pool;

	Light m_WorldLight;

	double m_ChunkAllocateTime;

	int m_RenderDistance;

};


#endif 