#include <umf/mempolicy.h>
#include <umf/memspace.h>

#include <umf/pools/pool_jemalloc.h>
#include <stdbool.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include <umf/ipc.h>
#include <umf/memory_pool.h>
#include <umf/pools/pool_proxy.h>
#include <umf/pools/pool_scalable.h>
#include <umf/providers/provider_level_zero.h>
#include <umf/providers/provider_os_memory.h>

#include <numa.h>
#include <numaif.h>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include "utils_examples.h"
#include <mutex>



// Function to create a memory provider which allocates memory from the specified NUMA node
// by using umfMemspaceCreateFromNumaArray
int createMemoryProviderFromArray(umf_memory_provider_handle_t *hProvider,
                                  unsigned numa) {
    int ret = 0;
    umf_result_t result;
    umf_memspace_handle_t hMemspace = NULL;
    umf_mempolicy_handle_t hPolicy = NULL;

    // Create a memspace - memspace is a list of memory sources.
    // In this example, we create a memspace that contains single numa node;
    result = umfMemspaceCreateFromNumaArray(&numa, 1, &hMemspace);
    if (result != UMF_RESULT_SUCCESS) {
        fprintf(stderr, "umfMemspaceCreateFromNumaArray() failed.\n");
        return -1;
    }

    // Create a mempolicy - mempolicy defines how we want to use memory from memspace.
    // In this example, we want to bind memory to the specified numa node.
    result = umfMempolicyCreate(UMF_MEMPOLICY_BIND, &hPolicy);
    if (result != UMF_RESULT_SUCCESS) {
        ret = -1;
        fprintf(stderr, "umfMempolicyCreate failed().\n");
        goto error_memspace;
    }

    // Create a memory provider using the memory space and memory policy
    result = umfMemoryProviderCreateFromMemspace(hMemspace, hPolicy, hProvider);
    if (result != UMF_RESULT_SUCCESS) {
        ret = -1;
        fprintf(stderr, "umfMemoryProviderCreateFromMemspace failed().\n");
        goto error_mempolicy;
    }

    // After creating the memory provider, we can destroy the memspace and mempolicy
error_mempolicy:
    umfMempolicyDestroy(hPolicy);
error_memspace:
    umfMemspaceDestroy(hMemspace);
    return ret;
}

static constexpr unsigned NUM_NODES = 2;
static umf_memory_provider_handle_t NUMA_HANDLES[NUM_NODES]={};
umf_memory_pool_handle_t jemalloc_pool[NUM_NODES]={};
    
static std::mutex umf_lock[NUM_NODES];

__attribute__((constructor))
void umf_alloc_init() {
    umf_memspace_handle_t hMemspace= NULL;
    umf_mempolicy_handle_t hPolicy = NULL;
    for (unsigned i = 0; i < NUM_NODES; ++i) {
        auto r = umfMemspaceCreateFromNumaArray(&i, 1, &hMemspace);
        if (r != UMF_RESULT_SUCCESS) {
            throw std::runtime_error("Could not create space");
        }
        auto policy = umfMempolicyCreate(UMF_MEMPOLICY_BIND, &hPolicy);
        if (policy != UMF_RESULT_SUCCESS) {
            throw std::runtime_error("Could not create policy");
        }
        auto h = umfMemoryProviderCreateFromMemspace(hMemspace, hPolicy, &NUMA_HANDLES[i]);
        if (h != UMF_RESULT_SUCCESS) {
            throw std::runtime_error("Could not create policy");
        }
        auto pool = umfPoolCreate(umfJemallocPoolOps(), NUMA_HANDLES[i], NULL,  UMF_POOL_CREATE_FLAG_DISABLE_TRACKING, &jemalloc_pool[i]);
        if(pool != UMF_RESULT_SUCCESS){
            throw std::runtime_error("Could not create pool");
        }

        umfMempolicyDestroy(hPolicy);
        umfMemspaceDestroy(hMemspace);
    }


}


void* umf_alloc(unsigned NodeId, size_t size, size_t allign){
    // umf_lock[NodeId].lock();
    void *ptr = NULL;
    ptr = umfPoolAlignedMalloc(jemalloc_pool[NodeId], size, allign);
    if(ptr == NULL){
        throw std::bad_alloc();
    }
    //umf_result_t ret = umfMemoryProviderAlloc(NUMA_HANDLES[NodeId], size, allign, &ptr);
    // umf_lock[NodeId].unlock();
    // if (ret==UMF_RESULT_SUCCESS){
    //     throw std::runtime_error("Could not allocate pool");
    // }
    return ptr;
}

void umf_free(unsigned NodeId){
    if(umfPoolFree(jemalloc_pool[NodeId], NULL) != UMF_RESULT_SUCCESS){
        throw std::runtime_error("Could not free pool");
    }
    //umfMemoryProviderDestroy(NUMA_HANDLES[NodeId]);
}