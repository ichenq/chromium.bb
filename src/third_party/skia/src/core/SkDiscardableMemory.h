/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkDiscardableMemory_DEFINED
#define SkDiscardableMemory_DEFINED

#include "SkTypes.h"

/**
 *  Interface for discardable memory. Implementation is provided by the
 *  embedder.
 */
class SK_API SkDiscardableMemory {

public:
    /**
     *  Factory method that creates, initializes and locks an SkDiscardableMemory
     *  object. If either of these steps fails, a NULL pointer will be returned.
     */
    static SkDiscardableMemory* Create(size_t bytes);

    /** Must not be called while locked.
     */
    virtual ~SkDiscardableMemory() {}

    /**
     * Locks the memory, prevent it from being discarded. Once locked. you may
     * obtain a pointer to that memory using the data() method.
     *
     * lock() may return false, indicating that the underlying memory was
     * discarded and that the lock failed.
     *
     * Nested calls to lock are not allowed.
     */
    virtual bool lock() = 0;

    /**
     * Returns the current pointer for the discardable memory. This call is ONLY
     * valid when the discardable memory object is locked.
     */
    virtual void* data() = 0;

    /**
     * Unlock the memory so that it can be purged by the system. Must be called
     * after every successful lock call.
     */
    virtual void unlock() = 0;
};

#endif
