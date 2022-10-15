#include "dir.h"
#include "queue.h"
#include "error.h"

#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <assert.h>

/*
 * This function perform file path concatenation
 */

static char *concat(const char *left, const char *right, size_t size)
{
    if (!left || !right)
    {
        errno = EINVAL;
        return NULL;
    }

    char *buff = calloc(size + 2, sizeof(*concat));
    if (!buff)
        return NULL;

    size_t i = 0;
    for (; i < size + 1 && *left; i++, left++)
        buff[i] = *left;
    if (i > 0 && i < size + 1 && buff[i-1] != '/')
        buff[i++] = '/';
    for (; i < size + 1 && *right; i++, right++)
        buff[i] = *right;
    
    if (buff[i-1] != '/')
        buff[i++] = '/';
    
    if (i == size && *right)
    {
        free(buff);
        errno = ENOMEM;
        return NULL;
    }

    return buff;
}

/*
 * This function call file_cb for all files in directory dirname 
 */

static int dir_process(const char *dirname, struct queue *queue, 
                       void (*file_cb) (const char *dirname, const char *fname, void *cxt), 
                       void *cxt)
{
    if (!dirname || !queue || !file_cb)
    {
        errno = EINVAL;
        return -errno;
    }

    DIR *dir = opendir(dirname);
    if (!dir)
    {
        ERROR("Can't scan directory \"%s\"", dirname);
        errno = 0;
        return 0;
    }

    do
    {
        errno = 0;
        struct dirent *ent = readdir(dir);
        if (!ent)
            break;

        assert(ent->d_type != DT_UNKNOWN);
        
        /*
         * Call file_cb for regular files
         */
        if (ent->d_type == DT_REG)
            file_cb(dirname, ent->d_name, cxt);

        /*
         * Add directories to the queue for future processing
         */
        if (ent->d_type == DT_DIR && 
            strncmp(ent->d_name, ".", NAME_SIZE) && 
            strncmp(ent->d_name, "..", NAME_SIZE))
        {
            char *name = concat(dirname, ent->d_name, NAME_SIZE);
            if (!name)
                break;
            if (queue_add(queue, name) == -1)
            {
                free(name);
                break;
            }
        }
    }
    while(1);

    closedir(dir);
    
    return -errno;
}

int dir_scan(const char *start, 
            void (*file_cb)(const char *dirname, const char *fname, void *cxt), void *cxt)
{
    if (!start)
    {
        errno = EINVAL;
        return -errno;
    }

    /*
     * Creation of a queue of directories for processing
     */

    struct queue *queue = queue_create();
    if (!queue)
        return -errno;

    /*
     * Addition '/' to the start point 
     */

    char *dir = concat(start, "", NAME_SIZE);
    if (!dir)
    {
        queue_delete(queue);
        return -errno;
    }

    do
    {
        /* 
         * Processing each directory 
         */ 

        int ret = dir_process(dir, queue, file_cb, cxt);
        free(dir);
        if (ret < 0)
            break;

        /*
         * Getting new directory from the queue
         */

        if (!(dir = queue_get(queue)))
            break;
    }
    while(1);
    
    /*
     * Free resources 
     */

    void *to_free = NULL;
    while((to_free = queue_get(queue)))
        free(to_free);

    queue_delete(queue);

    return -errno;
}
