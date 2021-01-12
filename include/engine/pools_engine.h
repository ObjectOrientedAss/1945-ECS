#ifndef POOLS_ENGINE_H
#define POOLS_ENGINE_H

#include "queue.h"
#include "entity.h"

//the engine containing all the entity families pools to be used in game
struct PoolsEngine
{
    aiv_vector* pools;
};

//start and allocate the pools engine and the queues
struct PoolsEngine*__StartPoolsEngine();
//create an empty pool for each entity family
void __CreatePools(struct PoolsEngine* engine);
//get the pool of the requested entity family
struct Queue* GetPool(struct PoolsEngine* engine, EntityFamily poolFamily);
//stop and deallocate the pools engine and all the queues related
void __DestroyPoolsEngine(struct PoolsEngine* engine);
//destroy and recreate the pools engine and the queues. You should do it when you have to load a new scene after ECSreset.
struct PoolsEngine* PoolsReset(struct PoolsEngine* engine);
//enqueue an entity in its pool
void Enqueue(struct PoolsEngine* engine, struct Entity* entity);
//dequeue an entity from its pool
struct Entity* Dequeue(struct PoolsEngine* engine, EntityFamily poolFamily);

#endif //POOLS_ENGINE_H