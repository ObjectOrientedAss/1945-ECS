#include "pools_engine.h"

struct PoolsEngine* StartPoolsEngine()
{
    struct PoolsEngine* poolsEngine = (struct PoolsEngine*)calloc(1, sizeof(struct PoolsEngine));
    poolsEngine->pools = aiv_vector_new();
    CreatePools(poolsEngine);
    return poolsEngine;
}

void CreatePools(struct PoolsEngine* engine)
{
    for (int i = 0; i < (int)e_family_last; i++)
    {
        queue* pool = createQueue();
        aiv_vector_add(engine->pools, pool);
    }
}

queue* GetPool(struct PoolsEngine* engine, EntityFamily poolType)
{
    queue* queue = aiv_vector_at(engine->pools, poolType);
    return queue;
}

void DestroyPoolsEngine(struct PoolsEngine* engine)
{
    for (uint i = 0; i < aiv_vector_size(engine->pools); i++)
    {
        queue* pool = (queue*)aiv_vector_at(engine->pools, i);
        destroyQueue(pool);
    }
    aiv_vector_destroy(engine->pools);
    free(engine);
}

struct PoolsEngine* PoolsReset(struct PoolsEngine* engine)
{
    DestroyPoolsEngine(engine);
    return StartPoolsEngine();
}

void Enqueue(struct PoolsEngine* engine, struct Entity* entity)
{
    queue* q = GetPool(engine, (int)entity->family);
    enqueue(q, entity);
}

struct Entity* Dequeue(struct PoolsEngine* engine, EntityFamily poolType)
{
    queue* q = GetPool(engine, poolType);
    return (struct Entity*)dequeue(q);
}