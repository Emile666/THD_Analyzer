/*==================================================================
  File Name    : ring_buffer.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This is the header-file that defines all functions and
            structures for working with ring-buffers. These ring-buffers
	    are primarily used for usart ISR driven communication.
  ==================================================================*/ 
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>

struct ring_buffer 
{
    volatile uint8_t write_offset;
    volatile uint8_t read_offset;
    uint8_t          size;
    uint8_t          *buffer;
}; /* ring_buffer */

/*-----------------------------------------------------------------------------
  Purpose  : Get the next offset in a ring buffer.
  Variables: cur_offset: the current offset in the ring buffer
             max_offset: the highest allowed offset in the ring buffer
  Returns  : next offset or 0 if we are wrapping
  ---------------------------------------------------------------------------*/
static inline uint8_t get_next(uint8_t cur_offset, uint8_t max_offset)
{
   return ((cur_offset == max_offset-1) ? 0 : cur_offset + 1);
} /* get_next() */

/*-----------------------------------------------------------------------------
  Purpose  : Get the next write offset in a ring buffer
  Variables: ring: pointer to a struct of type ring_buffer
  Returns  : the next write offset in the ring buffer
  ---------------------------------------------------------------------------*/
static inline uint8_t ring_buffer_get_next_write(const struct ring_buffer *ring)
{
    return get_next(ring->write_offset, ring->size);
} /* ring_buffer_get_next_write() */

/*-----------------------------------------------------------------------------
  Purpose  : Get the next read offset in a ring buffer
  Variables: ring: pointer to a struct of type ring_buffer
  Returns  : the next read offset in the ring buffer
  ---------------------------------------------------------------------------*/
static inline uint8_t ring_buffer_get_next_read(const struct ring_buffer *ring)
{
    return get_next(ring->read_offset, ring->size);
} /* ring_buffer_get_next_read() */

/*-----------------------------------------------------------------------------
  Purpose  : Function for checking if the ring buffer is full.
  Variables: ring: pointer to a struct of type ring_buffer
  Returns  : true: buffer is full ; false: space is available in the ring buffer
  ---------------------------------------------------------------------------*/
static inline bool ring_buffer_is_full(const struct ring_buffer *ring)
{
    return (ring->read_offset == ring_buffer_get_next_write(ring));
} /* ring_buffer_is_full() */

/*-----------------------------------------------------------------------------
  Purpose  : Function for checking if the ring buffer is empty
  Variables: ring: pointer to a struct of type ring_buffer
  Returns  : true: buffer is empty ; false: there is still data in the ring buffer
  ---------------------------------------------------------------------------*/
static inline bool ring_buffer_is_empty(const struct ring_buffer *ring)
{
    uint8_t r = ring->read_offset;
    return (r == ring->write_offset);
} /* ring_buffer_is_empty() */

/*-----------------------------------------------------------------------------
  Purpose  : Function for initializing a ring buffer
  Variables: buffer: pointer to the buffer to use as a ring buffer
             size  : the size of the ring buffer
  Returns  : struct containing the ring buffer
  ---------------------------------------------------------------------------*/
static inline struct ring_buffer ring_buffer_init(uint8_t *buffer, uint8_t size)
{
    struct ring_buffer ring;
    ring.write_offset = 0;
    ring.read_offset  = 0;
    ring.size         = size;
    ring.buffer       = buffer;
    return ring;
} /* ring_buffer_init() */

/*-----------------------------------------------------------------------------
  Purpose  : Function for getting one byte from the ring buffer
             Make sure buffer is not empty (using ring_buffer_is_empty) 
             before calling this function.
  Variables: ring: pointer to a struct of type ring_buffer
  Returns  : next byte in buffer
  ---------------------------------------------------------------------------*/
static inline uint8_t ring_buffer_get(struct ring_buffer *ring)
{
    //Assert(!ring_buffer_is_empty(ring));
    uint8_t data = ring->buffer[ring->read_offset];
    ring->read_offset = ring_buffer_get_next_read(ring);
    return data;
} /* ring_buffer_get() */

/*-----------------------------------------------------------------------------
  Purpose  : Function for putting a data byte in the ring buffer. 
             Make sure buffer is not full (using ring_buffer_is_full) 
             before calling this function.
  Variables: ring: pointer to a struct of type ring_buffer
             data: the byte to put into the buffer
  Returns  : -
  ---------------------------------------------------------------------------*/
static inline void ring_buffer_put(struct ring_buffer *ring, uint8_t data)
{
    //Assert(!ring_buffer_is_full(ring));   
    ring->buffer[ring->write_offset] = data;
    ring->write_offset = ring_buffer_get_next_write(ring);
} /* ring_buffer_put() */

#endif /* RING_BUFFER_H */
