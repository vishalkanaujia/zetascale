/*
 * FDF linkage; generated by mkfdfdll.
 * Copyright (c) 2012-2013, SanDisk Corporation.  All rights reserved.
 */
#include <dlfcn.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "fdf.h"
#include "fdf_range.h"


/*
 * Macros.
 */
#define nel(a)      (sizeof(a)/sizeof(*(a)))
#define unlikely(x) __builtin_expect((x), 0)


/*
 * Set once FDF has been loaded.
 */
static int loaded;


/*
 * FDF Library locations.
 */
static char *fdflibs[] ={
    "/usr/lib64/fdf/libbtree.so",
    "/usr/lib/fdf/libbtree.so",
    "/lib64/libbtree.so",
    "/lib/libbtree.so",
    "/usr/local/lib64/libbtree.so",
    "/usr/local/lib/libbtree.so",
    "libbtree.so",
};


/*
 * Function pointers.
 */
static const char *
(*ptr_FDFGetProperty)(const char *key, const char *def);

static void 
(*ptr_FDFSetProperty)(const char *property, const char *value);

static FDF_status_t 
(*ptr_FDFLoadProperties)(const char *prop_file);

static FDF_status_t 
(*ptr_FDFInit)(struct FDF_state **fdf_state);

static FDF_status_t 
(*ptr_FDFInitPerThreadState)(struct FDF_state *fdf_state,
                             struct FDF_thread_state **thd_state);

static FDF_status_t 
(*ptr_FDFReleasePerThreadState)(struct FDF_thread_state **thd_state);

static FDF_status_t 
(*ptr_FDFShutdown)(struct FDF_state *fdf_state);

static FDF_status_t 
(*ptr_FDFLoadCntrPropDefaults)(FDF_container_props_t *props);

static FDF_status_t 
(*ptr_FDFOpenContainer)(struct FDF_thread_state *fdf_thread_state,
                        char *cname,
                        FDF_container_props_t *properties,
                        uint32_t flags,
                        FDF_cguid_t *cguid);

static FDF_status_t 
(*ptr_FDFCloseContainer)(struct FDF_thread_state *fdf_thread_state,
                         FDF_cguid_t cguid);

static FDF_status_t 
(*ptr_FDFDeleteContainer)(struct FDF_thread_state *fdf_thread_state,
                          FDF_cguid_t cguid);

static FDF_status_t 
(*ptr_FDFGetContainers)(struct FDF_thread_state *fdf_thread_state,
                        FDF_cguid_t *cguids,
                        uint32_t *n_cguids);

static FDF_status_t 
(*ptr_FDFGetContainerProps)(struct FDF_thread_state *fdf_thread_state,
                            FDF_cguid_t cguid,
                            FDF_container_props_t *pprops);

static FDF_status_t 
(*ptr_FDFSetContainerProps)(struct FDF_thread_state *fdf_thread_state,
                            FDF_cguid_t cguid,
                            FDF_container_props_t *pprops);

static FDF_status_t 
(*ptr_FDFReadObject)(struct FDF_thread_state *fdf_thread_state,
                     FDF_cguid_t cguid,
                     char *key,
                     uint32_t keylen,
                     char **data,
                     uint64_t *datalen);

static FDF_status_t 
(*ptr_FDFReadObjectExpiry)(struct FDF_thread_state *fdf_thread_state,
                           FDF_cguid_t cguid,
                           FDF_readobject_t *robj);

static FDF_status_t 
(*ptr_FDFFreeBuffer)(char *buf);

static FDF_status_t 
(*ptr_FDFWriteObject)(struct FDF_thread_state *sdf_thread_state,
                      FDF_cguid_t cguid,
                      char *key,
                      uint32_t keylen,
                      char *data,
                      uint64_t datalen,
                      uint32_t flags);

static FDF_status_t 
(*ptr_FDFWriteObjectExpiry)(struct FDF_thread_state *fdf_thread_state,
                            FDF_cguid_t cguid,
                            FDF_writeobject_t *wobj,
                            uint32_t flags);

static FDF_status_t 
(*ptr_FDFDeleteObject)(struct FDF_thread_state *fdf_thread_state,
                       FDF_cguid_t cguid,
                       char *key,
                       uint32_t keylen);

static FDF_status_t 
(*ptr_FDFEnumerateContainerObjects)(struct FDF_thread_state *fdf_thread_state,
                                    FDF_cguid_t cguid,
                                    struct FDF_iterator **iterator);

static FDF_status_t 
(*ptr_FDFNextEnumeratedObject)(struct FDF_thread_state *fdf_thread_state,
                               struct FDF_iterator *iterator,
                               char **key,
                               uint32_t *keylen,
                               char **data,
                               uint64_t *datalen);

static FDF_status_t 
(*ptr_FDFFinishEnumeration)(struct FDF_thread_state *fdf_thread_state,
                            struct FDF_iterator *iterator);

static FDF_status_t 
(*ptr_FDFFlushObject)(struct FDF_thread_state *fdf_thread_state,
                      FDF_cguid_t cguid,
                      char *key,
                      uint32_t keylen);

static FDF_status_t 
(*ptr_FDFFlushContainer)(struct FDF_thread_state *fdf_thread_state,
                         FDF_cguid_t cguid);

static FDF_status_t 
(*ptr_FDFFlushCache)(struct FDF_thread_state *fdf_thread_state);

static FDF_status_t 
(*ptr_FDFGetStats)(struct FDF_thread_state *fdf_thread_state,
                   FDF_stats_t *stats);

static FDF_status_t 
(*ptr_FDFGetContainerStats)(struct FDF_thread_state *fdf_thread_state,
                            FDF_cguid_t cguid,
                            FDF_stats_t *stats);

static char *
(*ptr_FDFStrError)(FDF_status_t fdf_errno);

static FDF_status_t 
(*ptr_FDFMiniTransactionStart)(struct FDF_thread_state *fdf_thread_state);

static FDF_status_t 
(*ptr_FDFTransactionStart)(struct FDF_thread_state *fdf_thread_state);

static FDF_status_t 
(*ptr_FDFMiniTransactionCommit)(struct FDF_thread_state *fdf_thread_state);

static FDF_status_t 
(*ptr_FDFTransactionCommit)(struct FDF_thread_state *fdf_thread_state);

static FDF_status_t 
(*ptr_FDFTransactionRollback)(struct FDF_thread_state *fdf_thread_state);

static FDF_status_t 
(*ptr_FDFGetVersion)(char **str);

static FDF_status_t 
(*ptr_FDFGetRange)(struct FDF_thread_state *fdf_thread_state,
                  FDF_cguid_t              cguid, 
                  FDF_indexid_t            indexid,
                  struct FDF_cursor      **cursor,
                  FDF_range_meta_t        *rmeta);

static FDF_status_t
(*ptr_FDFGetNextRange)(struct FDF_thread_state *thrd_state,  
                       struct FDF_cursor       *cursor,
                       int                      n_in, 
                       int                     *n_out,
                       FDF_range_data_t        *values);

static FDF_status_t 
(*ptr_FDFGetRangeFinish)(struct FDF_thread_state *thrd_state, 
                         struct FDF_cursor *cursor);

#if 0
static void 
(*ptr_FDFTLMapDestroy)(struct FDFTLMap *pm);

static void 
(*ptr_FDFTLMapClear)(struct FDFTLMap *pm);

static struct FDFTLMapEntry *
(*ptr_FDFTLMapCreate)(struct FDFTLMap *pm,
                      char *pkey,
                      uint32_t keylen,
                      char *pdata,
                      uint64_t datalen);

static struct FDFTLMapEntry *
(*ptr_FDFTLMapUpdate)(struct FDFTLMap *pm,
                      char *pkey,
                      uint32_t keylen,
                      char *pdata,
                      uint64_t datalen);

static struct FDFTLMapEntry *
(*ptr_FDFTLMapSet)(struct FDFTLMap *pm,
                   char *pkey,
                   uint32_t keylen,
                   char *pdata,
                   uint64_t datalen,
                   char **old_pdata,
                   uint64_t *old_datalen);

static struct FDFTLMapEntry *
(*ptr_FDFTLMapGet)(struct FDFTLMap *pc,
                   char *key,
                   uint32_t keylen,
                   char **data,
                   uint64_t *pdatalen);

static int 
(*ptr_FDFTLMapIncrRefcnt)(struct FDFTLMap *pm, char *key, uint32_t keylen);

static void 
(*ptr_FDFTLMapCheckRefcnts)(struct FDFTLMap *pm);

static int 
(*ptr_FDFTLMapRelease)(struct FDFTLMap *pm, char *key, uint32_t keylen);

static int 
(*ptr_FDFTLMapReleaseEntry)(struct FDFTLMap *pm, struct FDFTLMapEntry *pme);

static struct FDFTLIterator *
(*ptr_FDFTLMapEnum)(struct FDFTLMap *pm);

static void 
(*ptr_FDFTLFinishEnum)(struct FDFTLMap *pm, struct FDFTLIterator *iterator);

static int 
(*ptr_FDFTLMapNextEnum)(struct FDFTLMap *pm,
                        struct FDFTLIterator *iterator,
                        char **key,
                        uint32_t *keylen,
                        char **data,
                        uint64_t *datalen);

static int 
(*ptr_FDFTLMapDelete)(struct FDFTLMap *pm, char *key, uint32_t keylen);

struct FDFTLMap*
(*ptr_FDFTLMapInit)(uint64_t nbuckets, uint64_t max_entries, char use_locks, void (*replacement_callback)(void *callback_data, char *key, uint32_t keylen, char *pdata, uint64_t datalen), void *replacement_callback_data);
#endif

/*
 * Linkage table.
 */
static struct {
    const char *name;
    void       *func;
} table[] ={
    { "_FDFGetProperty",                &ptr_FDFGetProperty               },
    { "_FDFSetProperty",                &ptr_FDFSetProperty               },
    { "_FDFLoadProperties",             &ptr_FDFLoadProperties            },
    { "_FDFInit",                       &ptr_FDFInit                      },
    { "_FDFInitPerThreadState",         &ptr_FDFInitPerThreadState        },
    { "_FDFReleasePerThreadState",      &ptr_FDFReleasePerThreadState     },
    { "_FDFShutdown",                   &ptr_FDFShutdown                  },
    { "_FDFLoadCntrPropDefaults",       &ptr_FDFLoadCntrPropDefaults      },
    { "_FDFOpenContainer",              &ptr_FDFOpenContainer             },
    { "_FDFCloseContainer",             &ptr_FDFCloseContainer            },
    { "_FDFDeleteContainer",            &ptr_FDFDeleteContainer           },
    { "_FDFGetContainers",              &ptr_FDFGetContainers             },
    { "_FDFGetContainerProps",          &ptr_FDFGetContainerProps         },
    { "_FDFSetContainerProps",          &ptr_FDFSetContainerProps         },
    { "_FDFReadObject",                 &ptr_FDFReadObject                },
    { "_FDFReadObjectExpiry",           &ptr_FDFReadObjectExpiry          },
    { "_FDFFreeBuffer",                 &ptr_FDFFreeBuffer                },
    { "_FDFWriteObject",                &ptr_FDFWriteObject               },
    { "_FDFWriteObjectExpiry",          &ptr_FDFWriteObjectExpiry         },
    { "_FDFDeleteObject",               &ptr_FDFDeleteObject              },
    { "_FDFEnumerateContainerObjects",  &ptr_FDFEnumerateContainerObjects },
    { "_FDFNextEnumeratedObject",       &ptr_FDFNextEnumeratedObject      },
    { "_FDFFinishEnumeration",          &ptr_FDFFinishEnumeration         },
    { "_FDFFlushObject",                &ptr_FDFFlushObject               },
    { "_FDFFlushContainer",             &ptr_FDFFlushContainer            },
    { "_FDFFlushCache",                 &ptr_FDFFlushCache                },
    { "_FDFGetStats",                   &ptr_FDFGetStats                  },
    { "_FDFGetContainerStats",          &ptr_FDFGetContainerStats         },
    { "_FDFStrError",                   &ptr_FDFStrError                  },
    { "_FDFMiniTransactionStart",       &ptr_FDFMiniTransactionStart      },
    { "_FDFTransactionStart",           &ptr_FDFTransactionStart          },
    { "_FDFMiniTransactionCommit",      &ptr_FDFMiniTransactionCommit     },
    { "_FDFTransactionCommit",          &ptr_FDFTransactionCommit         },
    { "_FDFTransactionRollback",        &ptr_FDFTransactionRollback       },
    { "_FDFGetVersion",                 &ptr_FDFGetVersion                },
    { "_FDFGetRange",                   &ptr_FDFGetRange                  },
    { "_FDFGetNextRange",               &ptr_FDFGetNextRange              },
    { "_FDFGetRangeFinish",             &ptr_FDFGetRangeFinish            },
#if 0
    { "_FDFTLMapDestroy",               &ptr_FDFTLMapDestroy              },
    { "_FDFTLMapClear",                 &ptr_FDFTLMapClear                },
    { "_FDFTLMapCreate",                &ptr_FDFTLMapCreate               },
    { "_FDFTLMapUpdate",                &ptr_FDFTLMapUpdate               },
    { "_FDFTLMapSet",                   &ptr_FDFTLMapSet                  },
    { "_FDFTLMapGet",                   &ptr_FDFTLMapGet                  },
    { "_FDFTLMapIncrRefcnt",            &ptr_FDFTLMapIncrRefcnt           },
    { "_FDFTLMapCheckRefcnts",          &ptr_FDFTLMapCheckRefcnts         },
    { "_FDFTLMapRelease",               &ptr_FDFTLMapRelease              },
    { "_FDFTLMapReleaseEntry",          &ptr_FDFTLMapReleaseEntry         },
    { "_FDFTLMapEnum",                  &ptr_FDFTLMapEnum                 },
    { "_FDFTLFinishEnum",               &ptr_FDFTLFinishEnum              },
    { "_FDFTLMapNextEnum",              &ptr_FDFTLMapNextEnum             },
    { "_FDFTLMapDelete",                &ptr_FDFTLMapDelete               },
    { "_FDFTLMapInit",                  &ptr_FDFTLMapInit                 },
#endif
};


/*
 * Print out an error message and exit.
 */
static void
panic(char *fmt, ...)
{
    va_list alist;

    va_start(alist, fmt);
    vfprintf(stderr, fmt, alist);
    va_end(alist);
    fprintf(stderr, "\n");
    exit(1);
}


/*
 * An undefined symbol was found.
 */
static void
undefined(char *sym)
{
    panic("FDF: undefined symbol: %s", sym);
}


/*
 * Determine if the string ends with "No such file or directory".
 */
static int
nsfod(char *str)
{
    char *err = "No such file or directory";
    int  elen = strlen(err);
    int  slen = strlen(str);

    if (slen < elen)
        return 0;
    return !strcmp(str+slen-elen, err);
}


/*
 * Given a pathname, assume it is a FDF library and load it.
 */
static int
load(char *path)
{
    int i;
    void  *dl = dlopen(path, RTLD_NOW | RTLD_DEEPBIND);
    char *err = dlerror();

    if (!dl) {
        if (nsfod(err))
            return 0;
        panic("%s", err);
    }
    
    int n = nel(table);
    for (i = 0; i < n; i++) {
        const char *name = table[i].name;
        void *func = dlsym(dl, name);
        if (func)
            *(void **)table[i].func = func;
        else
            fprintf(stderr, "warning: FDF: undefined symbol: %s\n", name);
    }
    return 1;
}


/*
 * Cause the given library to be loaded.
 */
static void
dlo(const char *lib)
{
    void *dl = dlopen(lib, RTLD_NOW|RTLD_GLOBAL);
    if (!dl) {
        char *err = dlerror();
        fprintf(stderr, "%s\n", err);
        exit(1);
    }
}


/*
 * If the FDF library is not already loaded, attempt to find it and then
 * load it.
 */
static void
parse(void)
{
    if (loaded)
        return;
    loaded = 1;

    dlo("librt.so");
    dlo("libaio.so");

    char *lib = getenv("BTREE_LIB");
    if (lib) {
        if (load(lib))
            return;
        panic("cannot find BTREE_LIB=%s", lib);
    }

    if (load("/usr/lib64/fdf/libbtree.so"))
        return;

    int i;
    for (i = 0; i < nel(fdflibs); i++)
        if (load(fdflibs[i]))
            return;
    panic("cannot find libbtree.so");
}


/*
 * FDFGetProperty
 */
const char *
FDFGetProperty(const char *key, const char *def)
{
    parse();
    if (unlikely(!ptr_FDFGetProperty))
        undefined("FDFGetProperty");

    return (*ptr_FDFGetProperty)(key, def);
}


/*
 * FDFSetProperty
 */
void 
FDFSetProperty(const char *property, const char *value)
{
    parse();
    if (unlikely(!ptr_FDFSetProperty))
        undefined("FDFSetProperty");

    return (*ptr_FDFSetProperty)(property, value);
}


/*
 * FDFLoadProperties
 */
FDF_status_t 
FDFLoadProperties(const char *prop_file)
{
    parse();
    if (unlikely(!ptr_FDFLoadProperties))
        undefined("FDFLoadProperties");

    return (*ptr_FDFLoadProperties)(prop_file);
}


/*
 * FDFInit
 */
FDF_status_t 
FDFInit(struct FDF_state **fdf_state)
{
    parse();
    if (unlikely(!ptr_FDFInit))
        undefined("FDFInit");

    return (*ptr_FDFInit)(fdf_state);
}


/*
 * FDFInitPerThreadState
 */
FDF_status_t 
FDFInitPerThreadState(struct FDF_state *fdf_state,
                      struct FDF_thread_state **thd_state)
{
    if (unlikely(!ptr_FDFInitPerThreadState))
        undefined("FDFInitPerThreadState");

    return (*ptr_FDFInitPerThreadState)(fdf_state, thd_state);
}


/*
 * FDFReleasePerThreadState
 */
FDF_status_t 
FDFReleasePerThreadState(struct FDF_thread_state **thd_state)
{
    if (unlikely(!ptr_FDFReleasePerThreadState))
        undefined("FDFReleasePerThreadState");

    return (*ptr_FDFReleasePerThreadState)(thd_state);
}


/*
 * FDFShutdown
 */
FDF_status_t 
FDFShutdown(struct FDF_state *fdf_state)
{
    if (unlikely(!ptr_FDFShutdown))
        undefined("FDFShutdown");

    return (*ptr_FDFShutdown)(fdf_state);
}


/*
 * FDFLoadCntrPropDefaults
 */
FDF_status_t 
FDFLoadCntrPropDefaults(FDF_container_props_t *props)
{
    if (unlikely(!ptr_FDFLoadCntrPropDefaults))
        undefined("FDFLoadCntrPropDefaults");

    return (*ptr_FDFLoadCntrPropDefaults)(props);
}


/*
 * FDFOpenContainer
 */
FDF_status_t 
FDFOpenContainer(struct FDF_thread_state *fdf_thread_state,
                 char *cname,
                 FDF_container_props_t *properties,
                 uint32_t flags,
                 FDF_cguid_t *cguid)
{
    if (unlikely(!ptr_FDFOpenContainer))
        undefined("FDFOpenContainer");

    return (*ptr_FDFOpenContainer)(fdf_thread_state,
                                   cname,
                                   properties,
                                   flags,
                                   cguid);
}


/*
 * FDFCloseContainer
 */
FDF_status_t 
FDFCloseContainer(struct FDF_thread_state *fdf_thread_state,
                  FDF_cguid_t cguid)
{
    if (unlikely(!ptr_FDFCloseContainer))
        undefined("FDFCloseContainer");

    return (*ptr_FDFCloseContainer)(fdf_thread_state, cguid);
}


/*
 * FDFDeleteContainer
 */
FDF_status_t 
FDFDeleteContainer(struct FDF_thread_state *fdf_thread_state,
                   FDF_cguid_t cguid)
{
    if (unlikely(!ptr_FDFDeleteContainer))
        undefined("FDFDeleteContainer");

    return (*ptr_FDFDeleteContainer)(fdf_thread_state, cguid);
}


/*
 * FDFGetContainers
 */
FDF_status_t 
FDFGetContainers(struct FDF_thread_state *fdf_thread_state,
                 FDF_cguid_t *cguids,
                 uint32_t *n_cguids)
{
    if (unlikely(!ptr_FDFGetContainers))
        undefined("FDFGetContainers");

    return (*ptr_FDFGetContainers)(fdf_thread_state, cguids, n_cguids);
}


/*
 * FDFGetContainerProps
 */
FDF_status_t 
FDFGetContainerProps(struct FDF_thread_state *fdf_thread_state,
                     FDF_cguid_t cguid,
                     FDF_container_props_t *pprops)
{
    if (unlikely(!ptr_FDFGetContainerProps))
        undefined("FDFGetContainerProps");

    return (*ptr_FDFGetContainerProps)(fdf_thread_state, cguid, pprops);
}


/*
 * FDFSetContainerProps
 */
FDF_status_t 
FDFSetContainerProps(struct FDF_thread_state *fdf_thread_state,
                     FDF_cguid_t cguid,
                     FDF_container_props_t *pprops)
{
    if (unlikely(!ptr_FDFSetContainerProps))
        undefined("FDFSetContainerProps");

    return (*ptr_FDFSetContainerProps)(fdf_thread_state, cguid, pprops);
}


/*
 * FDFReadObject
 */
FDF_status_t 
FDFReadObject(struct FDF_thread_state *fdf_thread_state,
              FDF_cguid_t cguid,
              char *key,
              uint32_t keylen,
              char **data,
              uint64_t *datalen)
{
    if (unlikely(!ptr_FDFReadObject))
        undefined("FDFReadObject");

    return (*ptr_FDFReadObject)(fdf_thread_state,
                                cguid,
                                key,
                                keylen,
                                data,
                                datalen);
}


/*
 * FDFReadObjectExpiry
 */
FDF_status_t 
FDFReadObjectExpiry(struct FDF_thread_state *fdf_thread_state,
                    FDF_cguid_t cguid,
                    FDF_readobject_t *robj)
{
    if (unlikely(!ptr_FDFReadObjectExpiry))
        undefined("FDFReadObjectExpiry");

    return (*ptr_FDFReadObjectExpiry)(fdf_thread_state, cguid, robj);
}


/*
 * FDFFreeBuffer
 */
FDF_status_t 
FDFFreeBuffer(char *buf)
{
    if (unlikely(!ptr_FDFFreeBuffer))
        undefined("FDFFreeBuffer");

    return (*ptr_FDFFreeBuffer)(buf);
}


/*
 * FDFWriteObject
 */
FDF_status_t 
FDFWriteObject(struct FDF_thread_state *sdf_thread_state,
               FDF_cguid_t cguid,
               char *key,
               uint32_t keylen,
               char *data,
               uint64_t datalen,
               uint32_t flags)
{
    if (unlikely(!ptr_FDFWriteObject))
        undefined("FDFWriteObject");

    return (*ptr_FDFWriteObject)(sdf_thread_state,
                                 cguid,
                                 key,
                                 keylen,
                                 data,
                                 datalen,
                                 flags);
}


/*
 * FDFWriteObjectExpiry
 */
FDF_status_t 
FDFWriteObjectExpiry(struct FDF_thread_state *fdf_thread_state,
                     FDF_cguid_t cguid,
                     FDF_writeobject_t *wobj,
                     uint32_t flags)
{
    if (unlikely(!ptr_FDFWriteObjectExpiry))
        undefined("FDFWriteObjectExpiry");

    return (*ptr_FDFWriteObjectExpiry)(fdf_thread_state, cguid, wobj, flags);
}


/*
 * FDFDeleteObject
 */
FDF_status_t 
FDFDeleteObject(struct FDF_thread_state *fdf_thread_state,
                FDF_cguid_t cguid,
                char *key,
                uint32_t keylen)
{
    if (unlikely(!ptr_FDFDeleteObject))
        undefined("FDFDeleteObject");

    return (*ptr_FDFDeleteObject)(fdf_thread_state, cguid, key, keylen);
}


/*
 * FDFEnumerateContainerObjects
 */
FDF_status_t 
FDFEnumerateContainerObjects(struct FDF_thread_state *fdf_thread_state,
                             FDF_cguid_t cguid,
                             struct FDF_iterator **iterator)
{
    if (unlikely(!ptr_FDFEnumerateContainerObjects))
        undefined("FDFEnumerateContainerObjects");

    return (*ptr_FDFEnumerateContainerObjects)(fdf_thread_state,
                                               cguid,
                                               iterator);
}


/*
 * FDFNextEnumeratedObject
 */
FDF_status_t 
FDFNextEnumeratedObject(struct FDF_thread_state *fdf_thread_state,
                        struct FDF_iterator *iterator,
                        char **key,
                        uint32_t *keylen,
                        char **data,
                        uint64_t *datalen)
{
    if (unlikely(!ptr_FDFNextEnumeratedObject))
        undefined("FDFNextEnumeratedObject");

    return (*ptr_FDFNextEnumeratedObject)(fdf_thread_state,
                                          iterator,
                                          key,
                                          keylen,
                                          data,
                                          datalen);
}


/*
 * FDFFinishEnumeration
 */
FDF_status_t 
FDFFinishEnumeration(struct FDF_thread_state *fdf_thread_state,
                     struct FDF_iterator *iterator)
{
    if (unlikely(!ptr_FDFFinishEnumeration))
        undefined("FDFFinishEnumeration");

    return (*ptr_FDFFinishEnumeration)(fdf_thread_state, iterator);
}


/*
 * FDFFlushObject
 */
FDF_status_t 
FDFFlushObject(struct FDF_thread_state *fdf_thread_state,
               FDF_cguid_t cguid,
               char *key,
               uint32_t keylen)
{
    if (unlikely(!ptr_FDFFlushObject))
        undefined("FDFFlushObject");

    return (*ptr_FDFFlushObject)(fdf_thread_state, cguid, key, keylen);
}


/*
 * FDFFlushContainer
 */
FDF_status_t 
FDFFlushContainer(struct FDF_thread_state *fdf_thread_state,
                  FDF_cguid_t cguid)
{
    if (unlikely(!ptr_FDFFlushContainer))
        undefined("FDFFlushContainer");

    return (*ptr_FDFFlushContainer)(fdf_thread_state, cguid);
}


/*
 * FDFFlushCache
 */
FDF_status_t 
FDFFlushCache(struct FDF_thread_state *fdf_thread_state)
{
    if (unlikely(!ptr_FDFFlushCache))
        undefined("FDFFlushCache");

    return (*ptr_FDFFlushCache)(fdf_thread_state);
}


/*
 * FDFGetStats
 */
FDF_status_t 
FDFGetStats(struct FDF_thread_state *fdf_thread_state, FDF_stats_t *stats)
{
    if (unlikely(!ptr_FDFGetStats))
        undefined("FDFGetStats");

    return (*ptr_FDFGetStats)(fdf_thread_state, stats);
}


/*
 * FDFGetContainerStats
 */
FDF_status_t 
FDFGetContainerStats(struct FDF_thread_state *fdf_thread_state,
                     FDF_cguid_t cguid,
                     FDF_stats_t *stats)
{
    if (unlikely(!ptr_FDFGetContainerStats))
        undefined("FDFGetContainerStats");

    return (*ptr_FDFGetContainerStats)(fdf_thread_state, cguid, stats);
}


/*
 * FDFStrError
 */
char *
FDFStrError(FDF_status_t fdf_errno)
{
    if (unlikely(!ptr_FDFStrError))
        undefined("FDFStrError");

    return (*ptr_FDFStrError)(fdf_errno);
}


/*
 * FDFMiniTransactionStart
 */
FDF_status_t 
FDFMiniTransactionStart(struct FDF_thread_state *fdf_thread_state)
{
    if (unlikely(!ptr_FDFMiniTransactionStart))
        undefined("FDFMiniTransactionStart");

    return (*ptr_FDFMiniTransactionStart)(fdf_thread_state);
}


/*
 * FDFTransactionStart
 */
FDF_status_t 
FDFTransactionStart(struct FDF_thread_state *fdf_thread_state)
{
    if (unlikely(!ptr_FDFTransactionStart))
        undefined("FDFTransactionStart");

    return (*ptr_FDFTransactionStart)(fdf_thread_state);
}


/*
 * FDFMiniTransactionCommit
 */
FDF_status_t 
FDFMiniTransactionCommit(struct FDF_thread_state *fdf_thread_state)
{
    if (unlikely(!ptr_FDFMiniTransactionCommit))
        undefined("FDFMiniTransactionCommit");

    return (*ptr_FDFMiniTransactionCommit)(fdf_thread_state);
}


/*
 * FDFTransactionCommit
 */
FDF_status_t 
FDFTransactionCommit(struct FDF_thread_state *fdf_thread_state)
{
    if (unlikely(!ptr_FDFTransactionCommit))
        undefined("FDFTransactionCommit");

    return (*ptr_FDFTransactionCommit)(fdf_thread_state);
}

/*
 * FDFTransactionRollback
 */
FDF_status_t 
FDFTransactionRollback(struct FDF_thread_state *fdf_thread_state)
{
    if (unlikely(!ptr_FDFTransactionRollback))
        undefined("FDFTransactionRollback");

    return (*ptr_FDFTransactionRollback)(fdf_thread_state);
}

/*
 * FDFGetVersion
 */
FDF_status_t 
FDFGetVersion(char **str)
{
    parse();
    if (unlikely(!ptr_FDFGetVersion))
        undefined("FDFGetVersion");

    return (*ptr_FDFGetVersion)(str);
}

/*
 * FDFGetRange
 */
FDF_status_t
FDFGetRange(struct FDF_thread_state *fdf_thread_state,
            FDF_cguid_t              cguid, 
            FDF_indexid_t            indexid,
            struct FDF_cursor      **cursor,
            FDF_range_meta_t        *rmeta)
{
    if (unlikely(!ptr_FDFGetRange))
        undefined("FDFGetRange");

    return (*ptr_FDFGetRange)(fdf_thread_state, cguid, indexid, cursor, rmeta);
}

/*
 * FDFGetNextRange
 */
FDF_status_t
FDFGetNextRange(struct FDF_thread_state *fdf_thread_state,  
                struct FDF_cursor       *cursor,
                int                      n_in, 
                int                     *n_out,
                FDF_range_data_t        *values)
{
    if (unlikely(!ptr_FDFGetNextRange))
        undefined("FDFGetNextRange");

    return (*ptr_FDFGetNextRange)(fdf_thread_state, cursor, n_in, n_out, values);
}

/*
 * FDFGetRangeFinish
 */
FDF_status_t 
FDFGetRangeFinish(struct FDF_thread_state *fdf_thread_state, 
                  struct FDF_cursor *cursor)
{
    if (unlikely(!ptr_FDFGetRangeFinish))
        undefined("FDFGetRangeFinish");

    return (*ptr_FDFGetRangeFinish)(fdf_thread_state, cursor);
}

#if 0
/*
 * FDFTLMapDestroy
 */
void 
FDFTLMapDestroy(struct FDFTLMap *pm)
{
    if (unlikely(!ptr_FDFTLMapDestroy))
        undefined("FDFTLMapDestroy");

    return (*ptr_FDFTLMapDestroy)(pm);
}

/*
 * FDFTLMapClear
 */
void 
FDFTLMapClear(struct FDFTLMap *pm)
{
    if (unlikely(!ptr_FDFTLMapClear))
        undefined("FDFTLMapClear");

    return (*ptr_FDFTLMapClear)(pm);
}

/*
 * FDFTLMapCreate
 */
struct FDFTLMapEntry *
FDFTLMapCreate(struct FDFTLMap *pm,
               char *pkey,
               uint32_t keylen,
               char *pdata,
               uint64_t datalen)
{
    if (unlikely(!ptr_FDFTLMapCreate))
        undefined("FDFTLMapCreate");

    return (*ptr_FDFTLMapCreate)(pm, pkey, keylen, pdata, datalen);
}


/*
 * FDFTLMapUpdate
 */
struct FDFTLMapEntry *
FDFTLMapUpdate(struct FDFTLMap *pm,
               char *pkey,
               uint32_t keylen,
               char *pdata,
               uint64_t datalen)
{
    if (unlikely(!ptr_FDFTLMapUpdate))
        undefined("FDFTLMapUpdate");

    return (*ptr_FDFTLMapUpdate)(pm, pkey, keylen, pdata, datalen);
}


/*
 * FDFTLMapSet
 */
struct FDFTLMapEntry *
FDFTLMapSet(struct FDFTLMap *pm,
            char *pkey,
            uint32_t keylen,
            char *pdata,
            uint64_t datalen,
            char **old_pdata,
            uint64_t *old_datalen)
{
    if (unlikely(!ptr_FDFTLMapSet))
        undefined("FDFTLMapSet");

    return (*ptr_FDFTLMapSet)(pm,
                              pkey,
                              keylen,
                              pdata,
                              datalen,
                              old_pdata,
                              old_datalen);
}


/*
 * FDFTLMapGet
 */
struct FDFTLMapEntry *
FDFTLMapGet(struct FDFTLMap *pc,
            char *key,
            uint32_t keylen,
            char **data,
            uint64_t *pdatalen)
{
    if (unlikely(!ptr_FDFTLMapGet))
        undefined("FDFTLMapGet");

    return (*ptr_FDFTLMapGet)(pc, key, keylen, data, pdatalen);
}


/*
 * FDFTLMapIncrRefcnt
 */
int 
FDFTLMapIncrRefcnt(struct FDFTLMap *pm, char *key, uint32_t keylen)
{
    if (unlikely(!ptr_FDFTLMapIncrRefcnt))
        undefined("FDFTLMapIncrRefcnt");

    return (*ptr_FDFTLMapIncrRefcnt)(pm, key, keylen);
}


/*
 * FDFTLMapCheckRefcnts
 */
void 
FDFTLMapCheckRefcnts(struct FDFTLMap *pm)
{
    if (unlikely(!ptr_FDFTLMapCheckRefcnts))
        undefined("FDFTLMapCheckRefcnts");

    return (*ptr_FDFTLMapCheckRefcnts)(pm);
}


/*
 * FDFTLMapRelease
 */
int 
FDFTLMapRelease(struct FDFTLMap *pm, char *key, uint32_t keylen)
{
    if (unlikely(!ptr_FDFTLMapRelease))
        undefined("FDFTLMapRelease");

    return (*ptr_FDFTLMapRelease)(pm, key, keylen);
}


/*
 * FDFTLMapReleaseEntry
 */
int 
FDFTLMapReleaseEntry(struct FDFTLMap *pm, struct FDFTLMapEntry *pme)
{
    if (unlikely(!ptr_FDFTLMapReleaseEntry))
        undefined("FDFTLMapReleaseEntry");

    return (*ptr_FDFTLMapReleaseEntry)(pm, pme);
}


/*
 * FDFTLMapEnum
 */
struct FDFTLIterator *
FDFTLMapEnum(struct FDFTLMap *pm)
{
    if (unlikely(!ptr_FDFTLMapEnum))
        undefined("FDFTLMapEnum");

    return (*ptr_FDFTLMapEnum)(pm);
}


/*
 * FDFTLFinishEnum
 */
void 
FDFTLFinishEnum(struct FDFTLMap *pm, struct FDFTLIterator *iterator)
{
    if (unlikely(!ptr_FDFTLFinishEnum))
        undefined("FDFTLFinishEnum");

    return (*ptr_FDFTLFinishEnum)(pm, iterator);
}


/*
 * FDFTLMapNextEnum
 */
int 
FDFTLMapNextEnum(struct FDFTLMap *pm,
                 struct FDFTLIterator *iterator,
                 char **key,
                 uint32_t *keylen,
                 char **data,
                 uint64_t *datalen)
{
    if (unlikely(!ptr_FDFTLMapNextEnum))
        undefined("FDFTLMapNextEnum");

    return (*ptr_FDFTLMapNextEnum)(pm, iterator, key, keylen, data, datalen);
}


/*
 * FDFTLMapDelete
 */
int 
FDFTLMapDelete(struct FDFTLMap *pm, char *key, uint32_t keylen)
{
    if (unlikely(!ptr_FDFTLMapDelete))
        undefined("FDFTLMapDelete");

    return (*ptr_FDFTLMapDelete)(pm, key, keylen);
}

/*
 * FDFTLMapInit
 */
struct FDFTLMap*
FDFTLMapInit(uint64_t nbuckets, uint64_t max_entries, char use_locks, void (*replacement_callback)(void *callback_data, char *key, uint32_t keylen, char *pdata, uint64_t datalen), void *replacement_callback_data)
{
    if (unlikely(!ptr_FDFTLMapInit))
        undefined("FDFTLMapInit");

    return (*ptr_FDFTLMapInit)(nbuckets, max_entries, use_locks, replacement_callback, replacement_callback_data);
}
#endif
