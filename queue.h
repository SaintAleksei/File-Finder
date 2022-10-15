#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED 1

/*
 * This simple data structure is used to store directories that should be processed
 */

/*
 * Constructor
 */

struct queue *queue_create(void);

/*
 * Destructor
 */

void queue_delete(struct queue *queue);

/*
 * Extract current value from queue
 */

void *queue_get(struct queue *queue);

/*
 * Add new value to queue
 */

int queue_add(struct queue *queue, void *data);

#endif
