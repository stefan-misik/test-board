#ifndef FIFO_H_
#define FIFO_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief Structure for one FIFO circular buffer object
 */
typedef struct fifo
{
    void * buffer; /**< Pointer to the underlying buffer of the FIFO */
    size_t length; /**< The length of underlying buffer */
    size_t head;   /**< The position of the head of the FIFO */
    size_t tail;   /**< The position of the tail of the FIFO */
} fifo_t;

/**
 * @brief Initialize the FIFO buffer object
 * 
 * @param[out] fifo The object to be initialized
 * @param buffer The underlying buffer for the FIFO
 * @param buffer_length The length of the underlying buffer
 */
void fifo_init(fifo_t * fifo, void * buffer, size_t buffer_length);

/**
 * @brief Get the free space in the buffer
 * 
 * @param[in] fifo The object to be inspected
 * 
 * @return Number of free bytes in the buffer
 */
size_t fifo_free(const fifo_t * fifo);

/**
 * @brief Get the number of bytes stored in the FIFO
 * 
 * @param[in] fifo The object to be inspected
 * 
 * @return Number of bytes in the FIFO
 */
size_t fifo_enqueued(const fifo_t * fifo);

/**
 * @brief Write provaided data into the FIFO
 * 
 * @param[in,out] fifo FIFO to be written to
 * @param[in] buffer Data to be written
 * @param length Length of the data to be written in bytes
 * 
 * @return Number of the bytes actually written into the FIFO
 */
size_t fifo_write(fifo_t * fifo, const void * buffer, size_t length);

/**
 * @brief Read data from the FIFO up to the provided number of bytes
 * 
 * @param[in,out] fifo FIFO to be read from
 * @param[out] buffer Buffer to read data into
 * @param length Maximum length of the data to be read in bytes
 * 
 * @return Number of the bytes actually read into the buffer
 */
size_t fifo_read(fifo_t * fifo, void * buffer, size_t length);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FIFO_H_ */