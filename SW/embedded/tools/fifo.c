#include "fifo.h"

#define BUF(buf) ((char *)(buf))

/******************************************************************************/
void fifo_init(fifo_t * fifo, void * buffer, size_t buffer_length)
{
    fifo->buffer = buffer;
    fifo->length = buffer_length;
    fifo->head = 0;
    fifo->tail = 0;
}

/******************************************************************************/
size_t fifo_free(const fifo_t * fifo)
{
    return fifo->length - (fifo->head - fifo->tail) - 1;
}

/******************************************************************************/
size_t fifo_enqueued(const fifo_t * fifo)
{
    return fifo->head - fifo->tail;
}

/******************************************************************************/
size_t fifo_write(fifo_t * fifo, const void * buffer, size_t length)
{
    size_t pos;
    for (pos = 0; pos < length; pos ++)
    {
        BUF(fifo->buffer)[fifo->head ++] = BUF(buffer)[pos];
    }
    return pos;
}

/******************************************************************************/
size_t fifo_read(fifo_t * fifo, void * buffer, size_t length)
{
    size_t pos;
    for (pos = 0; pos < length; pos ++)
    {
        BUF(buffer)[pos] = BUF(fifo->buffer)[fifo->tail ++];
    }
    return pos;
}