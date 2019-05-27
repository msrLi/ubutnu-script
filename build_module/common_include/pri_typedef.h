/**
 * @file ite_typedef.h
 * @brief 基本公用数据类型定义
 *
 * @author  <itarge@itarge.com>
 * @version 1.0.0
 * @date 2017-03-01
 */

/* Copyright (C) 2009-2016, Itarge Inc.
 * All right reserved
 *
 */

#ifndef __PRI_TYPEDEF_H__
#define __PRI_TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define OSAL_OK                 (0)
#define OSAL_FAIL               (-1)
#define OSAL_UNSUPPORT          (-2)
#define OSAL_ERROR_DATA         (-3)


#define OSAL_FALSE              (0)
#define OSAL_TRUE               (!PRI_OSAL_FALSE)

#define OSAL_WAIT_NONE          (0)
#define OSAL_WAIT_FOREVER       (~0)

/* 数据类型定义 */
typedef char                        OS_S8;
typedef unsigned char               OS_U8;

typedef short                       OS_S16;
typedef unsigned short              OS_U16;

typedef int                         OS_S32;
typedef long                        OS_SLONG;
typedef unsigned int                OS_U32;
typedef unsigned long               OS_ULONG;
typedef float                       OS_F32;

typedef long long                   OS_S64;
typedef unsigned long long          OS_U64;
typedef double                      OS_D64;

typedef int                         OS_BOOL;
typedef void*                       OS_HANDLE;


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__PRI_TYPEDEF_H__
