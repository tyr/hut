/*
 * This file is part of Hut.
 *
 * Copyright 2015 Ironmelt Limited.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 *
 * ---------------------- *
 * ORIGINAL LICENSE BELOW *
 * ---------------------- *
 *
 * Copyright (c) 2011 Ed Schouten <ed@FreeBSD.org>
 *                    David Chisnall <theraven@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * COMPATIBILITY MACROS
 */

#if !defined(__has_feature)
#define __has_feature(x) 0
#endif

#if !defined(__has_builtin)
#define __has_builtin(x) 0
#endif

#if !defined(__GNUC_PREREQ__)
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ__(maj, min) \
      ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
#define __GNUC_PREREQ__(maj, min) 0
#endif
#endif

#if !defined(__CLANG_ATOMICS) && !defined(__GNUC_ATOMICS)
#if __has_feature(c_atomic)
#define __CLANG_ATOMICS
#elif __GNUC_PREREQ__(4, 7)
#define __GNUC_ATOMICS
#elif !defined(__GNUC__)
#error "stdatomic.h does not support your compiler"
#endif
#endif

#if !defined(__CLANG_ATOMICS)
#define _Atomic(T) struct { volatile __typeof__(T) __val; }
#endif

/******************************************************************************
 * INITIALIZATION
 */

#ifdef ATOMIC_VAR_INIT
#undef ATOMIC_VAR_INIT
#endif

#if defined(__CLANG_ATOMICS)
#define ATOMIC_VAR_INIT(__value)    (__value)
  #define atomic_init(__obj, __value) __c11_atomic_init(__obj, __value)
#else
#define ATOMIC_VAR_INIT(__value)    { .__val = (__value) }
#define atomic_init(__obj, __value) \
    do { \
      (__obj)->__val = (__value); \
    } while (0)
#endif

/******************************************************************************
 * ORDER / CONSISTENCY
 */

/*
 * Clang and recent GCC both provide predefined macros for the memory
 * orderings.  If we are using a compiler that doesn't define them, use the
 * clang values - these will be ignored in the fallback path.
 */
#ifndef __ATOMIC_RELAXED
#define __ATOMIC_RELAXED 0
#endif
#ifndef __ATOMIC_CONSUME
#define __ATOMIC_CONSUME 1
#endif
#ifndef __ATOMIC_ACQUIRE
#define __ATOMIC_ACQUIRE 2
#endif
#ifndef __ATOMIC_RELEASE
#define __ATOMIC_RELEASE 3
#endif
#ifndef __ATOMIC_ACQ_REL
#define __ATOMIC_ACQ_REL 4
#endif
#ifndef __ATOMIC_SEQ_CST
#define __ATOMIC_SEQ_CST 5
#endif

/*
 * The memory_order_* constants that denote the barrier behaviour of the
 * atomic operations.
 */
typedef enum memory_order {
  memory_order_relaxed = __ATOMIC_RELAXED,
  memory_order_consume = __ATOMIC_CONSUME,
  memory_order_acquire = __ATOMIC_ACQUIRE,
  memory_order_release = __ATOMIC_RELEASE,
  memory_order_acq_rel = __ATOMIC_ACQ_REL,
  memory_order_seq_cst = __ATOMIC_SEQ_CST
} memory_order;

/******************************************************************************
 * FENCES
 */

#ifdef __CLANG_ATOMICS
#define atomic_thread_fence(__order) __c11_atomic_thread_fence(__order)
  #define atomic_signal_fence(__order) __c11_atomic_signal_fence(__order)
#elif defined(__GNUC_ATOMICS)
#define atomic_thread_fence(__order) __atomic_thread_fence(__order)
  #define atomic_signal_fence(__order) __atomic_signal_fence(__order)
#else
#define atomic_thread_fence(__order) __sync_synchronize()
#define atomic_signal_fence(__order) __asm volatile ("" : : : "memory")
#endif

/******************************************************************************
 * LOCK-FREE PROPERTIES
 */

#if defined(__CLANG_ATOMICS)
#define atomic_is_lock_free(__obj) __c11_atomic_is_lock_free(sizeof(__obj))
#elif defined(__GNUC_ATOMICS)
#define atomic_is_lock_free(__obj) __atomic_is_lock_free(sizeof((__obj)->__val))
#else
#define atomic_is_lock_free(__obj) (sizeof((__obj)->__val) <= sizeof(void *))
#endif

/******************************************************************************
 * ATOMIC INTEGER TYPES
 */

typedef _Atomic(bool)               atomic_bool;
typedef _Atomic(char)               atomic_char;
typedef _Atomic(signed char)        atomic_schar;
typedef _Atomic(unsigned char)      atomic_uchar;
typedef _Atomic(short)              atomic_short;
typedef _Atomic(unsigned short)     atomic_ushort;
typedef _Atomic(int)                atomic_int;
typedef _Atomic(unsigned int)       atomic_uint;
typedef _Atomic(long)               atomic_long;
typedef _Atomic(unsigned long)      atomic_ulong;
typedef _Atomic(long long)          atomic_llong;
typedef _Atomic(unsigned long long) atomic_ullong;
typedef _Atomic(wchar_t)            atomic_wchar_t;
typedef _Atomic(int_least8_t)       atomic_int_least8_t;
typedef _Atomic(uint_least8_t)      atomic_uint_least8_t;
typedef _Atomic(int_least16_t)      atomic_int_least16_t;
typedef _Atomic(uint_least16_t)     atomic_uint_least16_t;
typedef _Atomic(int_least32_t)      atomic_int_least32_t;
typedef _Atomic(uint_least32_t)     atomic_uint_least32_t;
typedef _Atomic(int_least64_t)      atomic_int_least64_t;
typedef _Atomic(uint_least64_t)     atomic_uint_least64_t;
typedef _Atomic(int_fast8_t)        atomic_int_fast8_t;
typedef _Atomic(uint_fast8_t)       atomic_uint_fast8_t;
typedef _Atomic(int_fast16_t)       atomic_int_fast16_t;
typedef _Atomic(uint_fast16_t)      atomic_uint_fast16_t;
typedef _Atomic(int_fast32_t)       atomic_int_fast32_t;
typedef _Atomic(uint_fast32_t)      atomic_uint_fast32_t;
typedef _Atomic(int_fast64_t)       atomic_int_fast64_t;
typedef _Atomic(uint_fast64_t)      atomic_uint_fast64_t;
typedef _Atomic(int8_t)             atomic_int8_t;
typedef _Atomic(uint8_t)            atomic_uint8_t;
typedef _Atomic(int16_t)            atomic_int16_t;
typedef _Atomic(uint16_t)           atomic_uint16_t;
typedef _Atomic(int32_t)            atomic_int32_t;
typedef _Atomic(uint32_t)           atomic_uint32_t;
typedef _Atomic(int64_t)            atomic_int64_t;
typedef _Atomic(uint64_t)           atomic_uint64_t;
typedef _Atomic(intptr_t)           atomic_intptr_t;
typedef _Atomic(uintptr_t)          atomic_uintptr_t;
typedef _Atomic(size_t)             atomic_size_t;
typedef _Atomic(ptrdiff_t)          atomic_ptrdiff_t;
typedef _Atomic(intmax_t)           atomic_intmax_t;
typedef _Atomic(uintmax_t)          atomic_uintmax_t;

/******************************************************************************
 * BASE ATOMIC OPERATIONS
 */

/*
 * Compiler-specific operations.
 */
#if defined(__CLANG_ATOMICS)

#define atomic_compare_exchange_strong_explicit(__object, __expected, __desired, __success, __failure) \
    __c11_atomic_compare_exchange_strong(__object, __expected, __desired, __success, __failure)

  #define atomic_compare_exchange_weak_explicit(__object, __expected, __desired, __success, __failure) \
    __c11_atomic_compare_exchange_weak(__object, __expected, __desired, __success, __failure)

  #define atomic_exchange_explicit(__object, __desired, __order) \
    __c11_atomic_exchange(__object, __desired, __order)

  #define atomic_fetch_add_explicit(__object, __operand, __order) \
    __c11_atomic_fetch_add(__object, __operand, __order)

  #define atomic_fetch_and_explicit(__object, __operand, __order) \
    __c11_atomic_fetch_and(__object, __operand, __order)

  #define atomic_fetch_or_explicit(__object, __operand, __order) \
    __c11_atomic_fetch_or(__object, __operand, __order)

  #define atomic_fetch_sub_explicit(__object, __operand, __order) \
    __c11_atomic_fetch_sub(__object, __operand, __order)

  #define atomic_fetch_xor_explicit(__object, __operand, __order) \
    __c11_atomic_fetch_xor(__object, __operand, __order)

  #define atomic_load_explicit(__object, __order) \
    __c11_atomic_load(__object, __order)

  #define atomic_store_explicit(__object, __desired, __order) \
    __c11_atomic_store(__object, __desired, __order)

#elif defined(__GNUC_ATOMICS)

#define atomic_compare_exchange_strong_explicit(__object, __expected, __desired, __success, __failure) \
    __atomic_compare_exchange_n(&(__object)->__val, __expected, __desired, 0, __success, __failure)

  #define atomic_compare_exchange_weak_explicit(__object, __expected, __desired, __success, __failure) \
    __atomic_compare_exchange_n(&(__object)->__val, __expected, __desired, 1, __success, __failure)

  #define atomic_exchange_explicit(__object, __desired, __order) \
    __atomic_exchange_n(&(__object)->__val, __desired, __order)

  #define atomic_fetch_add_explicit(__object, __operand, __order) \
    __atomic_fetch_add(&(__object)->__val, __operand, __order)

  #define atomic_fetch_and_explicit(__object, __operand, __order) \
    __atomic_fetch_and(&(__object)->__val, __operand, __order)

  #define atomic_fetch_or_explicit(__object, __operand, __order) \
    __atomic_fetch_or(&(__object)->__val, __operand, __order)

  #define atomic_fetch_sub_explicit(__object, __operand, __order) \
    __atomic_fetch_sub(&(__object)->__val, __operand, __order)

  #define atomic_fetch_xor_explicit(object, operand, order) \
    __atomic_fetch_xor(&(object)->__val, operand, order)

  #define atomic_load_explicit(__object, __order) \
    __atomic_load_n(&(__object)->__val, __order)

  #define atomic_store_explicit(__object, __desired, __order) \
    __atomic_store_n(&(__object)->__val, __desired, __order)

#else

#define atomic_compare_exchange_strong_explicit(__object, __expected, __desired, __success, __failure) \
    ({ \
      __typeof__((__object)->__val) __v; \
      _Bool __r; \
      __v = __sync_val_compare_and_swap(&(__object)->__val, *(__expected), __desired); \
      __r = *(__expected) == __v; \
      *(__expected) = __v; \
      __r; \
    })

#define atomic_compare_exchange_weak_explicit(__object, __expected, __desired, __success, __failure) \
    atomic_compare_exchange_strong_explicit(__object, __expected, __desired, __success, __failure)

#if __has_builtin(__sync_swap)
#define atomic_exchange_explicit(__object, __desired, __order) \
      __sync_swap(&(__object)->__val, __desired)
#else
#define atomic_exchange_explicit(__object, __desired, __order) \
      ({ \
        __typeof__((__object)->__val) __v; \
        __v = __sync_lock_test_and_set(&(__object)->__val, __desired); \
        __sync_synchronize(); \
        __v; \
      })
#endif

#define atomic_fetch_add_explicit(__object, __operand, __order) \
    __sync_fetch_and_add(&(__object)->__val, __operand)

#define atomic_fetch_and_explicit(__object, __operand, __order) \
    __sync_fetch_and_and(&(__object)->__val, __operand)

#define atomic_fetch_or_explicit(__object, __operand, __order) \
    __sync_fetch_and_or(&(__object)->__val, __operand)

#define atomic_fetch_sub_explicit(__object, __operand, __order) \
    __sync_fetch_and_sub(&(__object)->__val, __operand)

#define atomic_fetch_xor_explicit(__object, __operand, __order) \
    __sync_fetch_and_xor(&(__object)->__val, __operand)

#define atomic_load_explicit(__object, __order) \
    __sync_fetch_and_add(&(__object)->__val, 0)

#define atomic_store_explicit(__object, __desired, __order) \
    do {  \
      __sync_synchronize(); \
      (__object)->__val = (__desired); \
      __sync_synchronize(); \
    } while (0)

#endif

/******************************************************************************
 * CONVINIENCE FUNCTIONS
 */

#define atomic_compare_exchange_strong(__object, __expected, __desired) \
  atomic_compare_exchange_strong_explicit(__object, __expected, __desired, memory_order_seq_cst, \
      memory_order_seq_cst)

#define atomic_compare_exchange_weak(__object, __expected, __desired) \
  atomic_compare_exchange_weak_explicit(__object, __expected, __desired, memory_order_seq_cst, \
      memory_order_seq_cst)

#define atomic_exchange(__object, __desired) \
  atomic_exchange_explicit(__object, __desired, memory_order_seq_cst)

#define atomic_fetch_add(__object, __operand) \
  atomic_fetch_add_explicit(__object, __operand, memory_order_seq_cst)

#define atomic_fetch_and(__object, __operand) \
  atomic_fetch_and_explicit(__object, __operand, memory_order_seq_cst)

#define atomic_fetch_or(__object, __operand) \
  atomic_fetch_or_explicit(__object, __operand, memory_order_seq_cst)

#define atomic_fetch_sub(__object, __operand) \
  atomic_fetch_sub_explicit(__object, __operand, memory_order_seq_cst)

#define atomic_fetch_xor(__object, __operand) \
  atomic_fetch_xor_explicit(__object, __operand, memory_order_seq_cst)

#define atomic_load(__object) \
  atomic_load_explicit(__object, memory_order_seq_cst)

#define atomic_store(__object, __desired) \
  atomic_store_explicit(__object, __desired, memory_order_seq_cst)

/******************************************************************************
 * ATOMIC FLAGS
 */

typedef atomic_bool atomic_flag;

#ifdef ATOMIC_FLAG_INIT
#undef ATOMIC_FLAG_INIT
#endif

#define ATOMIC_FLAG_INIT ATOMIC_VAR_INIT(0)

#define atomic_flag_clear_explicit(__object, __order) \
  atomic_store_explicit(__object, 0, __order)

#define atomic_flag_test_and_set_explicit(__object, __order) \
  ({ \
    bool __expected = false; \
    !atomic_compare_exchange_strong_explicit(__object, &__expected, 1, __order, __order); \
  })

#define atomic_flag_clear(__object) \
  atomic_flag_clear_explicit(__object, memory_order_seq_cst)

#define atomic_flag_test_and_set(__object) \
  atomic_flag_test_and_set_explicit(__object, memory_order_seq_cst)

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
