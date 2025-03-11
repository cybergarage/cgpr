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

#include <cgpr/util/string.h>

////////////////////////////////////////
// testNetworkInterface
////////////////////////////////////////

#define CG_TESTCASE_INTVALUE -100
#define CG_TESTCASE_INTVALUE_STRING "-100"
#define CG_TESTCASE_UINTVALUE 100
#define CG_TESTCASE_UINTVALUE_STRING "100"

#define CG_TESTCASE_DBLVALUE -12.3
#define CG_TESTCASE_DBLVALUE_STRING "-12.3"
#define CG_TESTCASE_UDBLVALUE 12.3
#define CG_TESTCASE_UDBLVALUE_STRING "12.3"

BOOST_AUTO_TEST_CASE(StringTest)
{
  char buf[128];

  cg_int2str(CG_TESTCASE_INTVALUE, buf, sizeof(buf));
  BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_INTVALUE_STRING));
  cg_int2str(CG_TESTCASE_UINTVALUE, buf, sizeof(buf));
  BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_UINTVALUE_STRING));

  cg_long2str(CG_TESTCASE_INTVALUE, buf, sizeof(buf));
  BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_INTVALUE_STRING));
  cg_long2str(CG_TESTCASE_UINTVALUE, buf, sizeof(buf));
  BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_UINTVALUE_STRING));

  /*
  cg_float2str(CG_TESTCASE_DBLVALUE, buf, sizeof(buf));
        BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_DBLVALUE_STRING));
  cg_float2str(CG_TESTCASE_UDBLVALUE, buf, sizeof(buf));
        BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_UDBLVALUE_STRING));

  cg_double2str(CG_TESTCASE_DBLVALUE, buf, sizeof(buf));
        BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_DBLVALUE_STRING));
  cg_double2str(CG_TESTCASE_UDBLVALUE, buf, sizeof(buf));
        BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_UDBLVALUE_STRING));
  */

  cg_sizet2str(CG_TESTCASE_UINTVALUE, buf, sizeof(buf));
  BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_UINTVALUE_STRING));

  cg_ssizet2str(CG_TESTCASE_INTVALUE, buf, sizeof(buf));
  BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_INTVALUE_STRING));
  cg_ssizet2str(CG_TESTCASE_UINTVALUE, buf, sizeof(buf));
  BOOST_REQUIRE(cg_streq(buf, CG_TESTCASE_UINTVALUE_STRING));
}
