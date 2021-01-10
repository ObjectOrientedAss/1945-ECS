#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "common.h"

struct QueueData
{
	void * data;
	struct QueueData * next;
};

struct Queue
{
	size_t size;
	//size_t allocationSize;
	struct QueueData* head;
	struct QueueData* tail;
};

/*
Create and return an empty queue
*/
struct Queue* createQueue(/*size_t allocSize*/);
/*
Insert data into the queue(last position)
*/
void enqueue(struct Queue * q, void* data);
/*
Remove first element of the queue of save its value to the toRet argument
*/
void* dequeue(struct Queue * q);
/*
Save first element of the queue to the toRet argument
*/
void front(struct Queue*q, void *toRet);//Return the first element
/*
Deletes all data of the queue
*/
void clearQueue(struct Queue* q);
/*
Clears and destoys the queue
*/
void destroyQueue(struct Queue *q);
/*
Return size of the queue
*/
size_t getSize(struct Queue *q);
/*
Check is queue is empty
*/
boolean isEmpty(struct Queue * q);


#endif