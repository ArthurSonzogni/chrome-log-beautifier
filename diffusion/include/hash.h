/**
 * @file hash.h
 *
 * A simple hashmap implementation.
 *
 * Copyright © 2014, 2015 Push Technology Ltd., All Rights Reserved.
 *
 * Use is subject to license terms.
 *
 * NOTICE: All information contained herein is, and remains the
 * property of Push Technology. The intellectual and technical
 * concepts contained herein are proprietary to Push Technology and
 * may be covered by U.S. and Foreign Patents, patents in process, and
 * are protected by trade secret or copyright law.
 */

#ifndef _diffusion_hash_h_
#define _diffusion_hash_h_ 1

#include <apr_portable.h>

/**
 * This structure represents an entry within a hash table.
 */
typedef struct hash_entry_s {
        /// The key for this hash entry.
        char *key;

        /// The value associated with this hash entry.
        void *val;

        /// If multiple keys have the same hash, this is a pointer to
        /// the next possible matching entry.
        struct hash_entry_s *next;
} HASH_ENTRY_T;

/**
 * This represents a hash table.
 */
typedef struct hash_s {
        /// The maximum number of available slots in this hash table.
        unsigned long slots;

        /// The number of items held by this hash table.
        unsigned long size;

        /// An array of entries assigned to slots.
        HASH_ENTRY_T **entries;

        /// Mutex protecting hash modification/access.
        apr_pool_t *pool;
        apr_thread_mutex_t *mutex;
} HASH_T;

/**
 * Create a new hashmap with the given number of slots.
 *
 * @param slots         The number of slots available in the hashmap. Keys hash to a slot,
 *                      and if a slot already contains a key which yields the same hash,
 *                      it is chained to other entries in the bucket.
 * @retval "HASH_T *"   Returns a pointer to a HASH_T structure.
 * @retval NULL         If the hash cannot be created.
 */
HASH_T *hash_new(const unsigned long slots);

/**
 * Create a new unsynchronized hashmap with the given number of slots.
 *
 * @param slots         The number of slots available in the hashmap. Keys hash to a slot,
 *                      and if a slot already contains a key which yields the same hash,
 *                      it is chained to other entries in the bucket.
 * @retval "HASH_T *"   Returns a pointer to a HASH_T structure.
 * @retval NULL         If the hash cannot be created.
 */
HASH_T *unsync_hash_new(const unsigned long slots);

/**
 * @brief Clears all keys and values from a hashmap.
 *
 * Frees all the keys and values in a HASH_T, but does not free the hash
 * itself.
 *
 * @param hash          The hashmap to be freed.
 * @param key_free_fn   A function to be used to free memory associated with the key, or
 *                      NULL if the keys should not be freed.
 * @param val_free_fn   A function to be used to free memory associated with the value, or
 *                      NULL if the values should not be freed.
 */
void hash_clear(HASH_T *hash, void (*key_free_fn)(void *), void(*val_free_fn)(void *));

/**
 * @brief Frees memory (including keys) associated with a hashmap.
 *
 * This function can free all memory associated with a hashmap.
 *
 * @param hash          The hashmap to be freed.
 * @param key_free_fn   A function to be used to free memory associated with the key, or
 *                      NULL if the keys should not be freed.
 * @param val_free_fn   A function to be used to free memory associated with the value, or
 *                      NULL if the values should not be freed.
 */
void hash_free(HASH_T *hash, void (*key_free_fn)(void *), void(*val_free_fn)(void *));

/**
 * Add a value to a hashmap with the given key.
 *
 * If the hashmap already contains the key, the value is replaced and
 * the old value returned. In this case, the key in the hashmap is
 * reused; you may need to free() the key that was passed.
 *
 * @param hash          The hashmap to which the key/value pair will be added.
 * @param key           The key under which to store the value.
 * @param val           The value stored under the key.
 * @retval "void *"     If the key already exists in the hashmap the previous value is returned.
 * @retval NULL         If the key cannot be found in the hashmap.
 */
void *hash_add(HASH_T *hash, const char *key, const void *val);

/**
 * Remove a value from the hashmap
 *
 * @param hash          The hashmap from which the key/value pair will be removed.
 * @param key           The key for the entry which is to be removed.
 * @retval "void *"     The value which was removed.
 * @retval NULL         If the key was not found.
 */
void *hash_del(HASH_T *hash, const char *key);

/**
 * Get a value from the hashmap
 *
 * @param hash          The hashmap to be searched for the key.
 * @param key           The key for which the value is to be returned.
 * @retval "void *"     The value in the hashmap associated with the key, or NULL if not found.
 * @retval NULL         If the key was not found.
 */
void *hash_get(const HASH_T *hash, const char *key);

/**
 * Obtains all keys currently in the hashmap.
 *
 * @param hash          The hashmap to be inspected.
 *
 * @return              A NULL-terminated array of all keys in the hashmap.
 *                      Call free() on the array when it is no longer required;
 *                      keys are pointers to the value in the hash and should
 *                      not be freed.
 */
char **hash_keys(const HASH_T *hash);

/**
 * @brief Create a deep copy of a hashmap, where dup_fn() is the function
 * used to duplicated the hash value. (Hash keys are assumed to be strings).
 *
 * @param src          The hashmap to copy.
 * @param dup_fn       The function used to copy the hash value.
 *
 * @return             A copy of the hashmap.
 */
HASH_T *hash_dup(const HASH_T *src, void *(*dup_fn)(void *));

/**
 * Create a deep copy of a hashmap, assuming that the values are NULL-terminated strings.
 *
 * @param src           The hashmap to copy.
 * @return              A copy of the hashmap.
 */
HASH_T *hash_dup_strval(const HASH_T *src);

#endif
