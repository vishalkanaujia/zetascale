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
 * File: msg_sdf.c
 * Author: Johann George.
 * (c) Copyright 2009, Schooner Information Technology, Inc.
 *
 * Allow SDF to initialize the messaging system.
 */
#include <mpi.h>
#include <stdio.h>
#include "trace.h"
#include "msg_map.h"
#include "msg_msg.h"
#include "msg_sdf.h"
#include "platform/alloc.h"
#include "sdfmsg/sdf_msg_types.h"


/*
 * Static variables.
 */
static int NoRanks;


/*
 * Global variables.
 */
int MeRank;
int NewMsg;


/*
 * Initialize.
 */
void
msg_sdf_init(msg_sdf_init_t *sdf)
{
    int myrank = -1;

    MeRank = -1;
    msgt_init(sdf->debug);

    if (!sdf->nodes)
        sdf->nodes = 1;

    NewMsg = 1;
    msg_init(&sdf->init);
    myrank = sdf->rank;
    msg_map_init(-1, sdf->nodes, &myrank);
    MeRank = myrank;
    NoRanks = sdf->nodes;
    sdf_msg_init_mpi(sdf->argc, sdf->argv, 0);

    sdf->rank  = MeRank;
    sdf->nodes = NoRanks;
}


/*
 * Exit.
 */
void
msg_sdf_exit(void)
{
    if (NewMsg) {
        msg_map_exit();
        msg_exit();
    } else {
        sched_yield();
        MPI_Finalize();
    }
}


/*
 * Wait for messages to complete.
 */
void
msg_sdf_wait(void)
{
    sdf_msg_stopmsg(MeRank, SYS_SHUTDOWN_SELF);
}


/*
 * Return my rank.
 */
int
msg_sdf_myrank(void)
{
    return MeRank;
}


/*
 * Return the number of ranks.
 */
int
msg_sdf_numranks(void)
{
    if (NewMsg)
        return msg_map_numranks();
    return NoRanks;
}


/*
 * Return the lowest rank.
 */
int
msg_sdf_lowrank(void)
{
    if (NewMsg)
        return msg_map_lowrank();
    return 0;
}


/*
 * Return a sorted list of the valid ranks.
 */
int *
msg_sdf_ranks(int *np)
{
    int i;
    int *ranks;

    if (NewMsg)
        return msg_map_ranks(np);

    ranks = q_malloc((NoRanks+1) * sizeof(ranks[0]));
    for (i = 0; i < NoRanks; i++)
        ranks[i] = i;
    ranks[NoRanks] = -1;
    if (np)
        *np = NoRanks;
    return ranks;
}
