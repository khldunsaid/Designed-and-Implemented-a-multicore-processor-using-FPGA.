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
* altera_avalon_mutex.c                                                       *
*                                                                             *
* API for manipulating the hardware mutex                                     *
*                                                                             *
*****************************************************************************/
#include <stddef.h>
#include <errno.h>
#include "nios2.h"
#include "alt_types.h"
#include "sys/alt_errno.h"
#include "priv/alt_file.h"
#include "altera_avalon_mutex.h"
#include "altera_avalon_mutex_regs.h"

/*
 * The list of registered mutex components.
 */

ALT_LLIST_HEAD(alt_mutex_list);

/*
 * alt_mutex_trylock - Try to lock the hardware mutex
 *
 * returns 0 on success -1 otherwise
 *
 */
static int alt_mutex_trylock( alt_mutex_dev* dev, alt_u32 value )
{
  alt_u32 id, data, check;
  int ret_code = -1;

  NIOS2_READ_CPUID(id);

  /* the data we want the mutex to hold */
  data = (id << ALTERA_AVALON_MUTEX_MUTEX_OWNER_OFST) | value;

  /* attempt to write to the mutex */
  IOWR_ALTERA_AVALON_MUTEX_MUTEX(dev->mutex_base, data);
  
  check = IORD_ALTERA_AVALON_MUTEX_MUTEX(dev->mutex_base);

  if ( check == data)
  {
    ret_code = 0;
  }

  return ret_code;
}


/*
 * altera_avalon_mutex_open - Retrieve a pointer to the hardware mutex
 *
 * Search the list of registered mutexes for one with the supplied name.
 *
 * The return value will be NULL on failure, and non-NULL otherwise.
 */
alt_mutex_dev* altera_avalon_mutex_open (const char* name)
{
  alt_mutex_dev* dev;

  dev = (alt_mutex_dev*) alt_find_dev (name, &alt_mutex_list);

  if (NULL == dev)
  {
    ALT_ERRNO = ENODEV;
  }

  return dev;
}

/*
 * altera_avalon_mutex_close - Does nothing at the moment, but included for 
 * completeness
 *
 */
void altera_avalon_mutex_close (alt_mutex_dev* dev)
{
  return;
}


/*
 * altera_avalon_mutex_lock - Lock the hardware mutex
 *
 */
void altera_avalon_mutex_lock( alt_mutex_dev* dev, alt_u32 value )
{
  /*
   * When running in a multi threaded environment, obtain the "lock"
   * semaphore. This ensures that reading from the device is thread-safe.
   */

  ALT_SEM_PEND (dev->lock, 0);

  while ( alt_mutex_trylock( dev, value ) != 0);
}


/*
 * altera_avalon_mutex_trylock - Try to lock the hardware mutex
 *
 * returns 0 on success -1 otherwise
 *
 */
int altera_avalon_mutex_trylock( alt_mutex_dev* dev, alt_u32 value )
{
  int ret_code;

  ALT_SEM_PEND (dev->lock, 0);

  ret_code = alt_mutex_trylock( dev, value);

  /*
  * If the try failed then release the thread Mutex  
  */
  if (ret_code)
  {
    ALT_SEM_POST (dev->lock);
  }

  return ret_code;
}


/*
 * altera_avalon_mutex_unlock - Unlock the hardware mutex
 * 
 * This function does not check that you own the Mutex if you try to release
 * a Mutex you do not own the behaviour is undefined
 *
 */
void altera_avalon_mutex_unlock( alt_mutex_dev* dev )
{
  alt_u32 id;
  NIOS2_READ_CPUID(id);

  /*
  * This Mutex has been claimed and released since Reset so clear the Reset bit
  * This MUST happen before we release the MUTEX
  */
  IOWR_ALTERA_AVALON_MUTEX_RESET(dev->mutex_base, 
                                  ALTERA_AVALON_MUTEX_RESET_RESET_MSK);
  IOWR_ALTERA_AVALON_MUTEX_MUTEX(dev->mutex_base, 
                                  id << ALTERA_AVALON_MUTEX_MUTEX_OWNER_OFST);

  /*
  * Now that access to the hardware Mutex is complete, release the thread lock
  */
  ALT_SEM_POST (dev->lock);
}

/*
 * altera_avalon_mutex_is_mine - Do I own the Mutex?
 *
 *  returns non zero if the mutex is owned by this CPU
 */
int altera_avalon_mutex_is_mine( alt_mutex_dev* dev )
{
  alt_u32 id, data, owner, value;
  int ret_code = 0;

  NIOS2_READ_CPUID(id);

  /* retrieve the contents of the mutex */
  data = IORD_ALTERA_AVALON_MUTEX_MUTEX(dev->mutex_base);
  owner = (data & ALTERA_AVALON_MUTEX_MUTEX_OWNER_MSK) >> 
              ALTERA_AVALON_MUTEX_MUTEX_OWNER_OFST;

  if (owner == id) 
  {
    value = (data & ALTERA_AVALON_MUTEX_MUTEX_VALUE_MSK) >> 
                  ALTERA_AVALON_MUTEX_MUTEX_VALUE_OFST;
    if (value != 0)
    {
      ret_code = 1;
    }
  }

  return ret_code;
}


/*
 * altera_avalon_mutex_first_lock
 *
 * Has this Mutex been released since reset if not return 1
 * otherwise return 0
 */
int altera_avalon_mutex_first_lock( alt_mutex_dev* dev )
{
  alt_u32 data;
  int ret_code = 0;

  data = IORD_ALTERA_AVALON_MUTEX_RESET(dev->mutex_base);

  if (data & ALTERA_AVALON_MUTEX_RESET_RESET_MSK) 
  {
    ret_code = 1;
  }

  return ret_code;
}

