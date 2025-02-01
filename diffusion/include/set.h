/**
 * @file set.h
 *
 * A simple set implementation, based on hash.h
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

#ifndef _diffusion_set_h_
#define _diffusion_set_h_ 1

#include "hash.h"

/**
 * This structure represents an entry in the set.
 */
typedef struct set_entry_s {
        /// The value stored in this set entry.
        void *val;

        /// If multiple values have the same hash, this is a pointer to
        /// the next possible matching entry.
        struct set_entry_s *next;
} SET_ENTRY_T;

/**
 * This respresents a set.
 */
typedef struct set_s {
        /// The maximum number of available slots in this set.
        unsigned long slots;

        /// The number of items held in this set.
        unsigned long size;

        /// An array of entries assigned to slots.
        SET_ENTRY_T **entries;

        /// Function used to duplicate values as they're added to the
        /// set. If NULL the values are not duplicated, and the user
        /// responsible for memory management.
        void *(*dup_fn)(const void *);

        /// Function used to free values from the set. If NULL, the
        /// user is responsible for memory management. Typically used
        /// if dup_fn has been provided.
        void (*free_fn)(void *);

        /// Function used to calculate the hash for the value. If
        /// NULL, a hash function is used which works with
        /// NULL-terminated char arrays.
        unsigned long (*hash_fn)(const void *);

        /// Function used to compare if two entries have the same value.
        /// Two different values may produce the same hash and both need
        /// to be stored; supplying this function allows this.
        int (*cmp_fn)(const void *, const void *);
} SET_T;

/**
 * Create a new set with the given number of slots.
 *
 * @param slots         The number of slots available in the set. Values hash to a slot,
 *                      and if a slot already contains a value which yields the same hash,
 *                      it is chained to other entries in the bucket.
 * @retval "SET_T *"    Returns a pointer to a SET_T structure.
 * @retval NULL         If the set cannot be created.
 */
SET_T *set_new(const unsigned long slots);

SET_T *set_new_ex(const unsigned long slots,
                  void *(*dup_fn)(const void *),
                  void (*free_fn)(void *),
                  unsigned long (*hash_fn)(const void *),
                  int (*cmp_fn)(const void *, const void *));

/**
 * Create a new set which holds strings (pointers to char).
 */
SET_T *set_new_string(const unsigned long slots);

/**
 * Create a new set which holds pointers to integers.
 */
SET_T *set_new_int(const unsigned long slots);

/**
 * Helper to add integers to sets.
 */
void *set_add_int(SET_T *set, const int val);

/**
 * @brief Frees memory (including values) associated with a set.
 *
 * This function can free all memory associated with a set.
 *
 * @param set           The set to be freed.
 */
void set_free(SET_T *set);

/**
 * Add a value to a set.
 *
 * If the set already contains the value, a pointer to the value
 * in the set is returned.
 *
 * @param set           The set to which the value will be added.
 * @param val           The value to store.
 * @retval "void *"     If the value already exists in the set.
 * @retval NULL         If the value did not already exist in the set.
 */
void *set_add(SET_T *set, const void *val);

/**
 * Remove a value from the set.
 *
 * @param set           The set from which the value will be removed.
 * @param val           The value to be removed from the set.
 * @retval "void *"     The value which was removed. The caller should free
 *                      this value.
 * @retval NULL         If the value was not found.
 */
void *set_del(SET_T *set, const void *val);

/**
 * Test whether a set contains a given value.
 *
 * @param set           The set to be searched for the value.
 * @param val           The value which is to be searched for.
 * @retval "void *"     The value, if found.
 * @retval NULL         If the value was not found.
 */
void *set_contains(const SET_T *set, const void *val);

/**
 * Obtains all values currently in the set.
 *
 * @param set           The set to be inspected.
 *
 * @return              A NULL-terminated array of all values in the set.
 *                      Call free() on the array when it is no longer required;
 *                      values are pointers into the set and should not be freed.
 */
void **set_values(const SET_T *set);

/**
 * @brief Create a deep copy of a set.
 *
 * @param set          The set to copy.
 * @return             A copy of the set.
 */
SET_T *set_dup(const SET_T *set);

#endif
