/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004-5 Altera Corporation, San Jose, California, USA.         *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
*                                                                             *
* altera_avalon_mutex.h                                                       *
*                                                                             *
* Public interfaces to the Mutex hardware component                           *
*                                                                             *
******************************************************************************/
#ifndef __ALTERA_AVALON_MUTEX_H__
#define __ALTERA_AVALON_MUTEX_H__
#include "priv/alt_dev_llist.h"
#include "sys/alt_dev.h"
#include "sys/alt_errno.h"
#include "os/alt_sem.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * The function alt_find_dev() is used to search the device list "list" to
 * locate a device named "name". If a match is found, then a pointer to the
 * device is returned, otherwise NULL is returned.
 */

extern alt_dev* alt_find_dev (const char* name, alt_llist* list);

/*
 * Mutex Device Structure
 */
typedef struct alt_mutex_dev
{
  alt_llist  llist;
  const char* name;
  void*       mutex_base;
  ALT_SEM     (lock)      /* Semaphore used to control access to the Mutex */
} alt_mutex_dev;

/*
 * Prototypes
 */
extern alt_mutex_dev* altera_avalon_mutex_open (const char* name);
extern void altera_avalon_mutex_lock( alt_mutex_dev* dev, alt_u32 value );
extern int altera_avalon_mutex_trylock( alt_mutex_dev* dev, alt_u32 value );
extern void altera_avalon_mutex_unlock( alt_mutex_dev* dev );
extern int altera_avalon_mutex_is_mine( alt_mutex_dev* dev );
extern int altera_avalon_mutex_first_lock( alt_mutex_dev* dev );
extern void altera_avalon_mutex_close( alt_mutex_dev* dev );

/*
 * Register a Mutex device
 */

static ALT_INLINE int alt_avalon_mutex_reg (alt_mutex_dev* dev)
{
  int ret_code;
  extern alt_llist alt_mutex_list;
  ret_code = ALT_SEM_CREATE (&dev->lock, 1);
  if (!ret_code)
  {
    ret_code = alt_dev_llist_insert((alt_dev_llist*) dev, &alt_mutex_list);
  }
  else
  {
    ALT_ERRNO = ENOMEM;
    ret_code = -ENOMEM;
  }

  return ret_code;
}


/*
*   Macros used by alt_sys_init.c
*
*/

#define ALTERA_AVALON_MUTEX_INSTANCE(name, dev)   \
static alt_mutex_dev dev =                        \
{                                                 \
  ALT_LLIST_ENTRY,                                \
  name##_NAME,                                    \
  ((void*)( name##_BASE))                         \
}

#define ALTERA_AVALON_MUTEX_INIT(name, dev) alt_avalon_mutex_reg( &dev )

#ifdef __cplusplus
}
#endif

#endif /* __ALTERA_AVALON_MUTEX_H__ */
