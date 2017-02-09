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
 * License along with this library.
 */

#pragma once

/******************************************************************************
 * API ACCESS
 */


#ifndef HUT_API
  #ifdef _WIN32
    #if defined(HUT_BUILD_SHARED) /* build dll */
      #define HUT_API __declspec(dllexport)
    #elif !defined(HUT_BUILD_STATIC) /* use dll */
      #define HUT_API __declspec(dllimport)
    #else /* static library */
      #define HUT_API
    #endif
  #else
    #if __GNUC__ >= 4
      #define HUT_API __attribute__((visibility("default"), unused))
    #else
      #define HUT_API
    #endif
  #endif
#endif

#define HUT_INLINE static inline

/******************************************************************************
 * STATIC ASSERT
 */

#ifdef __GNUC__
  #define HUT_STATIC_ASSERT_HELPER(__expr, __msg) \
      (!!sizeof(struct { unsigned int HUT_STATIC_ASSERTION__##__msg: (__expr) ? 1 : -1; }))
  #define HUT_STATIC_ASSERT_INTERNAL(__expr, __msg) \
      extern int (*assert_function__(void)) [HUT_STATIC_ASSERT_HELPER(__expr, __msg)]
#else
  #define HUT_STATIC_ASSERT_INTERNAL(__expr, __msg) \
      extern char HUT_STATIC_ASSERTION__##__msg[1]; \
      extern char HUT_STATIC_ASSERTION__##__msg[(__expr)?1:2]
#endif

/**
 * @private
 *
 * Asserts that a condition is true at compile-time. If it isn't it throws a compilation error with the given message.
 *
 * @param __expr static condition to check for
 * @param __msg  the identifying message (must follow variable-naming requirements)
 */
#define HUT_STATIC_ASSERT(__expr, __msg) HUT_STATIC_ASSERT_INTERNAL(__expr, __msg)
