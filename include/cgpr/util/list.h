/******************************************************************
 *
 * Copyright (C) 2025 The Cyber Garage Portable Runtime for C Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef _CGPR_UTIL_LIST_H_
#define _CGPR_UTIL_LIST_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>

#include <cgpr/util/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

/**
 * Prototype for individual list node destructor functions
 *
 */
typedef void (*CG_LIST_DESTRUCTORFUNC)(void*);

/**
 * Macro for list node members
 *
 */

#define CG_LIST_STRUCT_MEMBERS             \
  /** Marks the beginning of a list */     \
  bool headFlag;                           \
  /** Pointer to the next list node */     \
  struct _CGList* prev;                    \
  /** Pointer to the previous list node */ \
  struct _CGList* next;

/****************************************
 * Data Type
 ****************************************/

/**
 * \brief The generic list interface in mUPnP.
 *
 * Each struct in mUPnP, that is designed to be a part of
 * a list, must have these elements in their definition before the
 * actual struct definition. The struct pointers are then cast to
 * CGList* and operated with cg_list_* functions.
 */
typedef struct _CGList {
  CG_LIST_STRUCT_MEMBERS
} CGList;

/****************************************
 * Functions
 ****************************************/

/**
 * Initialize a list node to act as the first node
 *
 * \param list List node
 */
bool cg_list_header_init(CGList* list);

/**
 * Initialize a list node to act as a regular node, part of a list.
 *
 * \param list List node
 */
bool cg_list_node_init(CGList* list);

/**
 * Insert a list node or a complete list structure after the given node
 *
 * \param prevList Insert after this node
 * \param list List node or list structure to insert
 */
bool cg_list_insert(CGList* prevList, CGList* list);

/**
 * \todo This works essentially like insert, although it is more error-prone?!
 * \todo There might be a bug in this function.
 *
 */
bool cg_list_add(CGList* headList, CGList* list);

/**
 * Remove a node from a list. Does not free any memory, but only removes
 * the next and previous link associations regarding the given node.
 *
 * \param list List node to remove
 */
bool cg_list_remove(CGList* list);

/**
 * Get the number of nodes in the current list structure. Counts forwards from the given
 * node, so if you want to get the complete size, give a header node as the parameter.
 *
 * \param headList List header
 */
size_t cg_list_size(CGList* headList);

/**
 * Get an item from the list by the item's index
 *
 * \param headList List header
 * \param index The index of the item to get
 */
CGList* cg_list_get(CGList* headList, int index);

/**
 * Get the previous node. Wrap around if the beginning has been reached.
 *
 *
 * \param list Current node
 */
CGList* cg_list_prev_circular(CGList* list);

/**
 * Get the previous node. Returns NULL if beginning has been reached
 *
 * \param list Current node
 */
CGList* cg_list_prev(CGList* list);

/**
 * Get the next node. Wrap around if the end has been reached.
 *
 * \param list Current node
 */
CGList* cg_list_next_circular(CGList* list);

/**
 * Get the next node. Returns NULL if end has been reached.
 *
 * \param list Current node
 */
CGList* cg_list_next(CGList* list);

/**
 * Clear the list and delete all of its contents with \ref CG_LIST_DESTRUCTORFUNC
 *
 * \param headList List header
 * \param destructorFunc Function pointer that clears the contents of individual nodes
 */
bool cg_list_clear(CGList* headList, CG_LIST_DESTRUCTORFUNC destructorFunc);

/**
 * Get the first actual item from a list for iteration
 *
 * \param headList List header
 */
CGList* cg_list_gets(CGList* headList);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // _CGPR_UTIL_LIST_H_
