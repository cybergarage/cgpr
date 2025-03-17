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

/****************************************
 * cg_string_tokenizer_new
 ****************************************/

CGStringTokenizer* cg_string_tokenizer_new(const char* value,
    const char* delim)
{
  CGStringTokenizer* str_token;

  str_token = (CGStringTokenizer*)malloc(sizeof(CGStringTokenizer));

  if (NULL != str_token) {
    str_token->value = cg_strdup(value);
    str_token->delim = cg_strdup(delim);
    str_token->delimCnt = cg_strlen(str_token->delim);
    str_token->nextStartPos = 0;
    str_token->lastPos = cg_strlen(value) - 1;
    str_token->currToken = NULL;
    str_token->nextToken = NULL;
    cg_string_tokenizer_nexttoken(str_token);
  }

  return str_token;
}

/****************************************
 * cg_string_tokenizer_delete
 ****************************************/

void cg_string_tokenizer_delete(CGStringTokenizer* str_token)
{
  free(str_token->value);
  free(str_token->delim);
  free(str_token);
}

/****************************************
 * cg_string_tokenizer_hasmoretoken
 ****************************************/

bool cg_string_tokenizer_hasmoretoken(CGStringTokenizer* str_token)
{
  return str_token->hasNextTokens;
}

/****************************************
 * cg_string_tokenizer_nexttoken
 ****************************************/

char* cg_string_tokenizer_nexttoken(CGStringTokenizer* str_token)
{
  size_t token_cnt;
  size_t i, j;

  str_token->currToken = str_token->nextToken;
  str_token->nextToken = NULL;
  str_token->hasNextTokens = false;
  str_token->repToken = '\0';

  token_cnt = 0;
  for (i = str_token->nextStartPos; i <= str_token->lastPos; i++) {
    bool is_delim_char = false;
    for (j = 0; j < str_token->delimCnt; j++) {
      if (str_token->value[i] == str_token->delim[j]) {
        is_delim_char = true;
        if (token_cnt == 0) {
          str_token->nextStartPos = i + 1;
          break;
        }
        str_token->hasNextTokens = true;
        str_token->repToken = str_token->value[i];
        str_token->value[i] = '\0';
        str_token->nextToken = str_token->value + str_token->nextStartPos;
        str_token->nextStartPos = i + 1;
      }
    }
    if (str_token->hasNextTokens == true)
      break;
    if (is_delim_char == false)
      token_cnt++;
  }

  if (str_token->hasNextTokens == false && 0 < token_cnt) {
    str_token->hasNextTokens = true;
    str_token->nextToken = str_token->value + str_token->nextStartPos;
    str_token->nextStartPos = str_token->lastPos + 1;
  }

  return str_token->currToken;
}

/****************************************
 * cg_string_tokenizer_nextalltoken
 ****************************************/

char* cg_string_tokenizer_nextalltoken(CGStringTokenizer* str_token)
{
  size_t nextToken_len;

  nextToken_len = cg_strlen(str_token->nextToken);
  str_token->nextToken[nextToken_len] = str_token->repToken;
  str_token->currToken = str_token->nextToken;
  str_token->nextToken = NULL;
  str_token->hasNextTokens = false;
  return str_token->currToken;
}
