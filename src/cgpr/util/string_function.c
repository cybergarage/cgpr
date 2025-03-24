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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <ctype.h>

#include <cgpr/util/log.h>
#include <cgpr/util/string.h>

#if defined(WINCE)
// Theo Beisch - needed for _i64toa_s support -
//  strangely just with the VS2005 ce environment
//  evc4 is only happy without :-(
#include <altcecrt.h>
#endif

#if !defined(NORTiAPI)
#include <string.h>
#endif

/****************************************
 * cg_strdup
 ****************************************/

char* cg_strdup(const char* str)
{
#if !defined(HAVE_STRDUP)
  char* cpStrBuf;
#endif

  if (str == NULL)
    return NULL;

#if defined(HAVE_STRDUP)
  return strdup(str);
#else
  cpStrBuf = (char*)malloc(strlen(str) + 1);
  if (NULL != cpStrBuf)
    strcpy(cpStrBuf, str);
  return cpStrBuf;
#endif
}

/****************************************
 * cg_strlen
 ****************************************/

size_t cg_strlen(const char* str)
{
  return (str == NULL) ? 0 : strlen(str);
}

/****************************************
 * cg_strcpy
 ****************************************/

char* cg_strcpy(char* dest, const char* src)
{
  return strcpy(dest, src);
}

/****************************************
 * cg_strcat
 ****************************************/

char* cg_strcat(char* dest, const char* src)
{
  return strcat(dest, src);
}

/****************************************
 * cg_strcmp
 ****************************************/

int cg_strcmp(const char* str1, const char* str2)
{
  if (str1 == NULL)
    return -1;
  if (str2 == NULL)
    return 1;
  return strcmp(str1, str2);
}

/****************************************
 * cg_strncmp
 ****************************************/

int cg_strncmp(const char* str1, const char* str2, int nchars)
{
  if (str1 == NULL)
    return -1;
  if (str2 == NULL)
    return 1;

  return strncmp(str1, str2, nchars);
}

/****************************************
 * cg_strcasecmp
 ****************************************/

int cg_strcasecmp(const char* str1, const char* str2)
{
  if (str1 == NULL || str2 == NULL)
    return -1;

#if !defined(WIN32)
  return strcasecmp(str1, str2);
#else
  for (; toupper(*str1) == toupper(*str2); str1++, str2++) {
    if (*str1 == '\0')
      return 0;
  }
  return *str1 - *str2;
#endif
}

/****************************************
 * cg_streq
 ****************************************/

bool cg_streq(const char* str1, const char* str2)
{
  if (str1 == NULL || str2 == NULL)
    return false;
  return ((cg_strcmp(str1, str2) == 0) ? true : false);
}

/****************************************
 * cg_strcaseeq
 ****************************************/

bool cg_strcaseeq(const char* str1, const char* str2)
{
  if (str1 == NULL || str2 == NULL)
    return false;
  return ((cg_strcasecmp(str1, str2) == 0) ? true : false);
}

/****************************************
 * cg_strstr
 ****************************************/

ssize_t cg_strstr(const char* haystack, const char* needle)
{
  char* strPos;
  if (haystack == NULL || needle == NULL)
    return -1;

  strPos = strstr(haystack, needle);
  if (strPos == NULL)
    return -1;

  return (strPos - haystack);
}

/****************************************
 * cg_strchr
 ****************************************/

ssize_t cg_strchr(const char* str, const char* chars, size_t nchars)
{
  size_t strLen;
  ssize_t i, j;

  if (str == NULL || chars == NULL)
    return -1;

  strLen = cg_strlen(str);
  for (i = 0; i < strLen; i++) {
    for (j = 0; j < nchars; j++) {
      if (str[i] == chars[j])
        return i;
    }
  }

  return -1;
}

/****************************************
 * cg_strrchr
 ****************************************/

ssize_t cg_strrchr(const char* str, const char* chars, size_t nchars)
{
  size_t strLen;
  ssize_t i, j;

  if (str == NULL || chars == NULL)
    return -1;

  strLen = cg_strlen(str);
  for (i = (strLen - 1); 0 <= i; i--) {
    for (j = 0; j < nchars; j++) {
      if (str[i] == chars[j])
        return i;
    }
  }

  return -1;
}

/****************************************
 * cg_strtrimwhite
 ****************************************/

char* cg_strtrimwhite(char* str)
{
  size_t strLen;
  ssize_t i;
  strLen = cg_strlen(str);
  if (strLen == 0)
    return str;
  for (i = (strLen - 1); 0 <= i; i--) {
    if (isspace(str[i])) {
      strLen--;
    }
  }
  for (i = 0; i < strLen; i++) {
    if (!isspace(str[i]))
      break;
  }

  if (i > 0) {
    memmove(str, str + i, strLen - i);
  }

  str[strLen] = 0;
  return str;
}

/****************************************
 * cg_strtrim
 ****************************************/

char* cg_strtrim(char* str, char* delim, size_t ndelim)
{
  if (str == NULL || delim == NULL)
    return NULL;

  cg_strrtrim(str, delim, ndelim);

  return cg_strltrim(str, delim, ndelim);
}

/****************************************
 * cg_strltrim
 ****************************************/

char* cg_strltrim(char* str, char* delim, size_t ndelim)
{
  size_t strLen;
  ssize_t i, j;

  strLen = cg_strlen(str);
  for (i = 0; i < strLen; i++) {
    bool hasDelim = false;
    for (j = 0; j < ndelim; j++) {
      if (str[i] == delim[j]) {
        hasDelim = true;
        break;
      }
    }
    if (hasDelim == false)
      return (str + i);
  }

  return (str + strLen);
}

/****************************************
 * cg_strrtrim
 ****************************************/

char* cg_strrtrim(char* str, char* delim, size_t ndelim)
{
  size_t strLen;
  ssize_t i, j;

  strLen = cg_strlen(str);
  for (i = (strLen - 1); 0 <= i; i--) {
    bool hasDelim = false;
    for (j = 0; j < ndelim; j++) {
      if (str[i] == delim[j]) {
        hasDelim = true;
        str[i] = '\0';
        break;
      }
    }
    if (hasDelim == false)
      break;
  }

  return str;
}

/****************************************
 * cg_strncpy
 ****************************************/

char* cg_strncpy(char* str1, const char* str2, size_t cnt)
{
  strncpy(str1, str2, cnt);
  return str1;
}

/****************************************
 * cg_strncat
 ****************************************/

char* cg_strncat(char* str1, const char* str2, size_t cnt)
{
  size_t str1Len;
  str1Len = cg_strlen(str1);
  return cg_strncpy((str1 + str1Len), str2, cnt);
}

/****************************************
 * cg_int2str
 ****************************************/

const char* cg_int2str(int value, char* buf, size_t bufSize)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%d", value);
#else
  sprintf(buf, "%d", value);
#endif
  return buf;
}

/****************************************
 * cg_long2str
 ****************************************/

const char* cg_long2str(long value, char* buf, size_t bufSize)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%ld", value);
#else
  sprintf(buf, "%ld", value);
#endif
  return buf;
}

/****************************************
 * cg_float2str
 ****************************************/

const char* cg_float2str(float value, char* buf, size_t bufSize)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%f", value);
#else
  sprintf(buf, "%f", value);
#endif
  return buf;
}

/****************************************
 * cg_double2str
 ****************************************/

const char* cg_double2str(double value, char* buf, size_t bufSize)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%lf", value);
#else
  sprintf(buf, "%lf", value);
#endif
  return buf;
}

/****************************************
 * cg_sizet2str
 ****************************************/

const char* cg_sizet2str(size_t value, char* buf, size_t bufSize)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%zd", value);
#else
  sprintf(buf, "%zd", value);
#endif
  return buf;
}

/****************************************
 * cg_ssizet2str
 ****************************************/

const char* cg_ssizet2str(ssize_t value, char* buf, size_t bufSize)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%zd", value);
#else
  sprintf(buf, "%zd", value);
#endif
  return buf;
}
