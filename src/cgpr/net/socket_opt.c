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

#include <cgpr/net/socket_opt.h>
#include <stdlib.h>

/****************************************
 * cg_socket_option_new
 ****************************************/

CGSocketOption* cg_socket_option_new()
{
  CGSocketOption* opt;

  opt = (CGSocketOption*)malloc(sizeof(CGSocketOption));
  if (!opt)
    return NULL;

  cg_socket_option_setreuseaddress(opt, false);
  cg_socket_option_setbindinterface(opt, false);
  cg_socket_option_setmulticastloop(opt, false);

  return opt;
}

/****************************************
 * cg_socket_option_delete
 ****************************************/

bool cg_socket_option_delete(CGSocketOption* opt)
{
  if (!opt)
    return true;

  free(opt);

  return true;
}
