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
#include <boost/thread.hpp>

#include <cgpr/util/thread.h>
#include <cgpr/util/timer.h>

const int THREAD_TEST_LOOP_NUM = 10;

void test_theread_func(CGThread* thread)
{
  int* testCounter = (int*)cg_thread_getuserdata(thread);
  for (int n = 0; n < THREAD_TEST_LOOP_NUM; n++) {
    (*testCounter)++;
  }
}

BOOST_AUTO_TEST_CASE(ThreadTest)
{
  CGThread* thread = cg_thread_new();

  int testCounter = 0;
  cg_thread_setaction(thread, test_theread_func);
  cg_thread_setuserdata(thread, &testCounter);

  BOOST_REQUIRE_EQUAL(cg_thread_start(thread), true);
  while (testCounter != THREAD_TEST_LOOP_NUM) {
    cg_sleep(100);
  }
  BOOST_REQUIRE_EQUAL(testCounter, THREAD_TEST_LOOP_NUM);
  BOOST_REQUIRE_EQUAL(cg_thread_stop(thread), true);

  cg_thread_delete(thread);
}
