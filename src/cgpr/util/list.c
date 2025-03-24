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

size_t cg_list_size(CGList* headList)
{
  CGList* list;
  int listCnt;

  if (!headList)
    return 0;

  listCnt = 0;
  for (list = cg_list_next(headList); list != NULL;
       list = cg_list_next(list))
    listCnt++;

  return listCnt;
}

/****************************************
 * cg_list_get
 ****************************************/

CGList* cg_list_get(CGList* headList, int index)
{
  CGList* list;
  int n;

  if (!headList)
    return NULL;

  list = cg_list_next(headList);
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

bool cg_list_insert(CGList* prevList, CGList* list)
{
  if (!prevList || !list)
    return false;

  list->prev = prevList;
  list->next = prevList->next;
  prevList->next->prev = list;
  prevList->next = list;

  return true;
}

/****************************************
 * cg_list_add
 ****************************************/

bool cg_list_add(CGList* headList, CGList* list)
{
  if (!headList || !list)
    return false;

  if (!headList->prev)
    return false;

  cg_list_insert(headList->prev, list);

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

CGList* cg_list_gets(CGList* headList)
{
  if (!headList)
    return NULL;

  return cg_list_next(headList);
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

bool cg_list_clear(CGList* headList,
    CG_LIST_DESTRUCTORFUNC dstructorFunc)
{
  CGList *list, *nextList;

  if (!headList)
    return false;

  list = cg_list_next(headList);
  while (list != NULL) {
    nextList = cg_list_next(list);
    cg_list_remove(list);
    if (dstructorFunc != NULL) {
      dstructorFunc(list);
    }
    else {
      free(list);
    }
    list = nextList;
  }

  return true;
}
