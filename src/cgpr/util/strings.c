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

#include <cgpr/util/strings.h>

#include <string.h>

/* Define amount of extra characters allocated on each realloc, with this we
   can avoid many small subsequent reallocs, which takes lots of time */
#define cg_STRING_REALLOC_EXTRA 16

/****************************************
 * cg_string_new
 ****************************************/

CGString* cg_string_new(void)
{
  CGString* str;

  str = (CGString*)malloc(sizeof(CGString));

  if (NULL != str) {
    str->value = NULL;
    str->memSize = 0;
    str->valueSize = 0;
  }

  return str;
}

/****************************************
 * cg_string_delete
 ****************************************/

void cg_string_delete(CGString* str)
{
  if (NULL != str) {
    cg_string_clear(str);
    free(str);
  }
}

/****************************************
 * cg_string_delete
 ****************************************/

void cg_string_clear(CGString* str)
{
  if (NULL != str) {
    if (str->value != NULL) {
      free(str->value);
      str->value = NULL;
      str->memSize = 0;
      str->valueSize = 0;
    }
  }
}

/****************************************
 * cg_string_setvalue
 ****************************************/

void cg_string_setvalue(CGString* str, const char* value)
{
  if (NULL != str) {
    if (value != NULL)
      cg_string_setnvalue(str, value, cg_strlen(value));
  }
}

/****************************************
 * cg_string_setintvalue
 ****************************************/

void cg_string_setintvalue(CGString* str, int value)
{
  char buf[CG_STRING_INTEGER_BUFLEN];

  cg_string_setvalue(str, cg_int2str(value, buf, sizeof(buf)));
}

/****************************************
 * cg_string_setlongvalue
 ****************************************/

void cg_string_setlongvalue(CGString* str, long value)
{
  char buf[CG_STRING_LONG_BUFLEN];

  cg_string_setvalue(str, cg_long2str(value, buf, sizeof(buf)));
}

/****************************************
 * cg_string_setnvalue
 ****************************************/

void cg_string_setnvalue(CGString* str, const char* value, size_t len)
{
  if (NULL != str) {
    cg_string_clear(str);
    if (value != NULL) {
      str->valueSize = len;
      str->memSize = str->valueSize + 1;
      str->value = (char*)malloc(str->memSize * sizeof(char));

      if (NULL == str->value) {
        return;
      }

      /* memcpy works better with non-zero-terminated data than strncpy */
      memcpy(str->value, value, len);
      str->value[len] = '\0';
    }
  }
}

/****************************************
 * cg_string_setpointervalue
 ****************************************/

void cg_string_setpointervalue(CGString* str, char* value, size_t len)
{
  if (NULL != str) {
    cg_string_clear(str);
    str->value = value;
    str->valueSize = len;
    str->memSize = str->valueSize + 1;
  }
}

/****************************************
 * cg_string_getvalue
 ****************************************/

char* cg_string_getvalue(CGString* str)
{
  return (NULL != str) ? str->value : NULL;
}

/****************************************
 * cg_string_getmemorysize
 ****************************************/

size_t cg_string_getmemorysize(CGString* str)
{
  if (NULL == str)
    return 0;

  return str->memSize;
}

/****************************************
 * cg_string_length
 ****************************************/

size_t cg_string_length(CGString* str)
{
  if (NULL == str)
    return 0;

  if (str->value == NULL)
    return 0;

  return str->valueSize;
}

/****************************************
 * cg_string_add
 ****************************************/

char* cg_string_addvalue(CGString* str, const char* value)
{
  return cg_string_naddvalue(str, value, cg_strlen(value));
}

/****************************************
 * cg_string_add
 ****************************************/

char* cg_string_naddvalue(CGString* str, const char* value, size_t value_len)
{
  char* new_value = NULL;
  size_t new_memSize = 0;

  if (NULL == str)
    return NULL;

  if (value == NULL || value_len <= 0) {
    /* Empty string, nothing to add */
    return cg_string_getvalue(str);
  }

  /* Check, if we need to allocate memory for the new data */
  new_memSize = str->valueSize + value_len + 1;
  if (new_memSize > str->memSize || str->value == NULL) {
    /* realloc also some extra in order to avoid multiple reallocs */
    new_memSize += cg_STRING_REALLOC_EXTRA;
    new_value = realloc(str->value, new_memSize * sizeof(char));

    if (new_value == NULL) {
      /* Memory allocation failed, bail out */
      return NULL;
    }

    str->memSize = new_memSize;
    str->value = new_value;
  }

  /* memcpy works better with non-zero-terminated data
     than strncpy */
  memcpy(str->value + str->valueSize, value, value_len);

  str->valueSize += value_len;

  /* In case this is a string, append a termination character */
  str->value[str->valueSize] = '\0';

  return cg_string_getvalue(str);
}

/****************************************
 * cg_string_addrep
 ****************************************/

char* cg_string_addrepvalue(CGString* str, const char* value, size_t repeat_cnt)
{
  int n;

  for (n = 0; n < repeat_cnt; n++)
    cg_string_addvalue(str, value);

  return cg_string_getvalue(str);
}

/****************************************
 * cg_string_naddrep
 ****************************************/

char* cg_string_naddrepvalue(CGString* str, const char* value, size_t value_len, size_t repeat_cnt)
{
  int n;

  for (n = 0; n < repeat_cnt; n++)
    cg_string_naddvalue(str, value, value_len);

  return cg_string_getvalue(str);
}

/****************************************
 * cg_string_replace
 ****************************************/

char* cg_string_replace(CGString* str, char* from_str[], char* to_str[], size_t from_str_cnt)
{
  char* org_value = NULL;
  size_t org_value_len = 0;
  int n = 0;
  int copy_pos = 0;
  size_t* from_str_len = NULL;
  CGString* rep_value = NULL;
  bool is_replaced = false;

  if (NULL == str)
    return NULL;

  rep_value = cg_string_new();

  from_str_len = (size_t*)malloc(sizeof(size_t) * from_str_cnt);

  if (NULL == from_str_len) {
    cg_string_delete(rep_value);
    return NULL;
  }

  for (n = 0; n < from_str_cnt; n++)
    from_str_len[n] = cg_strlen(from_str[n]);

  org_value = cg_string_getvalue(str);
  org_value_len = cg_string_length(str);

  copy_pos = 0;
  while (copy_pos < org_value_len) {
    is_replaced = false;
    for (n = 0; n < from_str_cnt; n++) {
      if (strncmp(from_str[n], org_value + copy_pos, from_str_len[n]) == 0) {
        cg_string_addvalue(rep_value, to_str[n]);
        copy_pos += from_str_len[n];
        is_replaced = true;
        continue;
      }
    }
    if (is_replaced == true)
      continue;
    cg_string_naddvalue(rep_value, org_value + copy_pos, 1);
    copy_pos++;
  }

  free(from_str_len);

  cg_string_setvalue(str, cg_string_getvalue(rep_value));

  cg_string_delete(rep_value);

  return cg_string_getvalue(str);
}
