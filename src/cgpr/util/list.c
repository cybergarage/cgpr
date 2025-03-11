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

#include <cgpr/util/list.h>

/****************************************
 * cg_list_header_init
 ****************************************/

bool cg_list_header_init(CGList* list)
{
  if (!list)
    return false;

  list->headFlag = true;
  list->prev = list->next = list;

  return true;
}

/****************************************
 * cg_list_node_init
 ****************************************/

bool cg_list_node_init(CGList* list)
{
  if (!list)
    return false;

  list->headFlag = false;
  list->prev = list->next = list;

  return true;
}

/****************************************
 * cg_list_size
 ****************************************/

size_t cg_list_size(CGList* head_list)
{
  CGList* list;
  int list_cnt;

  if (!head_list)
    return 0;

  list_cnt = 0;
  for (list = cg_list_next(head_list); list != NULL;
      list = cg_list_next(list))
    list_cnt++;

  return list_cnt;
}

/****************************************
 * cg_list_get
 ****************************************/

CGList* cg_list_get(CGList* head_list, int index)
{
  CGList* list;
  int n;

  if (!head_list)
    return NULL;

  list = cg_list_next(head_list);
  for (n = 0; n < index; n++) {
    if (!list)
      break;
    list = cg_list_next(list);
  }

  return list;
}

/****************************************
 * cg_list_insert
 ****************************************/

bool cg_list_insert(CGList* prev_list, CGList* list)
{
  if (!prev_list || !list)
    return false;

  list->prev = prev_list;
  list->next = prev_list->next;
  prev_list->next->prev = list;
  prev_list->next = list;

  return true;
}

/****************************************
 * cg_list_add
 ****************************************/

bool cg_list_add(CGList* head_list, CGList* list)
{
  if (!head_list || !list)
    return false;

  if (!head_list->prev)
    return false;

  cg_list_insert(head_list->prev, list);

  return true;
}

/****************************************
 * cg_list_remove
 ****************************************/

bool cg_list_remove(CGList* list)
{
  if (!list)
    return false;

  if (!list->prev || !list->next)
    return false;

  list->prev->next = list->next;
  list->next->prev = list->prev;
  list->prev = list->next = list;

  return true;
}

/****************************************
 * cg_list_prev_circular
 ****************************************/

CGList* cg_list_prev_circular(CGList* list)
{
  if (!list)
    return NULL;

  if (!list->prev)
    return NULL;

  if (list->prev->headFlag)
    list = list->prev;

  return list->prev;
}

/****************************************
 * cg_list_prev
 ****************************************/

CGList* cg_list_prev(CGList* list)
{
  if (!list)
    return NULL;

  if (!list->prev)
    return NULL;

  if (list->prev->headFlag)
    return NULL;

  return list->prev;
}

/****************************************
 * cg_list_next_circular
 ****************************************/

CGList* cg_list_next_circular(CGList* list)
{
  if (!list)
    return NULL;

  if (!list->next)
    return NULL;

  if (list->next->headFlag == true)
    list = list->next;

  return list->next;
}

/****************************************
 * cg_list_gets
 ****************************************/

CGList* cg_list_gets(CGList* head_list)
{
  if (!head_list)
    return NULL;

  return cg_list_next(head_list);
}

/****************************************
 * cg_list_next
 ****************************************/

CGList* cg_list_next(CGList* list)
{
  if (!list)
    return NULL;

  if (!list->next)
    return NULL;

  if (list->next->headFlag == true)
    return NULL;

  return list->next;
}

/****************************************
 * cg_list_clear
 ****************************************/

bool cg_list_clear(CGList* head_list,
    CG_LIST_DESTRUCTORFUNC dstructor_func)
{
  CGList *list, *next_list;

  if (!head_list)
    return false;

  list = cg_list_next(head_list);
  while (list != NULL) {
    next_list = cg_list_next(list);
    cg_list_remove(list);
    if (dstructor_func != NULL) {
      dstructor_func(list);
    }
    else {
      free(list);
    }
    list = next_list;
  }

  return true;
}
