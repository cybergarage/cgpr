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

#ifndef _CGPR_UTIL_STRING_H_
#define _CGPR_UTIL_STRING_H_

#include <cgpr/typedef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#define cg_STRING_MEMORY_ALLOCATION_UNIT 64

/* UINT_MAX : 4294967295U */
#define CG_STRING_INTEGER_BUFLEN 16

/* ULONG_MAX : 4294967295UL */
#define CG_STRING_LONG_BUFLEN 32

/*  ULLONG_MAX : 18446744073709551615ULL */
#define cg_STRING_LONGLONG_BUFLEN 32

#define cg_STRING_FLOAT_BUFLEN 64
#define cg_STRING_DOUBLE_BUFLEN 64

/****************************************
 * Function
 ****************************************/

char* cg_strdup(const char* str);
size_t cg_strlen(const char* str);
char* cg_strcpy(char* dst, const char* src);
char* cg_strcat(char* dst, const char* src);
int cg_strcmp(const char* str1, const char* str2);
int cg_strncmp(const char* str1, const char* str2, size_t nchars);
int cg_strcasecmp(const char* str1, const char* str2);
bool cg_streq(const char* str1, const char* str2);
bool cg_strcaseeq(const char* str1, const char* str2);
ssize_t cg_strchr(const char* str, const char* chars, size_t nchars);
ssize_t cg_strrchr(const char* str, const char* chars, size_t nchars);
ssize_t cg_strstr(const char* haystack, const char* needle);
char* cg_strtrimwhite(char* str);
char* cg_strtrim(char* str, char* delim, size_t ndelim);
char* cg_strltrim(char* str, char* delim, size_t ndelim);
char* cg_strrtrim(char* str, char* delim, size_t ndelim);
char* cg_strncpy(char* str1, const char* str2, size_t cnt);
char* cg_strncat(char* str1, const char* str2, size_t cnt);
bool cg_strloc(const char* str, char** buf);

const char* cg_int2str(int value, char* buf, size_t buf_size);
const char* cg_long2str(long value, char* buf, size_t buf_size);
const char* cg_float2str(float value, char* buf, size_t buf_size);
const char* cg_double2str(double value, char* buf, size_t buf_size);
const char* cg_sizet2str(size_t value, char* buf, size_t buf_size);
const char* cg_ssizet2str(ssize_t value, char* buf, size_t buf_size);

#define cg_str2int(value) (value ? atoi(value) : 0)
#define cg_str2long(value) (value ? atol(value) : 0)
#define cg_strhex2long(value) (value ? strtol(value, NULL, 16) : 0)
#define cg_strhex2ulong(value) (value ? strtoul(value, NULL, 16) : 0)
#define cg_str2float(value) ((float)(value ? atof(value) : 0.0))
#define cg_str2double(value) (value ? atof(value) : 0.0)
#define cg_str2sizet(value) ((size_t)(value ? atol(value) : 0))
#define cg_str2ssizet(value) ((ssize_t)(value ? atol(value) : 0))

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoString {
  char* value;
  size_t mem_size;
  size_t value_size;
} CGString;

typedef struct UEchoStringTokenizer {
  char* value;
  char* delim;
  size_t delim_cnt;
  size_t next_start_pos;
  size_t last_pos;
  char* curr_token;
  char* next_token;
  char rep_token;
  bool has_next_tokens;
} CGStringTokenizer;

/****************************************
 * Function (StringTokenizer)
 ****************************************/

CGStringTokenizer* cg_string_tokenizer_new(const char* str,
    const char* delim);
void cg_string_tokenizer_delete(CGStringTokenizer* str_token);
bool cg_string_tokenizer_hasmoretoken(CGStringTokenizer* str_token);
char* cg_string_tokenizer_nexttoken(CGStringTokenizer* str_token);
char* cg_string_tokenizer_nextalltoken(CGStringTokenizer* str_token);

/****************************************
 * Function (String)
 ****************************************/

CGString* cg_string_new(void);
void cg_string_delete(CGString* str);
void cg_string_clear(CGString* str);

void cg_string_setvalue(CGString* str, const char* value);
void cg_string_setintvalue(CGString* str, int value);
void cg_string_setlongvalue(CGString* str, long value);
void cg_string_setfloatvalue(CGString* str, float value);
void cg_string_setdoublevalue(CGString* str, double value);
void cg_string_setnvalue(CGString* str, const char* value, size_t len);
void cg_string_setpointervalue(CGString* str, char* value, size_t len);

char* cg_string_getvalue(CGString* str);
#define cg_string_getintvalue(str) \
  cg_str2int(cg_string_getvalue(str))
#define cg_string_getlongvalue(str) \
  cg_str2long(cg_string_getvalue(str))
#define cg_string_getfloatvalue(str) \
  cg_str2float(cg_string_getvalue(str))
#define cg_string_getdoublevalue(str) \
  cg_str2double(cg_string_getvalue(str))

size_t cg_string_length(CGString* str);

char* cg_string_addvalue(CGString* str, const char* value);
char* cg_string_naddvalue(CGString* str, const char* value, size_t value_len);
char* cg_string_addrepvalue(CGString* str, const char* value, size_t repeat_cnt);
char* cg_string_naddrepvalue(CGString* str, const char* value, size_t value_len, size_t repeat_cnt);

char* cg_string_replace(CGString* str, char* from_str[], char* to_str[], size_t from_str_cnt);

size_t cg_string_getmemorysize(CGString* str);

#ifdef __cplusplus
}
#endif

#endif
