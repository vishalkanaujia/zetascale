/*
 * Copyright (c) 2009-2016, SanDisk Corporation. All rights reserved.
 * No use, or distribution, of this source code is permitted in any form or
 * means without a valid, written license agreement with SanDisk Corp.  Please
 * refer to the included End User License Agreement (EULA), "License" or "License.txt" file
 * for terms and conditions regarding the use and redistribution of this software.
 */

/*
 * File:   tlmap.c
 * Author: Brian O'Krafka
 *
 * Created on September 11, 2008
 *
 * (c) Copyright 2008, Schooner Information Technology, Inc.
 * http://www.schoonerinfotech.com/
 *
 * $Id: tlmap.c 308 2008-02-20 22:34:58Z tomr $
 */

#define _TLMAP_C

#include <stdint.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tlmap.h"
#include "hash.h"

void SDFTLMapInit(SDFTLMap_t *pm, uint64_t nbuckets, 
     int (*print_fn)(SDFTLMapEntry_t *pce, char *sout, int max_len))
{
    uint64_t          i;

    pm->nbuckets      = nbuckets;
    pm->print_fn      = print_fn;
    pm->buckets       = malloc(nbuckets*(sizeof(SDFTLMapBucket_t)));

    for (i=0; i<nbuckets; i++) {
	pm->buckets[i].entry = NULL;
    }
}

SDFTLMapEntry_t *SDFTLMapGetCreate(SDFTLMap_t *pm, char *pkey)
{
    int                keylen;
    uint64_t           h;
    SDFTLMapEntry_t   *pme;
    SDFTLMapBucket_t  *pb;

    keylen = strlen(pkey);
    h = hash((const unsigned char *) pkey, strlen(pkey), 0) % pm->nbuckets;
    pb = &(pm->buckets[h]);

    for (pme = pb->entry; pme != NULL; pme = pme->next) {
	if ((pme->keylen == keylen) && 
	    (strcmp((const char *) pme->key, (const char *) pkey) == 0))
	{
	    break;
	}
    }

    if (pme == NULL) {

        /* Create a new entry. */

	pme = (SDFTLMapEntry_t *) malloc(sizeof(SDFTLMapEntry_t));
	if (pme == NULL) {
	    (void) fprintf(stderr, "Could not allocate a thread-local map entry.");
	    exit(1);
	}

	pme->contents = NULL;
	pme->key      = (char *) malloc(keylen+1);
	strcpy(pme->key, pkey);
	pme->keylen   = keylen;

	/* put myself on the bucket list */
        pme->next = pb->entry;
        pb->entry = pme;
    }

    return(pme);
}

SDFTLMapEntry_t *SDFTLMapGet(SDFTLMap_t *pm, char *pkey)
{
    int                keylen;
    uint64_t           h;
    SDFTLMapEntry_t   *pme;
    SDFTLMapBucket_t  *pb;

    keylen = strlen(pkey);
    h = hash((const unsigned char *) pkey, strlen(pkey), 0) % pm->nbuckets;
    pb = &(pm->buckets[h]);

    for (pme = pb->entry; pme != NULL; pme = pme->next) {
	if ((pme->keylen == keylen) && 
	    (strcmp((const char *) pme->key, (const char *) pkey) == 0))
	{
	    break;
	}
    }

    return(pme);
}

