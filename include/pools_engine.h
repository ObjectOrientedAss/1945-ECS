#ifndef POOLS_ENGINE_H
#define POOLS_ENGINE_H

#include "queue.h"
#include "entity.h"

struct PoolsEngine
{
    aiv_vector* pools;
};

struct PoolsEngine* StartPoolsEngine();
void CreatePools(struct PoolsEngine* engine);
struct Queue* GetPool(struct PoolsEngine* engine, EntityFamily poolFamily);
void DestroyPoolsEngine(struct PoolsEngine* engine);
struct PoolsEngine* PoolsReset(struct PoolsEngine* engine);
void Enqueue(struct PoolsEngine* engine, struct Entity* entity);
struct Entity* Dequeue(struct PoolsEngine* engine, EntityFamily poolFamily);

#endif //POOLS_ENGINE_H