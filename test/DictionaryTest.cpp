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

#include <boost/test/unit_test.hpp>

#include <cgpr/util/dictionary.h>

////////////////////////////////////////
// testDictionary
////////////////////////////////////////

#define CG_TESTCASE_DICTIONARY_KEY "hello"
#define CG_TESTCASE_DICTIONARY_VALUE "hello_value"

BOOST_AUTO_TEST_CASE(DictionaryTest)
{
  CGDictionary* dir;
  const char* dirVal;

  /* Basic Unit Test */
  dir = cg_dictionary_new();
  BOOST_REQUIRE((dir != NULL));
  cg_dictionary_delete(dir);

  /* Complet Unit Test */
  dir = cg_dictionary_new();
  BOOST_REQUIRE((dir != NULL));
  BOOST_REQUIRE((cg_dictionary_size(dir) == 0));
  cg_dictionary_setvalue(dir, CG_TESTCASE_DICTIONARY_KEY, CG_TESTCASE_DICTIONARY_VALUE);
  BOOST_REQUIRE((cg_dictionary_size(dir) == 1));
  cg_dictionary_setvalue(dir, CG_TESTCASE_DICTIONARY_KEY, CG_TESTCASE_DICTIONARY_VALUE);
  BOOST_REQUIRE((cg_dictionary_size(dir) == 1));
  dirVal = cg_dictionary_getvalue(dir, CG_TESTCASE_DICTIONARY_KEY);
  BOOST_REQUIRE((0 < cg_strlen(dirVal)));
  BOOST_REQUIRE(cg_streq(dirVal, CG_TESTCASE_DICTIONARY_VALUE));
  cg_dictionary_delete(dir);
}
