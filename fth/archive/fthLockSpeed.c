//----------------------------------------------------------------------------
// ZetaScale
// Copyright (c) 2016, SanDisk Corp. and/or all its affiliates.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published by the Free
// Software Foundation;
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License v2.1 for more details.
//
// A copy of the GNU Lesser General Public License v2.1 is provided with this package and
// can also be found at: http://opensource.org/licenses/LGPL-2.1
// You should have received a copy of the GNU Lesser General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA 02111-1307 USA.
//----------------------------------------------------------------------------

/*
 * File:   fthLockSpeed.c
 * Author: Jim
 *
 * Created on February 29, 2008
 *
 * (c) Copyright 2008, Schooner Information Technology, Inc.
 * http:                                     //www.schoonerinfotech.com/
 *
 * $Id: fthLockSpeed.c 396 2008-02-29 22:55:43Z jim $
 */

//
// Test program for many fth functions
//


#include "fth.h"
#include "fthMbox.h"
#include "fthThread.h"
#include "fthWaitEl.h"
#include "fthMbox.h"
#include "platform/assert.h"
#include "platform/errno.h"
#include "platform/fcntl.h"
#include "platform/logging.h"
#include "platform/shmem.h"
#include "platform/signal.h"
#include "platform/stdlib.h"
#include "platform/string.h"
#include "platform/time.h"
#include "platform/unistd.h"
#include <stdio.h>


fthMbox_t mbox;
fthLock_t lock1;
fthLock_t lock2;
fthLock_t lock3;
fthLock_t lockStart;
uint64_t mem[3];

uint64_t iterations;
    
void threadRoutine1(uint64_t arg) {

    fthWaitEl_t *wait1 = fthLock(&lock1, 1, NULL);
    fthWaitEl_t *wait2, *wait3;
    fthWaitEl_t *waitStart = fthLock(&lockStart, 1, NULL);
    fthUnlock(waitStart);

    for (int i = 0; i < iterations; i++) {
        wait2 = fthLock(&lock2, 1, NULL);
        fthUnlock(wait1);
        wait3 = fthLock(&lock3, 1, NULL);
        fthUnlock(wait2);
        wait1 = fthLock(&lock1, 1, NULL);
        fthUnlock(wait3);
        printf("In %llu/%i %llu\n", (unsigned long long)arg, i,
               (unsigned long long)fthGetSchedulerIdleTime());
    }

}

void threadRoutine2(uint64_t arg) {

    fthWaitEl_t *wait2 = fthLock(&lock2, 1, NULL);
    fthWaitEl_t *wait1, *wait3;
    fthWaitEl_t *waitStart = fthLock(&lockStart, 1, NULL);
    fthUnlock(waitStart);

    for (int i = 0; i < iterations; i++) {
        wait3 = fthLock(&lock3, 1, NULL);
        fthUnlock(wait2);
        wait1 = fthLock(&lock1, 1, NULL);
        fthUnlock(wait3);
        wait2 = fthLock(&lock2, 1, NULL);
        fthUnlock(wait1);
        //        printf("In %i/%i\n", arg, i);
    }

    plat_exit(1);
}

void threadRoutine3(uint64_t arg) {

    fthUnlock((fthWaitEl_t *) arg);
}

#define SHM_SIZE 8 * 1024 *1024

int main(int argc, char **argv) {

    iterations = atoi(argv[1]);

    struct plat_shmem_config *shmem_config = plat_alloc(sizeof(struct plat_shmem_config));
    plat_shmem_config_init(shmem_config);
    shmem_config->size = SHM_SIZE;
    plat_shmem_prototype_init(shmem_config);
    int tmp = plat_shmem_attach(shmem_config->mmap);
    if (tmp) {
        plat_log_msg(20073, PLAT_LOG_CAT_PLATFORM_TEST_SHMEM,
                     PLAT_LOG_LEVEL_FATAL, "shmem_attach(%s) failed: %s",
                     shmem_config->mmap, plat_strerror(-tmp));
        plat_abort();
    }


    fthInit();

    fthLockInit(&lock1);
    fthLockInit(&lock2);
    fthLockInit(&lockStart);

    fthThread_t *thread1 = XSpawn(&threadRoutine1, 4096);
    fthThread_t *thread2 = XSpawn(&threadRoutine2, 4096);
    fthThread_t *thread3 = XSpawn(&threadRoutine3, 4096);

    fthWaitEl_t *wait = fthLock(&lockStart, 1, NULL);

    XResume(thread1, 1);
    XResume(thread2, 2);
    XResume(thread3, (uint64_t) wait);
    

    fthSchedulerPthread(0);

    return (0);
    
}
