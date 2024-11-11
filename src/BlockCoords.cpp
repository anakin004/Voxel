#include "BlockCoords.h"


// some globals for the different types of blocks im using
// we only need to define these once as they will be used many times
// threads will access these when initlizing chunks, they are const, and reading is thread safe
// also when placing blocks too we will use these

const Block Grass = Block(1, 15, 0, 15, 2, 15, 16, 256);
const Block Dirt = Block(2, 15, 2, 15, 2, 15, 16, 256);
const Block Water = Block(0, 0, 1, 0, 2, 0, 16, 256);
const Block Stone = Block(3, 15, 3, 15, 3, 15, 16, 256);

const blockCoordGroup GrassCoords = Grass.getBlockCoords();
const blockCoordGroup DirtCoords = Dirt.getBlockCoords();
const blockCoordGroup WaterCoords = Water.getBlockCoords();
const blockCoordGroup StoneCoords = Stone.getBlockCoords();