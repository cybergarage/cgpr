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

#include <cgpr/util/log.h>
#include <cgpr/util/string.h>

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

char* cg_string_naddvalue(CGString* str, const char* value, size_t valueLen)
{
  char* newValue = NULL;
  size_t newMemSize = 0;

  if (NULL == str)
    return NULL;

  if (value == NULL || valueLen <= 0) {
    /* Empty string, nothing to add */
    return cg_string_getvalue(str);
  }

  /* Check, if we need to allocate memory for the new data */
  newMemSize = str->valueSize + valueLen + 1;
  if (newMemSize > str->memSize || str->value == NULL) {
    /* realloc also some extra in order to avoid multiple reallocs */
    newMemSize += cg_STRING_REALLOC_EXTRA;
    newValue = realloc(str->value, newMemSize * sizeof(char));

    if (newValue == NULL) {
      /* Memory allocation failed, bail out */
      return NULL;
    }

    str->memSize = newMemSize;
    str->value = newValue;
  }

  /* memcpy works better with non-zero-terminated data
           than strncpy */
  memcpy(str->value + str->valueSize, value, valueLen);

  str->valueSize += valueLen;

  /* In case this is a string, append a termination character */
  str->value[str->valueSize] = '\0';

  return cg_string_getvalue(str);
}

/****************************************
 * cg_string_addrep
 ****************************************/

char* cg_string_addrepvalue(CGString* str, const char* value, size_t repeatCnt)
{
  int n;

  for (n = 0; n < repeatCnt; n++)
    cg_string_addvalue(str, value);

  return cg_string_getvalue(str);
}

/****************************************
 * cg_string_naddrep
 ****************************************/

char* cg_string_naddrepvalue(CGString* str, const char* value, size_t valueLen, size_t repeatCnt)
{
  int n;

  for (n = 0; n < repeatCnt; n++)
    cg_string_naddvalue(str, value, valueLen);

  return cg_string_getvalue(str);
}

/****************************************
 * cg_string_replace
 ****************************************/

char* cg_string_replace(CGString* str, char* fromStr[], char* toStr[], size_t fromStrCnt)
{
  char* orgValue = NULL;
  size_t orgValueLen = 0;
  int n = 0;
  int copyPos = 0;
  size_t* fromStrLen = NULL;
  CGString* repValue = NULL;
  bool isReplaced = false;

  if (NULL == str)
    return NULL;

  repValue = cg_string_new();

  fromStrLen = (size_t*)malloc(sizeof(size_t) * fromStrCnt);

  if (NULL == fromStrLen) {
    cg_string_delete(repValue);
    return NULL;
  }

  for (n = 0; n < fromStrCnt; n++)
    fromStrLen[n] = cg_strlen(fromStr[n]);

  orgValue = cg_string_getvalue(str);
  orgValueLen = cg_string_length(str);

  copyPos = 0;
  while (copyPos < orgValueLen) {
    isReplaced = false;
    for (n = 0; n < fromStrCnt; n++) {
      if (strncmp(fromStr[n], orgValue + copyPos, fromStrLen[n]) == 0) {
        cg_string_addvalue(repValue, toStr[n]);
        copyPos += fromStrLen[n];
        isReplaced = true;
        continue;
      }
    }
    if (isReplaced == true)
      continue;
    cg_string_naddvalue(repValue, orgValue + copyPos, 1);
    copyPos++;
  }

  free(fromStrLen);

  cg_string_setvalue(str, cg_string_getvalue(repValue));

  cg_string_delete(repValue);

  return cg_string_getvalue(str);
}
