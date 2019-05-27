/**
 * @file aes_extern_data.h
 * @brief aes data extension header file
 * @author lihb13  <NULL.com>
 * @version 1.0.0
 * @date 2019-05-24
 */

/* Copyright(C) 2009-2017, Itarge Inc.
 * All right reserved
 * 
 */

#ifndef __AES_EXTERN_DATA_H_
#define __AES_EXTERN_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


typedef struct {
    unsigned int date_length;
    char sha1[128];
}aes_data_head_t;


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__AES_EXTERN_DATA_H_
