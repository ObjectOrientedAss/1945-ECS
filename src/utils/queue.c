#include "queue.h"
#include <string.h>

struct Queue *createQueue()
{
	struct Queue *q = calloc(1, sizeof(struct Queue));
	if (q == NULL)
		return NULL;

	q->size = 0;
	q->head = q->tail = NULL;
	return q;
}

void enqueue(struct Queue *q, void *_data)
{
	if (q == NULL)
		printf("Enqueue Error: Queue can't be null");

	if (_data == NULL)
		printf("Enqueue Error: Data is NULL.");

	struct QueueData *d = calloc(1, sizeof(struct QueueData));
	d->data = _data;
	d->next = NULL;

	if (q->size == 0)  //First insertion
		q->head = q->tail = d;
	else
	{
		q->tail->next = d;
		q->tail = d;
	}

	q->size++;
}

void *dequeue(struct Queue *q)
{
	if(q == NULL)
		printf("Dequeue Error: Queue is NULL");

	if(isEmpty(q))
		printf("Dequeue Error: Queue is empty");

	void *temp = q->head->data;
	if (q->size == 1)
	{
		struct QueueData* oldHead = q->head;
		q->head = q->tail = NULL;
		free(oldHead);
	}
	else
	{
		struct QueueData* oldHead = q->head;
		q->head = q->head->next;
		free(oldHead);
	}

	q->size--;
	return temp;
}

void clearQueue(struct Queue *q)
{
	if (q == NULL)
	{
		fprintf(stderr, "clear() Queue can't be null");
		exit(-1);
	}

	while (!isEmpty(q))
	{
		struct QueueData *temp = q->head;
		q->head = q->head->next;
		free(temp);
		q->size--;
	}
}

size_t getSize(struct Queue *q)
{
	if (q == NULL)
	{
		fprintf(stderr, "getSize() Queue can't be null");
		exit(-1);
	}

	return q->size;
}

boolean isEmpty(struct Queue *q)
{
	if (q == NULL)
		return true;

	if (q->size == 0)
		return true;
	else
		return false;
}

void destroyQueue(struct Queue *q)
{
	clearQueue(q);
	free(q);
}