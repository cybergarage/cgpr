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

#include <cgpr/util/dictionary.h>
#include <cgpr/util/log.h>

/****************************************
 * cg_dictionary_element_new
 ****************************************/

CGDictionaryElement* cg_dictionary_element_new(void)
{
  CGDictionaryElement* dirElem;

  dirElem = (CGDictionaryElement*)malloc(sizeof(CGDictionaryElement));

  if (NULL != dirElem) {
    cg_list_node_init((CGList*)dirElem);
    dirElem->key = cg_string_new();
    dirElem->value = cg_string_new();
  }

  return dirElem;
}

/****************************************
 * cg_dictionary_element_delete
 ****************************************/

bool cg_dictionary_element_delete(CGDictionaryElement* dirElem)
{
  cg_list_remove((CGList*)dirElem);

  if (dirElem->key)
    cg_string_delete(dirElem->key);
  if (dirElem->value)
    cg_string_delete(dirElem->value);
  free(dirElem);

  return true;
}
