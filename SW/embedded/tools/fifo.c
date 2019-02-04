#include "fifo.h"

#define BUF(buf) ((char *)(buf))

/******************************************************************************/
void fifo_init(fifo_t * fifo, void * buffer, size_t buffer_length)
{
    fifo->buffer = buffer;
    fifo->length = buffer_length;
    fifo->head = 0;
}

/******************************************************************************/
size_t fifo_free(const fifo_t * fifo)
{
    return fifo->length - fifo->head;
}

/******************************************************************************/
size_t fifo_enqueued(const fifo_t * fifo)
{
    return fifo->head;
}

/******************************************************************************/
size_t fifo_write(fifo_t * fifo, const void * buffer, size_t length)
{
    BUF(fifo->buffer)[fifo->head] = BUF(buffer)[0];
    fifo->head += length;
    return length;
}

/******************************************************************************/
size_t fifo_read(fifo_t * fifo, void * buffer, size_t length)
{
    return 0;
}