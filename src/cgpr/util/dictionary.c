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
 * cg_dictionary_new
 ****************************************/

CGDictionary* cg_dictionary_new(void)
{
  CGDictionary* dictionaryList;

  dictionaryList = (CGDictionary*)malloc(sizeof(CGDictionary));

  if (NULL != dictionaryList) {
    cg_list_header_init((CGList*)dictionaryList);
    dictionaryList->key = NULL;
    dictionaryList->value = NULL;
  }

  return dictionaryList;
}

/****************************************
 * cg_dictionary_delete
 ****************************************/

void cg_dictionary_delete(CGDictionary* dictionaryList)
{
  cg_dictionary_clear(dictionaryList);
  free(dictionaryList);
}

/****************************************
 * cg_dictionary_getelement
 ****************************************/

CGDictionaryElement* cg_dictionary_getelement(CGDictionary* dir, const char* key)
{
  CGDictionaryElement* dirElem;
  char* dirElemKey;

  if (NULL == dir || cg_strlen(key) <= 0)
    return NULL;

  for (dirElem = cg_dictionary_gets(dir); dirElem != NULL; dirElem = cg_dictionary_element_next(dirElem)) {
    dirElemKey = cg_dictionary_element_getkey(dirElem);
    if (cg_strlen(dirElemKey) <= 0)
      continue;
    if (cg_streq(dirElemKey, key) == true)
      return dirElem;
  }

  return NULL;
}

/****************************************
 * cg_dictionary_setvalue
 ****************************************/

void cg_dictionary_setvalue(CGDictionary* dir, const char* key, const char* value)
{
  CGDictionaryElement* dirElem;

  dirElem = cg_dictionary_getelement(dir, key);
  if (NULL == dirElem) {
    dirElem = cg_dictionary_element_new();
    cg_dictionary_add(dir, dirElem);
  }
  cg_dictionary_element_setkey(dirElem, key);
  cg_dictionary_element_setvalue(dirElem, value);
}

/****************************************
 * cg_dictionary_getvalue
 ****************************************/

const char* cg_dictionary_getvalue(CGDictionary* dir, const char* key)
{
  CGDictionaryElement* dirElem;

  dirElem = cg_dictionary_getelement(dir, key);
  if (NULL == dirElem)
    return NULL;

  return cg_dictionary_element_getvalue(dirElem);
}
