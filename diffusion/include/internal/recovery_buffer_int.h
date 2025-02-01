#ifndef _recovery_buffer_int_h_
#define _recovery_buffer_int_h_ 1

/*
 * A buffer containing messages which have already been sent to the
 * server. These may be replayed on reconnection.
 */

#include "internal/queue_int.h"

typedef struct recovery_buffer_entry_s {
        int seqnum;
        void *data;
} RECOVERY_BUFFER_ENTRY_T;

typedef struct recovery_buffer_s {
        int seqnum;
        int size;
        int max_size;
        LIST_T *entry_list;
        void (*free_fn)(void *);
} RECOVERY_BUFFER_T;

/*
 * Create a new recovery buffer with a default size and a function to be use
 * to free data added with recovery_buffer_put().
 */
RECOVERY_BUFFER_T *recovery_buffer_create(int default_size, void(*free_fn)(void *));

/*
 * Frees memory associated with a recovery buffer (but not the memory for the contents).
 */
void recovery_buffer_free(RECOVERY_BUFFER_T *buffer);

/*
 * Add (append) an entry to a recovery buffer, removing the first entry if the
 * maximum size of the buffer has been reached.
 */
void recovery_buffer_put(RECOVERY_BUFFER_T *buffer, void *data);

/*
 * Gets the oldest entry from a recovery buffer and removes it. If not
 * NULL, seqnum is the address into which the sequence number of that
 * message is placed. Returns NULL if the buffer is empty.
 */
void *recovery_buffer_get(RECOVERY_BUFFER_T *buffer, int *seqnum);

/*
 * Gets the (oldest + offset) entry from a recovery buffer but does not remove
 * it. If not NULL, seqnum is the address into which the sequence number of
 * that message is placed. Returns NULL if the buffer is empty.
 */
void *recovery_buffer_peek(RECOVERY_BUFFER_T *buffer, int offset, int *seqnum);

/*
 * Removes all entries from a recovery buffer. free_fn (if not NULL) is a
 * function that is used to free the data field of a RECOVERY_BUFFER_ENTRY_T.
 */
void recovery_buffer_clear(RECOVERY_BUFFER_T *buffer);

/*
 * Clears all entries from the recovery buffer and also resets the buffer's
 * sequence counter, putting the buffer back into the same state as when it
 * was created (while retaining any max size adjustment).
 */
void recovery_buffer_reset(RECOVERY_BUFFER_T *buffer);

/*
 * Returns the number of entries in the recovery buffer. Over time, this will
 * reach the max size of the buffer and stay there.
 */
int recovery_buffer_count_entries(RECOVERY_BUFFER_T *buffer);

/*
 * Resizes a recovery buffer, discarding older entries first if the buffer
 * needs to be shrunk.
 */
void recovery_buffer_set_size(RECOVERY_BUFFER_T *buffer, int size);

/*
 * Return the sequence number of the oldest entry in the recovery buffer.
 */
int recovery_buffer_get_oldest_seq(RECOVERY_BUFFER_T *buffer);

/*
 * Return the sequence number of the newest entry in the recovery buffer.
 */
int recovery_buffer_get_newest_seq(RECOVERY_BUFFER_T *buffer);

#endif
