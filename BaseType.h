/*****************************************************************************
模块名      : Common
文件名      : BaseType.h
相关文件    : 各模块的开发文件
文件实现功能: 提供软件开发的基本类型定义
作者        : sq
版本        : V1.0  Copyright(C) 2016-2021 Boguan Investment, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2016/07/05  1.0         sq           创建

注：为避免32位系统，64位系统的不一致， 32位整型用long来定义，不用int。
	推荐用u32风格的定义
******************************************************************************/

#ifndef _BASETYPE_H_
#define _BASETYPE_H_

#ifdef __cplusplus
extern "C" {
#endif 

//-----------------------------------------------------------------------
//
//  General types
//
//-----------------------------------------------------------------------

//整型定义

typedef  unsigned long		u32;
typedef  unsigned short		u16;
typedef  unsigned char		u8;

typedef signed long			s32;
typedef signed short		s16;


#ifdef __cplusplus

typedef char				s8;

#else

#ifdef _LINUX_

typedef char				s8;

#else

typedef signed char			s8;

#endif //_LINUX_

#endif //__cplusplus

#ifdef _MSC_VER

typedef signed __int64		s64;
typedef unsigned _int64		u64;

#else 

typedef signed	long long	s64;
typedef unsigned long long	u64;

#endif //_MSC_VER


//兼容其它风格的整型定义

#ifndef _MSC_VER     //msc 已定义的UINT32类型

typedef  u64				UINT64;
typedef  u32				UINT32;
typedef  u16				UINT16;
typedef  u8					UINT8;

typedef  s64				INT64;
typedef  s32				INT32;
typedef  s16				INT16;
typedef  s8					INT8;

#endif //_MSC_VER




//兼容其它风格的整型定义
#ifdef _COMPATIBLE_TYPE      //由于某些模块调用的系统库如db已有 uint32_t 的定义，所以此处用宏来避免冲突

typedef  u64				uint64_t;
typedef  u32				uint32_t;
typedef  u16				uint16_t;
typedef  u8					uint8_t;

typedef  s64				int64_t;
typedef  s32				int32_t;
typedef  s16				int16_t;
typedef  s8					int8_t;

#endif //_COMPATIBLE_TYPE


//浮点型定义
#ifndef f32
typedef	float				f32;
#endif

#ifndef f64
typedef double				f64;
#endif


//BOOL 定义
typedef  int				BOOL;


// 字符指针
#ifndef _MSC_VER

#ifndef LPSTR
#define LPSTR   char *
#endif

#ifndef LPCSTR
#define LPCSTR  const char *
#endif

#endif //_MSC_VER


//句柄
#ifndef _MSC_VER

typedef void *  HANDLE;

#endif //_MSC_VER


typedef void *  BGHANDLE;	//bgfund句柄，避免和操作系统的同名


//BOOL常量
#ifndef TRUE
#define TRUE				(BOOL)1
#endif

#ifndef FALSE
#define FALSE				(BOOL)0
#endif

//-----------------------------------------------------------------------
//
// Macros
//
//-----------------------------------------------------------------------

//参数标识定义
#ifndef IN
#define IN
#endif /* for Input */

#ifndef OUT
#define OUT
#endif /* for Output */


// align must be power of 2
#ifndef DROUND_UP
#define DROUND_UP(_size, _align)    (((_size) + ((_align) - 1)) & ~((_align) - 1))
#endif

#ifndef DROUND_DOWN
#define DROUND_DOWN(_size, _align)  ((_size) & ~((_align) - 1))
#endif

#ifndef DCAL_MIN
#define DCAL_MIN(_a, _b)    ((_a) < (_b) ? (_a) : (_b))
#endif

#ifndef DCAL_MAX
#define DCAL_MAX(_a, _b)    ((_a) > (_b) ? (_a) : (_b))
#endif

#ifndef DARRAY_SIZE
#define DARRAY_SIZE(_array) (sizeof(_array) / sizeof(_array[0]))
#endif

#ifndef DPTR_OFFSET
#define DPTR_OFFSET(_type, _member) ((int)&((_type*)0)->member)
#endif

#ifndef DPTR_ADD
#define DPTR_ADD(_ptr, _size)   (void*)((char*)(_ptr) + (_size))
#endif

#ifdef _LINUX_

#define DLIKELY(x)   __builtin_expect(!!(x),1)
#define DUNLIKELY(x)   __builtin_expect(!!(x),0)

#else//_LINUX_

#define DLIKELY(x)	(x)
#define DUNLIKELY(x)	(x)

#endif//_LINUX_


//while 循环宏定义
#ifdef _LINUX_
#define __FUNCDNAME__ "noname"
#endif

#define MAXWHILE_DFTCYCLE (u32)(0xfffffffe)

//缺省循环次数
#define WHILE_DFTLIMIT_BEGIN(cond) \
{ \
    u32 dwFocusVMWhileCycle = 0; \
    while(cond) \
    { \
        dwFocusVMWhileCycle++; \
        if(dwFocusVMWhileCycle > MAXWHILE_DFTCYCLE) \
        { \
            printf("file: %s, line: %d, func: %s, while cycle(%lu) > MAXWHILE_DFTCYCLE(%lu)\n", \
                    __FILE__, __LINE__, __FUNCDNAME__, dwFocusVMWhileCycle, MAXWHILE_DFTCYCLE); \
                    break; \
        }

#define WHILE_DFTLIMIT_END \
    }\
}

//指定循环次数(建议使用)
#define WHILE_LIMIT_BEGIN(cond, dwMaxWhileCycle) \
{ \
    u32 dwFocusVMWhileCycle = 0; \
    while(cond) \
    { \
        dwFocusVMWhileCycle++; \
        if(dwFocusVMWhileCycle > dwMaxWhileCycle) \
        { \
            printf("file: %s, line: %d, func: %s, while cycle(%lu) > maxcycle(%lu)\n", \
                __FILE__, __LINE__, __FUNCDNAME__, dwFocusVMWhileCycle, dwMaxWhileCycle); \
            break; \
        }   

#define WHILE_LIMIT_END \
    }\
}

//主要用于任务的循环
#define WHILE_FOREVER(cond) while(cond)

//带超时的条件等待
#define EST_COND_WAIT(cdtn, timewaitms) \
{ \
    int dwWaitSum = 0; \
    while(!(cdtn)) \
    { \
        OalTaskDelay(20); \
        dwWaitSum += 20; \
        if(dwWaitSum > timewaitms) \
        { \
            break; \
        } \
    } \
}

//带超时的条件等待，bTimeout的值反映是否超时，必须为BOOL变量
#define EST_COND_WAIT_RETURN(cdtn, timewaitms, bTimeout) \
{ \
    int dwWaitSum = 0; \
    bTimeout = FALSE; \
    while(!(cdtn)) \
    { \
    OalTaskDelay(20); \
    dwWaitSum += 20; \
    if(dwWaitSum > timewaitms) \
        { \
        bTimeout = TRUE; \
        break; \
        } \
    } \
}

//文件名、目录名最大长度
#define MAX_FILENAME_LEN        (u8)64


//-----------------------------------------------------------------------
//
//  Error code
//
//-----------------------------------------------------------------------

#define DERRBASE_SET(base)       ((u32)base << 16)  //错误码的基数放在高16位，低16位为各模块内部具体的错误码
#define DERRBASE_GET(err)        (err & 0xffff0000) //得到具体错误码的基数
#define DERRLOWBITS_GET(err)     (err & 0x0000ffff) //得到具体错误码的低16位

/*================================= 错误码分配 ================================*/
#define DOAL_ERRBASE                 DERRBASE_SET(200)    //OAL
#define DOTL_ERRBASE                 DERRBASE_SET(210)    //OTL

typedef enum
{
    EECode_OK = 0,

    EECode_NotInitilized = 0x001,
    EECode_NotRunning = 0x002,

    EECode_Error = 0x003,
    EECode_Closed = 0x004,
    EECode_Busy = 0x005,
    EECode_OSError = 0x006,
    EECode_IOError = 0x007,
    EECode_TimeOut = 0x008,
    EECode_TooMany = 0x009,
    EECode_OutOfCapability = 0x00a,
    EECode_TimeTick = 0x00b,

    EECode_NoMemory = 0x00c,
    EECode_NoPermission = 0x00d,
    EECode_NoImplement = 0x00e,
    EECode_NoInterface = 0x00f,

    EECode_NotExist = 0x010,
    EECode_NotSupported = 0x011,

    EECode_BadState = 0x012,
    EECode_BadParam = 0x013,
    EECode_BadCommand = 0x014,
    EECode_BadFormat = 0x015,
    EECode_BadMsg = 0x016,
    EECode_BadSessionNumber = 0x017,

    EECode_TryAgain = 0x018,

    EECode_DataCorruption = 0x019,
    EECode_DataMissing = 0x01a,

    EECode_InternalMemoryBug = 0x01b,
    EECode_InternalLogicalBug = 0x01c,
    EECode_InternalParamsBug = 0x01d,

    EECode_ProtocolCorruption = 0x01e,
    EECode_AbortTimeOutAPI = 0x01f,
    EECode_AbortSessionQuitAPI = 0x020,

    EECode_UnknownError = 0x021,
    EECode_MandantoryDelete = 0x022,

    EECode_NetConnectFail = 0x023,

    EECode_NetSocketSend_Error = 0x024,
    EECode_NetSocketRecv_Error = 0x025,

    EECode_NotLogin = 0x026,
    EECode_AlreadyExist = 0x027,
} EECode;

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // _BASETYPE_H_
