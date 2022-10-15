#include "queue.h"

#include <errno.h>
#include <stdlib.h>

struct queue
{
    struct node *first;
    struct node *last;
};

struct node
{
    struct node *next;
    void *data;
};

struct queue* queue_create(void)
{
    struct queue *queue = calloc(1, sizeof(*queue)); 
    
    return queue;
}

void queue_delete(struct queue *queue)
{
    if (!queue)
        return;

    struct node *cur = queue->first;
    while (cur != NULL)
    {
        void *to_free = cur;
        cur = cur->next;
        free(to_free);
    }

    free(queue);
}

void *queue_get(struct queue *queue)
{
    if (!queue || !queue->first)
    {
        return NULL;
    }

    void *to_return = queue->first->data;
    void *to_free = queue->first;

    queue->first = queue->first->next;

    if (queue->last == to_free)
        queue->last = NULL;

    free(to_free);

    return to_return;
}

int queue_add(struct queue *queue, void *data)
{
    if (!queue)
    {
        errno = EINVAL;
        return -1;
    }

    struct node *new = calloc(1, sizeof(*new));
    if (!new)
        return -1;

    new->data = data;

    if (queue->last)
        queue->last->next = new;

    if (!queue->first)
        queue->first = new;

    queue->last = new;

    return 0;
}
