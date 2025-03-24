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

#ifndef _CGPR_UTIL_DICTIONARY_H_
#define _CGPR_UTIL_DICTIONARY_H_

#include <cgpr/util/list.h>
#include <cgpr/util/string.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

/****************************************
 * Data Type
 ****************************************/

/**
 * \brief The generic wrapper struct for mUPnP's dictionarys.
 *
 * This wrapper has been created to enable 100% code
 * compatibility between different platforms (Linux, Win32 etc..)
 */
typedef struct _CGDictionary {
  CG_LIST_STRUCT_MEMBERS

  CGString* key;
  CGString* value;
} CGDictionaryElement, CGDictionary;

/****************************************
 * Function
 ****************************************/

/**
 * Create a new dictionary
 */
CGDictionaryElement* cg_dictionary_element_new(void);

/**
 * Destroy a dictionary.
 *
 * \param dirElem Dictionary to destroy
 */
bool cg_dictionary_element_delete(CGDictionaryElement* dirElem);

/**
 * Set a key
 *
 * \param dirElem Dictionary struct
 * \param name String to set
 */
#define cg_dictionary_element_setkey(dirElem, name) cg_string_setvalue(dirElem->key, name)
/**
 * Get a key
 *
 * \param dirElem Dictionary struct
 */
#define cg_dictionary_element_getkey(dirElem) cg_string_getvalue(dirElem->key)

/**
 * Set a value
 *
 * \param dirElem Dictionary struct
 * \param value String to set
 */
#define cg_dictionary_element_setvalue(dirElem, val) cg_string_setvalue(dirElem->value, val)
/**
 * Get a value
 *
 * \param dirElem Dictionary struct
 */
#define cg_dictionary_element_getvalue(dirElem) cg_string_getvalue(dirElem->value)

#define cg_dictionary_element_next(dirElem) (CGDictionaryElement*)cg_list_next((CGList*)dirElem)
#define cg_dictionary_element_remove(dirElem) cg_list_remove((CGList*)dirElem)

/****************************************
 * Function (Dictionary List)
 ****************************************/

/**
 * Create a new dictionary list
 *
 * \return Dictionary list
 */
CGDictionary* cg_dictionary_new(void);

/**
 * Destroy a dictionary list
 *
 * \param dir The dictionary list in question
 */
void cg_dictionary_delete(CGDictionary* dir);

/**
 * Clear the contents of a dictionary list
 *
 * \param dir Dictionary list in question
 */
#define cg_dictionary_clear(dir) cg_list_clear((CGList*)dir, (CG_LIST_DESTRUCTORFUNC)cg_dictionary_element_delete)

/**
 * Get the size of a dictionary list
 *
 * \param dir The dictionary list in question
 */
#define cg_dictionary_size(dir) cg_list_size((CGList*)dir)

/**
 * Get the first actual item from a dictionary list to use as an iterator
 *
 * \param dir The dictionary list in question
 */
#define cg_dictionary_gets(dir) (CGDictionaryElement*)cg_list_next((CGList*)dir)

/**
 * Add a dictionary into a dictionary list
 *
 * \param dir The dictionary list in question
 * \param dirElem The dictionary to add to the list
 */
#define cg_dictionary_add(dir, dirElem) cg_list_add((CGList*)dir, (CGList*)dirElem)

/**
 * Remove a dictionary from dictionary list
 *
 * \param dirElem The dictionary to be removed
 */
#define cg_dictionary_remove(dirElem) cg_list_remove((CGList*)dirElem)

/**
 * Get a element of the specified key
 *
 * \param dir The dictionary list in question
 * \param key Name to search.
 * \return Element of the specified key.
 */
CGDictionaryElement* cg_dictionary_get(CGDictionary* dir, const char* key);

/**
 * Set a new element into a dictionary
 *
 * \param dir The dictionary list in question
 * \param key Name to set.
 * \param value Value to set.
 */
void cg_dictionary_setvalue(CGDictionary* dir, const char* key, const char* value);

/**
 * Get a value of the specified key
 *
 * \param dir The dictionary list in question
 * \param key Name to search.
 * \return Value of the specified key.
 */
const char* cg_dictionary_getvalue(CGDictionary* dir, const char* key);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
