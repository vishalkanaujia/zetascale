/*
 * Copyright (c) 2009-2016, SanDisk Corporation. All rights reserved.
 * No use, or distribution, of this source code is permitted in any form or
 * means without a valid, written license agreement with SanDisk Corp.  Please
 * refer to the included End User License Agreement (EULA), "License" or "License.txt" file
 * for terms and conditions regarding the use and redistribution of this software.
 */

/*
 * File:   utils/hash.h
 * Author: Jim
 *
 * Created on March 3, 2008, 1:38 PM
 *
 * SanDisk Proprietary Material, © Copyright 2012 SanDisk, all rights reserved.
 * http://www.sandisk.com
 *
 * $Id: hash.c 399 2008-02-28 23:46:15Z darpan $
 */

#ifndef __HASH_H
#define __HASH_H

#include <stdint.h>
#include <inttypes.h>

//
// Header file for Bob Tuttles lookup8 hash algorithm
//
// This is fast on 64-bit machines.  Just mask off the bits you need for shorter keys.
//

// Level is an arbitrary salt for the hash.
uint64_t zs_hash(const unsigned char *key, uint64_t keyLength, uint64_t level);

#endif
