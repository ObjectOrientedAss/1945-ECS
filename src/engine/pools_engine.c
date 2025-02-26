#include "pools_engine.h"

struct PoolsEngine* __PoolsEngineInit()
{
    struct PoolsEngine* poolsEngine = (struct PoolsEngine*)calloc(1, sizeof(struct PoolsEngine));
    poolsEngine->pools = aiv_vector_new();
    __CreatePools(poolsEngine);
    return poolsEngine;
}

void __CreatePools(struct PoolsEngine* engine)
{
    for (int i = 0; i < (int)e_family_last; i++)
    {
        struct Queue* pool = createQueue();
        aiv_vector_add(engine->pools, pool);
    }
}

struct Queue* GetPool(struct PoolsEngine* engine, EntityFamily poolType)
{
    struct Queue* queue = aiv_vector_at(engine->pools, poolType);
    return queue;
}

void __DestroyPoolsEngine(struct PoolsEngine* engine)
{
    for (uint i = 0; i < aiv_vector_size(engine->pools); i++)
    {
        struct Queue* pool = aiv_vector_at(engine->pools, i);
        destroyQueue(pool);
    }
    aiv_vector_destroy(engine->pools);
    free(engine);
}

struct PoolsEngine* __PoolsReset(struct PoolsEngine* engine)
{
    __DestroyPoolsEngine(engine);
    return __PoolsEngineInit();
}

void Enqueue(struct PoolsEngine* engine, struct Entity* entity)
{
    struct Queue* q = GetPool(engine, (int)entity->family);
    enqueue(q, entity);
}

struct Entity* Dequeue(struct PoolsEngine* engine, EntityFamily poolType)
{
    struct Queue* q = GetPool(engine, poolType);
    return (struct Entity*)dequeue(q);
}