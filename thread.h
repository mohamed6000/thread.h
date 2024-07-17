#pragma once
#include "common.h"

// do this:
//     #define THREAD_IMPLEMENTATION
//     before you include this file in *one* C or C++ file to create the implementation.

struct Thread {
    u64 thread_id;
    void *internal_data;
};

typedef u32 (*ThreadProc)(void *);


bool thread_create(Thread *out_thread, ThreadProc thread_proc, void *params, bool should_detach = false);

void thread_destroy(Thread *thread);

bool thread_is_active(Thread *thread);

bool thread_wait(Thread *thread); // also known as thread_join

bool thread_wait_timeout(Thread *thread, u64 wait_ms);

void thread_sleep(Thread *thread, u64 ms);

void thread_detach(Thread *thread);

void thread_cancel(Thread *thread);

u64 thread_get_current_id(void);


// mutex
struct Mutex {
    void *internal_data;
};


bool mutex_create(Mutex *out_mutex);

void mutex_destroy(Mutex *mutex);

bool mutex_lock(Mutex *mutex);

bool mutex_unlock(Mutex *mutex);


// semaphore
struct Semaphore {
    void *internal_data;
};


bool semaphore_create(Semaphore *out_semaphore, u32 max_count, u32 start_count);

void semaphore_destroy(Semaphore *semaphore);

bool semaphore_signal(Semaphore *semaphore);

bool semaphore_wait(Semaphore *semaphore, u64 wait_ms);


// thread lib implementation
#ifdef THREAD_IMPLEMENTATION
#if OS_WINDOWS
// windows layer
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

bool thread_create(Thread *out_thread, ThreadProc thread_proc, void *params, bool should_detach) {
    assert(out_thread != null);
    if (!thread_proc) return false;

    out_thread->internal_data = CreateThread(0, 0, 
                                             (LPTHREAD_START_ROUTINE)thread_proc, 
                                             params, 0,
                                             (DWORD *)&out_thread->thread_id);
    if (!out_thread->internal_data) return false;

    if (should_detach) {
        CloseHandle((HANDLE)out_thread->internal_data);
    }

    return true;
}

void thread_destroy(Thread *thread) {
    if (thread && thread->internal_data) {
        DWORD exit_code;
        GetExitCodeThread(thread->internal_data, &exit_code);
        CloseHandle((HANDLE)thread->internal_data);
        thread->internal_data = null;
        thread->thread_id = 0;
    }
}

bool thread_is_active(Thread *thread) {
    if (thread && thread->internal_data) {
        DWORD exit_code = WaitForSingleObject(thread->internal_data, 0);
        if (exit_code == WAIT_TIMEOUT) return true;
    }

    return false;
}

bool thread_wait(Thread *thread) {
    if (thread && thread->internal_data) {
        DWORD exit_code = WaitForSingleObject(thread->internal_data, INFINITE);
        if (exit_code == WAIT_OBJECT_0) return true;
    }

    return false;
}

bool thread_wait_timeout(Thread *thread, u64 wait_ms) {
    if (thread && thread->internal_data) {
        DWORD exit_code = WaitForSingleObject(thread->internal_data, wait_ms);
        if (exit_code == WAIT_OBJECT_0) {
            return true;
        } else if (exit_code == WAIT_TIMEOUT) {
            return false;
        }
    }

    return false;
}

void thread_sleep(Thread *thread, u64 ms) {
    Sleep(ms);
}

void thread_detach(Thread *thread) {
    if (thread && thread->internal_data) {
        CloseHandle((HANDLE)thread->internal_data);
        thread->internal_data = null;
    }
}

void thread_cancel(Thread *thread) {
    if (thread && thread->internal_data) {
        TerminateThread(thread->internal_data, 0);
        thread->internal_data = null;
    }
}

u64 thread_get_current_id(void) {
    return GetCurrentThreadId();
}


// mutex implementation
bool mutex_create(Mutex *out_mutex) {
    assert(out_mutex != null);
    out_mutex->internal_data = CreateMutex(0, 0, 0);
    if (!out_mutex->internal_data) {
        return false;
    }

    return true;
}

void mutex_destroy(Mutex *mutex) {
    if (mutex && mutex->internal_data) {
        CloseHandle((HANDLE)mutex->internal_data);
        mutex->internal_data = null;
    }
}

bool mutex_lock(Mutex *mutex) {
    assert(mutex != null);
    DWORD result = WaitForSingleObject((HANDLE)mutex->internal_data, INFINITE);
    switch (result) {
        case WAIT_OBJECT_0:  return true;

        case WAIT_ABANDONED: return false;
    }

    return true;
}

bool mutex_unlock(Mutex *mutex) {
    if (mutex && mutex->internal_data) {
        DWORD result = ReleaseMutex((HANDLE)mutex->internal_data);
        return result != 0;
    }

    return false;
}


// semaphore
bool semaphore_create(Semaphore *out_semaphore, u32 max_count, u32 start_count) {
    assert(out_semaphore != null);
    out_semaphore->internal_data = CreateSemaphore(0, start_count, max_count, 0);
    if (!out_semaphore->internal_data) {
        return false;
    }

    return true;
}

void semaphore_destroy(Semaphore *semaphore) {
    if (semaphore && semaphore->internal_data) {
        CloseHandle(semaphore->internal_data);
        semaphore->internal_data = null;
    }
}

bool semaphore_signal(Semaphore *semaphore) {
    if (!semaphore || !semaphore->internal_data) {
        return false;
    }

    LONG previous_count = 0;
    if (!ReleaseSemaphore(semaphore->internal_data, 1, &previous_count)) {
        return false;
    }

    return true;
}

bool semaphore_wait(Semaphore *semaphore, u64 wait_ms) {
    if (!semaphore || !semaphore->internal_data) {
        return false;
    }

    DWORD result = WaitForSingleObject(semaphore->internal_data, wait_ms);
    switch (result) {
        case WAIT_ABANDONED:
            return false;

        case WAIT_OBJECT_0:
            return true;

        case WAIT_TIMEOUT:
            return false;

        case WAIT_FAILED:
            return false;

        default: return false;
    }

    return true;
}
#endif
#endif