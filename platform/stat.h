/*
 * Copyright (c) 2009-2016, SanDisk Corporation. All rights reserved.
 * No use, or distribution, of this source code is permitted in any form or
 * means without a valid, written license agreement with SanDisk Corp.  Please
 * refer to the included End User License Agreement (EULA), "License" or "License.txt" file
 * for terms and conditions regarding the use and redistribution of this software.
 */

#ifndef PLATFORM_STAT_H
#define PLATFORM_STAT_H 1

/*
 * File:   $HeadURL: svn://svn.schoonerinfotech.net/schooner-trunk/trunk/sdf/platform/stat.h $
 * Author: drew
 *
 * Created on February 10, 2008
 *
 * (c) Copyright 2008, Schooner Information Technology, Inc.
 * http://www.schoonerinfotech.com/
 *
 * $Id: stat.h 2051 2008-07-09 11:05:27Z drew $
 */

/*
 * Thin wrappers for unix functions to accomodate interception for
 * 1) Blocking behavior in user-scheduled threads
 * 2) The simulated cluster environment
 */

#include <sys/cdefs.h>
#include <sys/stat.h>

#include "platform/wrap.h"

#define PLAT_STAT_WRAP_FILE_ITEMS()                                            \
    item(int, mkdir, (__const char *__path, __mode_t __mode),                  \
         (__path, __mode), __THROW, __nonnull((1)))                            \
    item(int, lstat, (__const char *__restrict pathname, struct stat *buf),    \
         (pathname, buf), __THROW, __nonnull((1, 2)))                          \
    item(int, fstat, (int fd, struct stat *__restrict buf), (fd, buf),         \
         __THROW, __nonnull((2)))                                              \
    item(int, stat, (__const char *__restrict pathname,                        \
                     struct stat *__restrict buf), (pathname, buf),            \
         __THROW, __nonnull((1, 2)))

#define PLAT_STAT_WRAP_ITEMS()                                                 \
    PLAT_STAT_WRAP_FILE_ITEMS()

__BEGIN_DECLS

#define __leaf__

#define item(ret, sym, declare, call, cppthrow, attributes) \
    PLAT_WRAP_NO_POISON(ret, sym, declare, call, cppthrow, attributes)
PLAT_STAT_WRAP_ITEMS()
#undef item

/*
 * FIXME: The conventional way does not work because Linux has the 64 bit
 * hacks which the headers make fast with extern inline wrappers.
 */

PLAT_WRAP_CPP_POISON(lstat fstat)

__END_DECLS

#endif /* ndef PLATFORM_STAT_H */
