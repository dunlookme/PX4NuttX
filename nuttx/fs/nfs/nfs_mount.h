/****************************************************************************
 * fs/nfs/nfs_mount.h
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Copyright (C) 2012 Jose Pablo Rojas Vargas. All rights reserved.
 *   Author: Jose Pablo Rojas Vargas <jrojas@nx-engineering.com>
 *           Gregory Nutt <gnutt@nuttx.org>
 *
 * Leveraged from OpenBSD:
 *
 *  Copyright (c) 1989, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Rick Macklem at The University of Guelph.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __FS_NFS_NFS_MOUNT_H
#define __FS_NFS_NFS_MOUNT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/socket.h>

#include "rpc.h"
 
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Mount structure. One mount structure is allocated for each NFS mount. This
 * structure holds NFS specific information for mount.
 */

struct nfsmount
{
  struct nfsnode  *nm_head;                   /* A list of all files opened on this mountpoint */
  sem_t            nm_sem;                    /* Used to assure thread-safe access */
  nfsfh_t          nm_fh;                     /* File handle of root dir */
  char             nm_path[90];               /* server's path of the directory being mounted */
  struct nfs_fattr nm_fattr;                  /* nfs file attribute cache */
  struct rpcclnt  *nm_rpcclnt;                /* RPC state */
  struct socket   *nm_so;                     /* RPC socket */
  struct sockaddr  nm_nam;                    /* Addr of server */
  bool             nm_mounted;                /* true: The file system is ready */
  uint8_t          nm_flag;                   /* Flags for soft/hard... */
  uint8_t          nm_fhsize;                 /* Size of root file handle (host order) */
  uint8_t          nm_sotype;                 /* Type of socket */
  uint8_t          nm_soproto;                /* and protocol */
  uint8_t          nm_timeo;                  /* Init timer for NFSMNT_DUMBTIMR */
  uint8_t          nm_retry;                  /* Max retries */
  uint16_t         nm_rsize;                  /* Max size of read RPC */
  uint16_t         nm_wsize;                  /* Max size of write RPC */
  uint16_t         nm_readdirsize;            /* Size of a readdir RPC */
  uint8_t          nm_verf[NFSX_V3WRITEVERF]; /* V3 write verifier */

  /* Set aside memory on the stack to hold the largest call message.  NOTE
   * that for the case of the write call message, the reply message is in
   * this union.
   */

  union
  {
    struct rpc_call_pmap    pmap;
    struct rpc_call_mount   mountd;
    struct rpc_call_create  create;
    struct rpc_call_lookup  lookup;
    struct rpc_call_read    read;
    struct rpc_call_remove  removef;
    struct rpc_call_rename  renamef;
    struct rpc_call_mkdir   mkdir;
    struct rpc_call_rmdir   rmdir;
    struct rpc_call_readdir readdir;
    struct rpc_call_fs      fs;
    struct rpc_reply_write  write;
  } nm_smallbuffer;
};

#endif
