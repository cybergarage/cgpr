######################################################################
# 
# Copyright (C) 2025 The Cyber Garage Portable Runtime for C Authors
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#  http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# 
######################################################################

#if HAVE_LIBTOOL
#lib_LTLIBRARIES = libcgpr.la
#else
lib_LIBRARIES = libcgpr.a
#endif

AM_CFLAGS = -I$(top_builddir)/include -I$(top_builddir)/src

cg_sources = \
	../../src/cgpr/util/dictionary.c \
	../../src/cgpr/util/log.c \
	../../src/cgpr/util/thread.c \
	../../src/cgpr/util/list.c \
	../../src/cgpr/util/timer.c \
	../../src/cgpr/util/strings.c \
	../../src/cgpr/util/bytes.c \
	../../src/cgpr/util/dictionary_elem.c \
	../../src/cgpr/util/thread_list.c \
	../../src/cgpr/util/mutex.c \
	../../src/cgpr/util/cond.c \
	../../src/cgpr/util/string.c \
	../../src/cgpr/util/strings_function.c \
	../../src/cgpr/util/string_function.c \
	../../src/cgpr/util/time.c \
	../../src/cgpr/util/time_wince.c \
	../../src/cgpr/util/strings_tokenizer.c \
	../../src/cgpr/util/string_tokenizer.c

libcgprincludedir = $(includedir)/cgpr
nobase_libcgprinclude_HEADERS =  \
        $(cg_headers)

#if HAVE_LIBTOOL
#libcgpr_la_SOURCES = \
#        $(cg_source)
#libcgpr_la_CFLAGS = $(AM_CFLAGS)
#else
libcgpr_a_SOURCES = \
        $(cg_sources)
libcgpr_a_CFLAGS = $(AM_CFLAGS)
#endif
