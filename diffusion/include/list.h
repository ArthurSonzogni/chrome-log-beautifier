/**
 * @file list.h
 *
 * A simple linked list implementation.
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

#ifndef _list_h_
#define _list_h_ 1

/**
 * Structure representing a node in a linked list.
 */
typedef struct list_node_s {
        /// Pointer to the previous node.
        struct list_node_s *prev;
        /// Pointer to the next node.
        struct list_node_s *next;
        /// Node content.
        void *data;
} LIST_NODE_T;

/**
 * A structure defining a linked list.
 */
typedef struct list_s {
        /// Pointer to the first node in the list.
        LIST_NODE_T *first;
        /// Pointer to the last node in the list.
        LIST_NODE_T *last;
} LIST_T;

/**
 * Create a new linked list.
 * 
 * @retval "LIST_T *"	Returns a pointer to a LIST_T structure.
 */
LIST_T *list_create(void);

/**
 * @brief Copy a list.
 *
 * @param list The list to be copied.
 * @param copy_fn A function to be used to copy the data in each node.
 *
 * @retval "LIST_T *"   A copy of the list.
 */
LIST_T *list_dup(const LIST_T *list, void *(*copy_fn)(void *));

/**
 * Free memory allocated to a linked list.
 *
 * @param list		The list to free
 * @param free_fn	A function used to free the contents of a list node
 *			contents, or NULL if the contents are not to be freed.
 */
void list_free(LIST_T *list, void(*free_fn)(void *));

/**
 * Insert a node before another in the list.
 * 
 * @param list		The list in which to insert the node.
 * @param where		The node to insert before.
 * @param data		The data to be contained in the new node.
 */
void list_insert(LIST_T *list, LIST_NODE_T *where, void *data);

/**
 * Append a node after another in the list.
 * 
 * @param list		The list in which to append the node.
 * @param where		The node to append after.
 * @param data		The data to be contained in the new node.
 */
void list_append(LIST_T *list, LIST_NODE_T *where, void *data);

/**
 * Insert a node into the first position in the list.
 *
 * @param list		The list in which to insert the node.
 * @param data		The data to be contained in the new node.
 */
void list_insert_first(LIST_T *list, void *data);

/**
 * Append a node to the last position in the list.
 *
 * @param list		The list in which to append the node.
 * @param data		The data to be contained in the new node.
 */
void list_append_last(LIST_T *list, void *data);

/**
 * Return the node data of the first element in the list, or NULL if not
 * possible.
 *
 * @param list		The list to be accessed.
 * @retval "void *"	The data contained in the first list node, or NULL.
 */
void *list_get_data_first(const LIST_T *list);

/**
 * Return the node data of the last element in the list, or NULL if not
 * possible.
 *
 * @param list		The list to be accessed.
 * @retval "void *"	The data contained in the last list node, or NULL.
 */
void *list_get_data_last(const LIST_T *list);

/**
 * Return the node data of the nth element in the list, or NULL if not
 * possible.
 * 
 * @param list		The list to be accessed.
 * @param index		The index of the node in the list to access.
 * @retval "void *"	The data contained in the list node with the given
 *			index, or NULL.
 */
void *list_get_data_indexed(const LIST_T *list, const int index);

/**
 * Appends list src to list dst. Does not copy any data, but links the
 * two lists together.
 *
 * @param dst            The list to be appended to.
 * @param src            The list to append.
 */
void list_append_list(LIST_T *dst, const LIST_T *src);

#endif
