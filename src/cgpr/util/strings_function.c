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

#include <ctype.h>
#include <string.h>

#include <cgpr/util/strings.h>

/****************************************
 * cg_strdup
 ****************************************/

char* cg_strdup(const char* str)
{
#if !defined(HAVE_STRDUP)
  char* cp_str_buf;
#endif

  if (str == NULL)
    return NULL;

#if defined(HAVE_STRDUP)
  return strdup(str);
#else
  cp_str_buf = (char*)malloc(strlen(str) + 1);
  if (NULL != cp_str_buf)
    strcpy(cp_str_buf, str);
  return cp_str_buf;
#endif
}

/****************************************
 * cg_strloc
 ****************************************/

bool cg_strloc(const char* str, char** buf)
{
  if (!str || !buf)
    return false;

  if (*buf) {
    free(*buf);
    *buf = NULL;
  }

  if (!str)
    return true;

  *buf = cg_strdup(str);

  return true;
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

char* cg_strcpy(char* dst, const char* src) { return strcpy(dst, src); }

/****************************************
 * cg_strcat
 ****************************************/

char* cg_strcat(char* dst, const char* src) { return strcat(dst, src); }

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

int cg_strncmp(const char* str1, const char* str2, size_t nchars)
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
  char* str_pos;

  if (haystack == NULL || needle == NULL)
    return -1;
  str_pos = strstr(haystack, needle);
  if (str_pos == NULL)
    return -1;
  return (str_pos - haystack);
}

/****************************************
 * cg_strchr
 ****************************************/

ssize_t cg_strchr(const char* str, const char* chars, size_t nchars)
{
  size_t str_len;
  ssize_t i, j;

  if (str == NULL || chars == NULL)
    return -1;

  str_len = cg_strlen(str);
  for (i = 0; i < str_len; i++) {
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
  size_t str_len;
  ssize_t i, j;

  if (str == NULL || chars == NULL)
    return -1;

  str_len = cg_strlen(str);
  for (i = (str_len - 1); 0 <= i; i--) {
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
  size_t str_len;
  ssize_t i;
  str_len = cg_strlen(str);
  if (str_len == 0)
    return str;
  for (i = (str_len - 1); 0 <= i; i--) {
    if (isspace(str[i])) {
      str_len--;
    }
  }
  for (i = 0; i < str_len; i++) {
    if (!isspace(str[i]))
      break;
  }

  if (i > 0)
    memmove(str, str + i, str_len - i);

  str[str_len] = 0;
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
  size_t str_len;
  ssize_t i, j;

  str_len = cg_strlen(str);
  for (i = 0; i < str_len; i++) {
    bool has_delim = false;
    for (j = 0; j < ndelim; j++) {
      if (str[i] == delim[j]) {
        has_delim = true;
        break;
      }
    }
    if (has_delim == false)
      return (str + i);
  }

  return (str + str_len);
}

/****************************************
 * cg_strrtrim
 ****************************************/

char* cg_strrtrim(char* str, char* delim, size_t ndelim)
{
  size_t str_len;
  ssize_t i, j;

  str_len = cg_strlen(str);
  for (i = (str_len - 1); 0 <= i; i--) {
    bool has_delim = false;
    for (j = 0; j < ndelim; j++) {
      if (str[i] == delim[j]) {
        has_delim = true;
        str[i] = '\0';
        break;
      }
    }
    if (has_delim == false)
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
  size_t str1_len;
  str1_len = cg_strlen(str1);
  return cg_strncpy((str1 + str1_len), str2, cnt);
}

/****************************************
 * cg_int2str
 ****************************************/

const char* cg_int2str(int value, char* buf, size_t buf_size)
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

const char* cg_long2str(long value, char* buf, size_t buf_size)
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

const char* cg_float2str(float value, char* buf, size_t buf_size)
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

const char* cg_double2str(double value, char* buf, size_t buf_size)
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

const char* cg_sizet2str(size_t value, char* buf, size_t buf_size)
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

const char* cg_ssizet2str(ssize_t value, char* buf, size_t buf_size)
{
#if defined(HAVE_SNPRINTF)
  snprintf(buf, bufSize, "%zd", value);
#else
  sprintf(buf, "%zd", value);
#endif
  return buf;
}
