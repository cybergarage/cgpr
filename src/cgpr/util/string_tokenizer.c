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

/****************************************
 * cg_string_tokenizer_new
 ****************************************/

cgprStringTokenizer* cg_string_tokenizer_new(const char* value, const char* delim)
{
  cgprStringTokenizer* strToken;

  strToken = (cgprStringTokenizer*)malloc(sizeof(cgprStringTokenizer));

  if (NULL != strToken) {
    strToken->value = cg_strdup(value);
    strToken->delim = cg_strdup(delim);
    strToken->delimCnt = cg_strlen(strToken->delim);
    strToken->nextStartPos = 0;
    strToken->lastPos = cg_strlen(value) - 1;
    strToken->currToken = NULL;
    strToken->nextToken = NULL;
    cg_string_tokenizer_nexttoken(strToken);
  }

  return strToken;
}

/****************************************
 * cg_string_tokenizer_delete
 ****************************************/

void cg_string_tokenizer_delete(cgprStringTokenizer* strToken)
{
  free(strToken->value);
  free(strToken->delim);
  free(strToken);
}

/****************************************
 * cg_string_tokenizer_hasmoretoken
 ****************************************/

bool cg_string_tokenizer_hasmoretoken(cgprStringTokenizer* strToken)
{
  return strToken->hasNextTokens;
}

/****************************************
 * cg_string_tokenizer_nexttoken
 ****************************************/

char* cg_string_tokenizer_nexttoken(cgprStringTokenizer* strToken)
{
  size_t tokenCnt;
  size_t i, j;

  strToken->currToken = strToken->nextToken;
  strToken->nextToken = NULL;
  strToken->hasNextTokens = false;
  strToken->repToken = '\0';

  tokenCnt = 0;
  for (i = strToken->nextStartPos; i <= strToken->lastPos; i++) {
    bool isDelimChar = false;
    for (j = 0; j < strToken->delimCnt; j++) {
      if (strToken->value[i] == strToken->delim[j]) {
        isDelimChar = true;
        if (tokenCnt == 0) {
          strToken->nextStartPos = i + 1;
          break;
        }
        strToken->hasNextTokens = true;
        strToken->repToken = strToken->value[i];
        strToken->value[i] = '\0';
        strToken->nextToken = strToken->value + strToken->nextStartPos;
        strToken->nextStartPos = i + 1;
      }
    }
    if (strToken->hasNextTokens == true)
      break;
    if (isDelimChar == false)
      tokenCnt++;
  }

  if (strToken->hasNextTokens == false && 0 < tokenCnt) {
    strToken->hasNextTokens = true;
    strToken->nextToken = strToken->value + strToken->nextStartPos;
    strToken->nextStartPos = strToken->lastPos + 1;
  }

  return strToken->currToken;
}

/****************************************
 * cg_string_tokenizer_nextalltoken
 ****************************************/

char* cg_string_tokenizer_nextalltoken(cgprStringTokenizer* strToken)
{
  size_t nextTokenLen;

  nextTokenLen = cg_strlen(strToken->nextToken);
  strToken->nextToken[nextTokenLen] = strToken->repToken;
  strToken->currToken = strToken->nextToken;
  strToken->nextToken = NULL;
  strToken->hasNextTokens = false;
  return strToken->currToken;
}
