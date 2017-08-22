/*****************************************************************************
模块名      : Oal(operation system application layer)
文件名      : Oal.h
相关文件    : Oal模块相关文件
文件实现功能: 操作系统基本调用封装
作者        : sq
版本        : V1.0  Copyright(C) 2016-2021 Boguan Investment, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2016/08/21  1.0         sq          创建
******************************************************************************/
#define _LINUX_

#ifndef _OAL_H_
#define _OAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <winsock2.h>
#endif //_WIN32

#ifdef _LINUX_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sched.h>
#include <semaphore.h>
#include <errno.h>
#include <netinet/tcp.h>
#include <stdarg.h>
#include <unistd.h>

#endif //_LINUX_


#ifdef _WIN32

typedef SOCKET SOCKHANDLE;
typedef HANDLE SEMHANDLE;
typedef HANDLE TASKHANDLE;
#define API __declspec(dllexport)
#define HTASK_NULL	NULL

#endif //_WIN32


#ifdef _LINUX_

typedef int			SOCKHANDLE;
typedef void *		SEMHANDLE;
typedef pthread_t	TASKHANDLE;
typedef void *		HMODULE;

#define API // extern "C"
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1

#define HTASK_NULL	0

#endif //_LINUX_

#include "BaseType.h"

#define CREATETASK_SUSPEND			1 //创建任务时，将任务先挂起

#define OAL_OK			(u32)0

#define OALSEM_SIGNALED		OAL_OK

//错误码
#define OALSEM_TIMEOUT			(u32)(DOAL_ERRBASE + 1)
#define OALSEM_FAILED			(u32)(DOAL_ERRBASE + 2)
#define OALERR_UNINIT			(u32)(DOAL_ERRBASE + 3)

#define BDSLIB_INVALID_PARAM   (u32)(DOAL_ERRBASE + 4) /*参数无效*/
#define BDSLIB_SHOULD_BE_INIT  (u32)(DOAL_ERRBASE + 5) /*没有被初始化或者初始化失败*/
#define BDSLIB_NODE_INUSE    (u32)(DOAL_ERRBASE + 6) /*节点已是链表（栈、队列）中的节点*/
#define BDSLIB_NODE_NOT_INUSE  (u32)(DOAL_ERRBASE + 7) /*节点不是链表中的节点*/
#define BDSLIB_ALREADY_EMPTY   (u32)(DOAL_ERRBASE + 8) /*链表（栈、队列）已空*/

#define OALERR_NULLPOINT		(u32)(DOAL_ERRBASE + 9)	/*指针为空*/
#define OALERR_PARAM			(u32)(DOAL_ERRBASE + 10)	/*参数错*/
#define OALERR_CYCLE			(u32)(DOAL_ERRBASE + 11)	/*循环次数错*/
#define OALERR_SYSCALL			(u32)(DOAL_ERRBASE + 12)	/*系统调用错*/
#define OALERR_MEMERR			(u32)(DOAL_ERRBASE + 13)	/*内存错*/
#define OALERR_MEMLOW			(u32)(DOAL_ERRBASE + 14)	/*内存不足*/
#define OALERR_TASKERR			(u32)(DOAL_ERRBASE + 15)	/*任务出错*/
#define OALERR_STACKERR			(u32)(DOAL_ERRBASE + 16)	/*堆栈出错*/

#define OALERR_MAXERRNO			OALERR_STACKERR


//任务优先级
#define PRI_CRITICAL		(u8)10
#define PRI_HIGHEST			(u8)4
#define PRI_ABOVE_NORMAL	(u8)3
#define PRI_NORMAL			(u8)2
#define PRI_BELOW_NORMAL	(u8)1
#define PRI_LOWEST			(u8)0


//资源标签
//#define MAX_TAG_NO             (u32)512
#define RSRCTAGNAME_MAXLEN         (u32)8 //资源标签最大长度


//根据成员得到宿主结构的宏
/*============================================================================
ptr - 成员指针
type - 宿主结构类型
member - 宿主结构中 ptr 成员的名称

用法参考下面：

typedef struct tagMyInfo
{
  ENode m_tENode;
  u32 m_dwMyData;
}TMyInfo;

TMyInfo *ptMyInfo = NULL;
ENode *ptNode = EListGetFirst(ptList);

ptMyInfo = HOST_ENTRY(ptNode, TMyInfo, m_tENode);

============================================================================*/
#define HOST_ENTRY(ptr, type, member) ((type *)((u8 *)(ptr) - (u32)(&((type *)0)->member)))


/*=============================================== 基本接口 ================================================*/

/*====================================================================
函数名：OalInit
功能：oal初始化
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
API BOOL OalInit();

/*====================================================================
函数名：OalExit
功能：oal退出
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
API BOOL OalExit();


/*=============================================== 任务管理 ================================================*/

/*====================================================================
函数名：OalTaskCreate
功能：创建并激活一个任务
算法实现：（可选项）
引用全局变量：
输入参数说明：pfTaskEntry: 任务入口，
szName: 任务名，以NULL结束的字符串，
uPriority: 任务优先级别，
uStacksize: 任务堆栈大小，
uParam: 任务参数。
wFlag: 创建标志， 若要创建任务时，将任务先挂起，用 CREATETASK_SUSPEND。否则填0。
puTaskID: 返回参数，任务ID.

  返回值说明：成功返回任务的句柄，失败返回NULL.
====================================================================*/
API TASKHANDLE OalTaskCreate(void *pfTaskEntry, char *szName, u8 byPriority, u32 dwStacksize,
							 u32 dwParam,  u16 wFlag,  u32 *pdwTaskID);

/*====================================================================
 函数名：OalTaskExit
 功能：退出调用任务，任务退出之前用户要注意释放本任务申请的内存、信号量等资源。
 封装Windows的ExitThread(0)
 算法实现：（可选项）
 引用全局变量：
 输入参数说明：

返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
API BOOL OalTaskExit(void);

/*====================================================================
函数名：OalTaskHBChkItvlSet
功能：任务心跳检测间隔设置(在OalTaskCreate后调用)
算法实现：（可选项）
引用全局变量：
输入参数说明：	dwTaskId - 任务编号, 由OalTaskCreate返回
				dwItvlMs - 检测心跳的间隔(ms)
				pdwTaskHBHdl -- 返回心跳句柄
返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
API BOOL OalTaskHBChkItvlSet(u32 dwTaskId, u32 dwItvlMs);

/*====================================================================
函数名：OalTaskHBHdlGet
功能：任务心跳
算法实现：（可选项）
引用全局变量：
输入参数说明：pdwTaskHBHdl - 返回心跳句柄
返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
API BOOL OalTaskHBHdlGet(u32 *pdwTaskHBHdl);

/*====================================================================
函数名：OalTaskHeartBeat
功能：任务心跳
算法实现：（可选项）
引用全局变量：
输入参数说明：dwTaskHBHdl - 心跳句柄
返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
API BOOL OalTaskHeartBeat(u32 dwTaskHBHdl);

/*====================================================================
 函数名：OalTaskSuspend
 功能：任务挂起
 算法实现：
 引用全局变量：
 输入参数说明：hTask: 任务句柄

返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
API BOOL OalTaskSuspend(TASKHANDLE hTask);

/*====================================================================
 函数名：OalTaskResume
 功能：任务继续
  算法实现：
 引用全局变量：
 输入参数说明：hTask: 任务句柄

返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
API BOOL OalTaskResume(TASKHANDLE hTask);

/*====================================================================
函数名：OalTaskTerminate
功能：退出调用任务。任务退出之前用户要注意释放本任务申请的内存、信号量等资源。
封装Windows的TerminateThread()
算法实现：（可选项）
引用全局变量：
输入参数说明：hTask: 要杀死任务的句柄

  返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
BOOL OalTaskTerminate(TASKHANDLE hTask);

/*====================================================================
函数名：OalTaskWait
功能：等待任务完成
算法实现：（可选项）
引用全局变量：
输入参数说明：TASKHANDLE hTask 任务句柄
  返回值说明：等待成功返回真,否则假.
====================================================================*/
API BOOL OalTaskWait(TASKHANDLE hTask);

/*====================================================================
函数名：OalTaskHandleClose
功能：释放句柄
算法实现：（可选项）
引用全局变量：
输入参数说明：TASKHANDLE hTask 任务句柄
  返回值说明：成功返回真,否则假.
====================================================================*/
API BOOL OalTaskHandleClose(TASKHANDLE hTask);

/*====================================================================
函数名：OalTaskSetPriority
功能：改变任务的优先级。

  算法实现：（可选项）
  引用全局变量：
  输入参数说明：hTask: 目标任务的句柄,
  uPriority: 要设置的优先级.

返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
API BOOL OalTaskSetPriority(TASKHANDLE hTask, u8 byPriority);

/*====================================================================
函数名：OalTaskGetPriority
功能：获得任务的优先级。

  算法实现：（可选项）
  引用全局变量：
  输入参数说明：hTask: 目标任务的句柄,
  puPri: 返回参数, 成功返回任务的优先级.
  返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
API BOOL OalTaskGetPriority(TASKHANDLE hTask, u8* pbyPri);

/*====================================================================
函数名：OalTaskDelay
功能：任务延时
算法实现：
引用全局变量：
输入参数说明：dwDelayMs: 延时时间（ms）
返回值说明：成功返回TRUE, 失败返回FALSE.
====================================================================*/
API void OalTaskDelay(u32 dwDelayMs);

/*====================================================================
函数名：OalTaskSelfID
功能：获得调用任务的ID
算法实现：（可选项）
引用全局变量：
输入参数说明：

  返回值说明：调用任务的ID.
====================================================================*/
API u32 OalTaskSelfID(void);

/*====================================================================
函数名：OalTaskHandleVerify
功能：判断指定任务是否存在
算法实现：（可选项）
引用全局变量：
输入参数说明：

  返回值说明：
====================================================================*/
API BOOL OalTaskHandleVerify(TASKHANDLE hTask);

/*====================================================================
函数名：OalTaskDump
功能：打印现有的任务信息
算法实现：（可选项）
引用全局变量：
输入参数说明：
返回值说明：
====================================================================*/
API void OalTaskDump();


/*=============================================== 数据结构管理 ================================================*/

typedef struct tagENode
{
  u32 dwUseMagic;//判断节点是否已存在于某个数据结构中，防止节点被多次使用
  struct tagENode *pPre;//前驱指针
  struct tagENode *pSuc;//后继指针
}ENode;

/*双向链表*/
typedef struct tagEList
{

  u32  dwLength;//sizeof(tagEList)
  ENode tFirst;//表头节点（不存数据）
  ENode tLast;//表尾节点（不存数据）
  u32 dwSize;//当前节点个数

}EList;

/*===========================================================
函数名： EListInit
功能： 初始化链表
算法实现： 1、全部置零
    2、将表头，表尾链起来
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针

返回值说明： 无
===========================================================*/
API void EListInit(IN EList *pList);

/*===========================================================
函数名： EListInsert
功能： 在参考节点之后插入节点
算法实现：
引用全局变 量： 未引用
输入参数说明： EList *pList 链表指针
         ENode *pRefNode 参考节点的指针
         ENode *pNode 待插入节点的指针

返回值说明： 成功则返回真，否则假
===========================================================*/
API BOOL EListInsert(IN EList *pList,IN OUT ENode *pRefNode,
             IN OUT ENode *pNode);

/*===========================================================
函数名： EListInsertFirst
功能： 在链表起始处插入节点,使之成为第一个节点
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针
         ENode *pNode 待插入节点的指针

返回值说明： 成功则返回真，否则假
===========================================================*/
API BOOL EListInsertFirst(IN EList *pList,IN OUT ENode *pNode);

/*===========================================================
函数名： EListRemoveFirst
功能： 删除链表的第一个数据节点
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针

返回值说明： 成功则返回真，否则假
===========================================================*/
API BOOL EListRemoveFirst(IN EList *pList);

/*===========================================================
函数名： EListGetFirst
功能： 得到链表的第一个数据节点
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针

返回值说明： 成功则返回第一个数据节点的指针，否则NULL
===========================================================*/
API ENode* EListGetFirst(IN EList *pList);

/*===========================================================
函数名： EListInsertLast
功能： 在链表结束前插入节点,使之成为最后一个数据节点
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针
         ENode *pNode 待插入节点的指针

返回值说明： 成功则返回真，否则假
===========================================================*/
API BOOL EListInsertLast(IN EList *pList,IN OUT ENode *pNode);

/*===========================================================
函数名： EListRemoveLast
功能： 删除链表的最后一个数据节点
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针

返回值说明： 成功则返回真，否则假
===========================================================*/
API BOOL EListRemoveLast(IN EList *pList);

/*===========================================================
函数名： EListGetLast
功能： 得到链表的最后一个数据节点
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针

返回值说明： 成功则返回最后一个数据节点的指针，否则NULL
===========================================================*/
API ENode* EListGetLast(IN EList *pList);

/*===========================================================
函数名： EListRemove
功能： 使链表某个节点脱链
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针
         ENode *pNode 待脱链节点的指针

返回值说明： 成功则返回真，否则假
===========================================================*/
API BOOL EListRemove(IN EList *pList,IN OUT ENode *pNode);

/*===========================================================
函数名： EListClear
功能： 删除链表起始处至结束处的节点（全部断链，并将链表恢复到初始化后的状态）
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针

返回值说明： 无
===========================================================*/
API void EListClear(IN EList *pList);

/*===========================================================
函数名： EListNext
功能： 得到下一个节点的指针
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针
         ENode *pRefNode 参考节点
返回值说明： 返回下一节点的指针，若失败返回NULL
===========================================================*/
API ENode* EListNext(IN EList *pList,IN ENode *pRefNode);

/*===========================================================
函数名： EListPre
功能： 得到前一个节点的指针
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针
         ENode *pRefNode 参考节点
返回值说明： 返回前一节点的指针，若失败返回NULL
===========================================================*/
API ENode* EListPre(IN EList *pList,IN ENode *pRefNode);

/*===========================================================
函数名： EListIsEmpty
功能： 判断链表是否为空
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针

返回值说明： 链表为空返回真,否则返回假
===========================================================*/
API BOOL EListIsEmpty(IN EList *pList);

/*===========================================================
函数名： EListSize
功能： 得链表节点个数
算法实现：
引用全局变量： 未引用
输入参数说明： EList *pList 链表指针

返回值说明： 返回节点个数
===========================================================*/
API u32 EListSize(IN EList *pList);


/*=============================================== 临界区管理 ================================================*/

/*====================== 轻量级锁，用于互斥 ======================*/
//原有MyMutex类的跨平台实现

typedef void* TLightLock;

/*===========================================================
函数名： LightLockCreate
功能：创建轻量级锁
算法实现：
引用全局变量： 未引用
输入参数说明： TLightLock *ptLightLock － 锁指针
返回值说明：
===========================================================*/
BOOL LightLockCreate(TLightLock *ptLightLock);

/*===========================================================
函数名： LightLockDelete
功能：删除轻量级锁
算法实现：
引用全局变量： 未引用
输入参数说明： TLightLock *ptLightLock － 锁指针
返回值说明：
===========================================================*/
BOOL LightLockDelete(TLightLock *ptLightLock);


/*===========================================================
函数名： InnerLightLockLock
功能：锁轻量级锁
算法实现：
引用全局变量： 未引用
输入参数说明：	TLightLock *ptLightLock － 锁指针
				s8 strLockName[]:锁名
				s8 strFileName[]:调用所在的文件名
				u32 dwLine:调用所在的行
返回值说明：
===========================================================*/
BOOL InnerLightLockLock(TLightLock *ptLightLock, s8 strLockName[], s8 strFileName[], u32 dwLine);

#define LightLockLock(ptLightLock) InnerLightLockLock(ptLightLock, #ptLightLock, __FILE__, __LINE__)

/*===========================================================
函数名： LightLockUnLock
功能：解锁轻量级锁
算法实现：
引用全局变量： 未引用
输入参数说明： TLightLock *ptLightLock － 锁指针
返回值说明：
===========================================================*/
BOOL LightLockUnLock(TLightLock *ptLightLock);


/*====================== 慢速的信号量，用于同步 ======================*/
//原有MyEvent类的跨平台实现

/*====================================================================
函数名：OalSlowBadSemBCreate
功能：创建一个二元信号量
算法实现：
引用全局变量：
输入参数说明：phSem: 返回的信号量句柄
返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
API BOOL OalSlowBadSemBCreate(SEMHANDLE *phSem);

/*====================================================================
函数名：OalSlowBadSemCCreate
功能：创建计数信号量
算法实现：
引用全局变量：
输入参数说明：phSem: 信号量句柄返回参数，
dwInitCount: 初始计数，
dwMaxCount: 最大计数
返回值说明：成功返回TRUE，失败返回FALSE.
====================================================================*/
API BOOL OalSlowBadSemCCreate(SEMHANDLE *phSem, u32 dwInitCount, u32 dwMaxCount);


/*====================================================================
函数名：OalSlowBadSemDelete
功能：删除信号量
算法实现：（可选项）
引用全局变量：
输入参数说明：phSem: 待删除信号量的句柄
返回值说明：成功返回TRUE，失败返回FALSE.
====================================================================*/
API BOOL OalSlowBadSemDelete(SEMHANDLE phSem);


/*====================================================================
函数名：InnerOalSemTake
功能：信号量p操作(外界不调用此函数，调用OalSemTake)
算法实现：（可选项）
引用全局变量：
输入参数说明：hSem: 信号量句柄
s8 strSemName[]:信号量名
s8 strFileName[]:调用所在的文件名
u32 dwLine:调用所在的行
返回值说明：成功返回OALSEM_SIGNALED，否则返回OALSEM_TIMEOUT或者OALSEM_FAILED
====================================================================*/
API u32  InnerOalSemTake(SEMHANDLE hSem,s8 strSemName[],s8 strFileName[],u32 dwLine);
#define OalSlowBadSemTake(phSem) InnerOalSemTake(phSem,#phSem,__FILE__,__LINE__)


/*====================================================================
函数名：InnerOalSemTakeByTime
功能：带超时的信号量p操作(外界不调用此函数，调用OalSemTakeByTime)
算法实现：（可选项）
引用全局变量：
输入参数说明：hSem: 信号量句柄
dwTimeout: 超时时间（ms）,精度为10ms
s8 strSemName[]:信号量名
s8 strFileName[]:调用所在的文件名
u32 dwLine:调用所在的行
返回值说明：成功返回OALSEM_SIGNALED，否则返回OALSEM_TIMEOUT或者OALSEM_FAILED
====================================================================*/
API u32  InnerOalSemTakeByTime(SEMHANDLE hSem, u32 dwTimeout,s8 strSemName[],s8 strFileName[],u32 dwLine);
#define OalSlowBadSemTakeByTime(phSem,dwTimeout) InnerOalSemTakeByTime(phSem,dwTimeout,#phSem,__FILE__,__LINE__)


/*====================================================================
函数名：OalSlowBadSemGive
功能：信号量v操作
算法实现：（可选项）
引用全局变量：
输入参数说明：hSlowBadSem: 信号量句柄
返回值说明：成功返回TRUE，失败返回FALSE.
====================================================================*/
API BOOL OalSlowBadSemGive(SEMHANDLE hSlowBadSem);

/*====================================================================
函数名：OalTakedSemNumGet
功能：得到被take的信号量总数，测试用，用户不要调用 !!!!!!
算法实现：（可选项）
引用全局变量：
输入参数说明：
返回值说明：被take的信号量总数
注： 该接口是为了测试信号量监控是否有效，用户不要调用 !!!!!!
====================================================================*/
API u32 OalTakedSemNumGet(void);

/*====================================================================
函数名：OalSemDump
功能：将所有没有give的信号量的信息输出
算法实现：（可选项）
引用全局变量：
输入参数说明：
  返回值说明：无.
====================================================================*/
API void OalSemDump();


/*====================================== 目录管理  =========================================*/
#define MAX_DIRPATH_LEN		200		//目录操作中路径的最大长度
									//由于 OalIsDirExist 中要把结尾的 '/' 去掉才能正确判断，需拷贝输入的路径再处理，
									//为操作方便，定义此宏

/*====================================================================
函数名: OalDirCreate
功能:	创建目录
算法实现:
引用全局变量:
输入参数说明:	strDirPath - 目录路径
返回值说明： 成功，TRUE；失败，FALSE
====================================================================*/
BOOL OalDirCreate(IN const s8 *strDirPath);


/*====================================================================
函数名:	OalDirDelete
功能:	删除目录
算法实现:
引用全局变量:
输入参数说明:	strDirPath - 目录路径

返回值说明：成功，TRUE；失败，FALSE
====================================================================*/
BOOL OalDirDelete(IN const s8 *strDirPath);


/*====================================================================
函数名:	OalIsDirExist
功能:	目录是否存在
算法实现:
引用全局变量:
输入参数说明:	strDirPath - 目录路径
返回值说明：存在，TRUE；不存在，FALSE
====================================================================*/
BOOL OalIsDirExist(IN const s8 *strDirPath);


/*====================================================================
函数名:	OalIsPathExist
功能:	路径是否存在，可以是目录，也可以是文件
算法实现:
引用全局变量:
输入参数说明:	strDirPath - 目录路径
返回值说明：存在，TRUE；不存在，FALSE
====================================================================*/
BOOL OalIsPathExist(IN const s8 *strDirPath);


/*====================================================================
函数名:	OalFileDelete
功能:	删除文件
算法实现:
引用全局变量:
输入参数说明:	strFilePath - 文件路径

返回值说明：成功，TRUE；失败，FALSE
====================================================================*/
BOOL OalFileDelete(IN const s8 *strFilePath);

/*====================================================================
函数名:	OalDelFilesInDir
功能:	删除目录下的所有文件(不递归删除)
算法实现:
引用全局变量:
输入参数说明:	pchDir - 目录(绝对，相对路径都可以, 不要以 "/", "\\" 结尾)

返回值说明：成功，TRUE；失败，FALSE
====================================================================*/
BOOL OalDelFilesInDir(IN const s8 *pchDir);

/*====================================================================
函数名:	OalGetFilesInDir
功能:	得到目录下的所有文件(不递归)
算法实现:
引用全局变量:
输入参数说明:	pchDir - 目录(绝对，相对路径都可以, 不要以 "/", "\\" 结尾)

返回值说明：成功，OAL_OK；失败，错误码
注意: 如果文件总数大于MAXFILE_PERDIR，返回错误码
	  如果文件总数为0, 返回OAL_OK
====================================================================*/
#define MAXFILE_PERDIR	64
u32 OalGetFilesInDir(IN const s8 *pchDir, IN s8 astrFile[MAXFILE_PERDIR][MAX_FILENAME_LEN+1],
					OUT u32 *pdwGetFileNum);

/*====================================================================
函数名:  OalFileReadableJdg
功能: 判断文件是否可读
算法实现:
引用全局变量:
输入参数说明: strFilePath - 文件路径

返回值说明：成功，返回OAL_OK；失败，返回错误
====================================================================*/
u32 OalFileReadableJdg(IN const s8 *strFilePath);

/*====================================================================
函数名:	OalFileReadableFastJdg
功能:	快速判断文件是否可读
算法实现:
引用全局变量:
输入参数说明:	strFilePath - 文件路径
返回值说明：成功，返回OAL_OK；失败，返回错误码
====================================================================*/
u32 OalFileReadableFastJdg(IN const s8 *strFilePath);



/*=============================================== 系统时间 ================================================*/

/*====================================================================
函数名：OalU64TickGet
功能：取得当前的系统tick数
算法实现
引用全局变量：
输入参数说明：pqwTick － tick指针
返回值说明：
====================================================================*/
API void OalU64TickGet(u64 *pqwTick);

/*====================================================================
函数名：OalClkRateGet
功能：得到tick精度
算法实现：（可选项）
引用全局变量：
输入参数说明：
返回值说明：tick精度
====================================================================*/
API u32 OalTickRateGet(void);

/*====================================================================
函数名：OalGetU64Ms
功能：得到自系统运行后经过的毫秒数
算法实现：（可选项）
引用全局变量：
输入参数说明：pqwMs - ms 指针
返回值说明：
====================================================================*/
API void OalGetU64Ms(u64 *pqwMs);


/*=============================================== 时间函数 ================================================*/

#define TIMESTR_BUFLEN    30 //存放时间字符串的缓冲大小

#define SECONDS_PERMIN    60
#define SECONDS_PERHOUR   (SECONDS_PERMIN*60)
#define SECONDS_PERDAY    (SECONDS_PERHOUR*24)
#define SECONDS_PERWEEK   (SECONDS_PERDAY*7)

#define TZ_LOCAL  100   //本地时区
#define TZ_GMT    0   //格林威治时区，暂不支持

//日历时间
typedef struct tagCalTime {
  u32 m_dwSec;  //秒 - 取值区间为[0,59]
  u32 m_dwMin;  //分 - 取值区间为[0,59]
  u32 m_dwHour; //时 - 取值区间为[0,23]
  u32 m_dwMday; //一个月中的日期 - 取值区间为[1,31]
  u32 m_dwMon;  //月份（从一月开始，1代表一月） - 取值区间为[1,12]
  u32 m_dwYear; //实际年份
  u32 m_dwWday; //星期 - 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推
  u32 m_dwYday; //从每年的1月1日开始的天数 - 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推
  u32 m_dwIsdst;//暂时无用，夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负
}TCalTime;      //成员顺序和tm结构保持一致

typedef u32 STIME;  //从1970年1月1日0时0分0秒 开始的毫秒数
typedef u64 MSTIME;  //从1970年1月1日0时0分0秒 开始的毫秒数

/*===========================================================
函数名： TimeZoneSet
功能： 设置时区
算法实现：
引用全局变量：
输入参数说明：
返回值说明： byTimeZone -- 时区        (目前填 TZ_LOCAL 即可)
             bUseDst -- 是否实行夏令时 (目前填 FALSE)
注: 该函数会影响接口 CalTimeGet, STime2Str, STime2Day, STime2CalTime,
  即对于日历中的 年月日时分秒 有影响。
    对于绝对秒数, 绝对毫秒数, 不影响
===========================================================*/
void TimeZoneSet(IN u8 byTimeZone, BOOL bUseDst);

/*===========================================================
函数名： STimeGet
功能： 得到绝对秒时间, 相对格林威治时间1970年1月1日0时0分0秒
算法实现：
引用全局变量：
输入参数说明：
返回值说明： 秒时间
===========================================================*/
STIME STimeGet(void);

/*===========================================================
函数名： MSTimeGet
功能：得到绝对毫秒时间, 相对格林威治时间1970年1月1日0时0分0秒
算法实现：
引用全局变量：
输入参数说明： ptMsTime - ms 指针
返回值说明：
===========================================================*/
void MSTimeGet(IN OUT MSTIME *ptMsTime);

/*===========================================================
函数名： CalTimeGet
功能： 得到日历时间
算法实现：
引用全局变量：
输入参数说明： ptCalTime -- 日历时间指针

返回值说明：失败，返回NULL；成功，指向日历时间，即传入的参数
===========================================================*/
TCalTime * CalTimeGet(IN OUT TCalTime *ptCalTime);

/*===========================================================
函数名： CalTime2Str
功能： 日历时间转到字符串，例如："Sun Sep 16 01:03:52 1973\n\0"
算法实现：
引用全局变量：
输入参数说明： ptCalTime -- 日历时间指针
         pTimeStr -- 字符串指针
         dwBufLen -- 输入缓冲长度，用户给的缓冲大小为 TIMESTR_BUFLEN 即可

返回值说明：失败，返回NULL。成功，指向字符串，即传入的字符串指针
===========================================================*/
s8* CalTime2Str(IN TCalTime *ptCalTime, OUT s8 *pTimeStr, IN u32 dwBufLen);

/*===========================================================
函数名： STime2Str
功能： 秒时间转到字符串，例如："Sun Sep 16 01:03:52 1973\n\0"
算法实现：
引用全局变量：
输入参数说明： tSTime -- 秒时间
         pTimeStr -- 字符串指针
         dwBufLen -- 输入缓冲长度，用户给的缓冲大小为 TIMESTR_BUFLEN 即可

返回值说明：失败，返回NULL。成功，指向字符串，即传入的字符串指针
===========================================================*/
s8* STime2Str(STIME tSTime, OUT s8 *pTimeStr, IN u32 dwBufLen);

/*===========================================================
函数名： MSTime2STime
功能： 毫秒时间转到秒时间
算法实现：
引用全局变量：
输入参数说明： tMsTime -- 毫秒时间

返回值说明：秒时间
===========================================================*/
STIME MSTime2STime(IN MSTIME tMsTime);

/*===========================================================
函数名： STime2MSTime
功能： 秒时间转到毫秒时间
算法实现：
引用全局变量：
输入参数说明： tSTime -- 秒时间
        ptMsTime -- 毫秒时间指针

返回值说明：
===========================================================*/
void STime2MSTime(IN STIME tSTime, IN OUT MSTIME *ptMsTime);

/*===========================================================
函数名： STime2Day
功能： 秒时间转到天，1970年1月1日0时0分0秒 开始的绝对天数
算法实现：
引用全局变量：
输入参数说明： tSTime -- 秒时间

返回值说明：天
===========================================================*/
u32 STime2Day(IN STIME tSTime);

/*===========================================================
函数名： Day2STime
功能：天转到秒时间
算法实现：
引用全局变量：
输入参数说明： dwDay -- 天

返回值说明：秒时间
===========================================================*/
STIME Day2STime(IN u32 dwDay);

/*===========================================================
函数名：CalTime2STime
功能：日历时间转到秒时间
算法实现：
引用全局变量：
输入参数说明： ptCalTime -- 日历时间指针

返回值说明：秒时间
===========================================================*/
STIME CalTime2STime(IN TCalTime *ptCalTime);

/*===========================================================
函数名：STime2CalTime
功能：秒时间转到日历时间
算法实现：
引用全局变量：
输入参数说明： tSTime -- 秒时间
         ptCalTime -- 日历时间指针

返回值说明：失败，返回NULL。成功，指向日历时间，即传入的指针
===========================================================*/
TCalTime* STime2CalTime(IN STIME tSTime, OUT TCalTime *ptCalTime);

/*===========================================================
函数名：SysTimeSetS
功能：设置系统时间，精度秒
算法实现：
引用全局变量：
输入参数说明： tSTime -- 秒时间
返回值说明：成功，TRUE；失败，FALSE
===========================================================*/
BOOL SysTimeSetS(IN STIME tSTime);

/*===========================================================
函数名：SysTimeSetMS
功能：设置系统时间，精度毫秒
算法实现：
引用全局变量：
输入参数说明： tMSTime -- 毫秒时间
返回值说明：成功，TRUE；失败，FALSE
===========================================================*/
BOOL SysTimeSetMS(IN MSTIME tMSTime);

/*===========================================================
函数名：CalDayOfMonth
功能：计算某个月的天数
算法实现：
引用全局变量：
输入参数说明：
返回值说明：成功，TRUE；失败，FALSE
===========================================================*/
u32 CalDayOfMonth( u32 nYear, u32 nMonth );


/*=============================================== 内存管理 ================================================*/

#define MAX_MEMLIB_NUM					 (u32)16 //内存库实例的最大个数, 定义该宏是为了调试的方便，可以通过内存库索引来输入调试命令

//内存块的类型
#define BLOCKTYPE_CUSTOM		(u32)1 //定制内存块
#define BLOCKTYPE_EXPONENT		(u32)2 //2的n次方内存块
#define BLOCKTYPE_DIRECT		(u32)3 //直通内存块
#define BLOCKTYPE_ALIGN			(u32)4 //align内存块
#define BLOCKTYPE_POOLALIGN		(u32)5 //pool align内存块
#define BLOCKTYPE_NONE			(u32)0

/*===========================================================
函数名： OalMLibGet
功能： 得到oal内部的内存分配器
算法实现：
引用全局变量：
输入参数说明：
返回值说明： 返回内存分配器的句柄，失败返回NULL
===========================================================*/
API HANDLE OalMLibGet();

/*===========================================================
函数名： RsrcTagAlloc
功能： 分配资源标签，可用于内存接口，信号量，线程等资源
算法实现：
引用全局变量： 未引用
输入参数说明：  s8 *pchName - 标签名
u32 dwNameLen - 标签名长度，不包括结尾符'\0', 最多为8
u32 *pdwRsrctag - 返回标签

返回值说明： 成功返回OAL_OK，失败返回错误码
===========================================================*/
API u32 RsrcTagAlloc(IN s8 *pchName, IN u32 dwNameLen, IN OUT u32 *pdwRsrctag);

/*===========================================================
函数名： OalMAllocCreate
功能： 生成内存分配器
算法实现：
引用全局变量： 未引用
输入参数说明：  u32 *pdwCustomMem - 传入定制内存信息, 内存大小不限，数组中的内存大小不能相同。单位为 byte

				u32 dwArrayNum － 数组元素个数，即定制内存有dwArrayNum个不同尺寸


返回值说明： 内存分配器的句柄，失败返回NULL
===========================================================*/
API HANDLE OalMAllocCreate(IN u32 *pdwCustomMem, IN u32 dwArrayNum);


/*===========================================================
函数名： OalMAllocDestroy
功能： 释放内存分配器
算法实现：
引用全局变量： 未引用
输入参数说明：  HANDLE hAlloc 内存分配器的句柄

返回值说明： 成功返回真，否则假
===========================================================*/
API BOOL OalMAllocDestroy(IN HANDLE hAlloc);


/*===========================================================
函数名： OalInnerMAlloc
功能： 分配内存，并将分配信息存入链表中(内部函数，外部调用OalMAlloc)
算法实现：
引用全局变量： 未引用
输入参数说明：  HANDLE hAlloc 内存分配器的句柄
                u32 dwBytes 待分配的大小
				u32 dwTag 标签，由oal动态分配

				BOOL bPool - 是否经过内存池
						FALSE: 直通操作系统的内存分配，即释放，分配都经由操作系统。该选项适合大容量内存的分配，且该内存分配后就不释放。
						TRUE:  分配释放都通过内存池，减少系统调用的次，该选项适合小容量内存（或定制内存）的反复分配，释放。

				const s8 *sFileName 函数调用所在的文件名
				s32 sdwLine 函数调用所在的行

返回值说明： 分配的内存的指针
说明：   用户不要直接调该函数 ！！！！！！！！！
===========================================================*/
API void* OalInnerMAlloc( HANDLE hAlloc, u32 dwBytes, u32 dwTag, BOOL bPool, s8 *sFileName, s32 sdwLine);

/*===========================================================
函数名： OalInnerOsMemAlign
功能：
算法实现：
引用全局变量：

返回值说明： 分配给用户的内存指针
说明：   用户不要直接调该函数 ！！！！！！！！！
===========================================================*/
API void* OalInnerOsMemAlign( HANDLE hAlloc, u32 dwAlign, u32 dwBytes, u32 dwTag, s8 *sFileName, s32 sdwLine);


/*===========================================================
函数名： OalMemAlignMalloc
功能： 分配dwAlign字节对齐的内存
算法实现：
引用全局变量： 未引用
输入参数说明：
返回值说明： 分配的内存的指针
说明：   用户不要直接调该函数 ！！！！！！！！
大于8M, 返回失败
===========================================================*/
API void* OalMemAlignMalloc( HANDLE hAlloc, u32 dwAlign, u32 dwBytes, u32 dwTag, s8 *sFileName, s32 sdwLine);

/*=================================================================================================

注： 使用 OalPoolMalloc分配内存时, 先会查看是否有定制内存，如有则返回定制内存。否则按2的整数幂分配内存，最大分配1M。
	 所以使用 OalPoolMalloc 分配大于8M的内存时，如果没有定制内存，直接从操作系统要，即等同于OalMalloc！

	OalMalloc分配内存，直接调用操作系统的malloc, free

=================================================================================================*/

#define OalMalloc(hAlloc, dwBytes, dwTag)		OalInnerMAlloc(hAlloc, dwBytes, dwTag, FALSE, __FILE__, __LINE__)
#define OalPoolMalloc(hAlloc, dwBytes, dwTag)	OalInnerMAlloc(hAlloc, dwBytes, dwTag, TRUE, __FILE__, __LINE__)
#define OalMemAlign(hAlloc, dwAlign, dwBytes, dwTag)	OalInnerOsMemAlign(hAlloc, dwAlign, dwBytes, dwTag, __FILE__, __LINE__)

/*===========================================================
函数名： OalMFree
功能： 释放内存，将被释放的内存从链表中删除
算法实现：
引用全局变量： 未引用
输入参数说明： HANDLE hAlloc 内存分配器的句柄
               void *pMem 待释放的内存块的指针

返回值说明： 成功返回真，否则假
===========================================================*/
API BOOL OalMFree(IN HANDLE hAlloc, IN void *pMem);


/*===========================================================
函数名： OalMSizeGet
功能： 得到指针指向内存区的大小
算法实现：
引用全局变量： 未引用
输入参数说明：
返回值说明： 成功返回真，否则假
===========================================================*/
API BOOL OalMSizeGet(IN void *pMem, OUT u32 *pdwSize);

/*===========================================================
函数名： OalMDup
功能： 内存拷贝，只增加引用，并不真正拷贝
算法实现：
引用全局变量： 未引用
输入参数说明： HANDLE hAlloc 内存分配器的句柄
               void *pMem 内存块的指针

返回值说明： 成功返回真，否则假
===========================================================*/
API BOOL OalMDup(IN HANDLE hAlloc, IN void *pMem);


/*===========================================================
函数名： OalMUserMemGet
功能： 得到指定标签的用户内存总量
算法实现：
引用全局变量： 未引用
输入参数说明： HANDLE hAlloc － 内存分配器的句柄
               u32 dwTag 标签 －如果为0，得到所有标签的用户内存总量
			   u32 *pdwSize － 用户内存总量

返回值说明： 成功返回真，否则假
===========================================================*/
API BOOL OalMUserMemGet(IN HANDLE hAlloc, IN u32 dwTag, OUT u32 *pdwSize);

/*===========================================================
函数名： OalMSysMemGet
功能： 得到oal内存库向操作系统申请的内存总量
算法实现：
引用全局变量： 未引用
输入参数说明： HANDLE hAlloc 内存分配器的句柄
               u32 *pdwSize － 系统内存总量

返回值说明： 成功返回真，否则假
===========================================================*/
API BOOL OalMSysMemGet(IN HANDLE hAlloc, OUT u32 *pdwSize);

/*===========================================================
函数名： OalMRefGet
功能： 得到内存引用计数
算法实现：
引用全局变量： 未引用
输入参数说明： HANDLE hAlloc 内存分配器的句柄
               void *pMem 内存块的指针
			   u32 *pdwRef - 引用计数

返回值说明： 成功返回真，否则假
===========================================================*/
API BOOL OalMRefGet(IN HANDLE hAlloc, IN void *pMem, OUT u32 *pdwRef);


/*===========================================================
函数名： OalMTypeGet
功能： 得到内存块类型
算法实现：
引用全局变量： 未引用
输入参数说明： HANDLE hAlloc 内存分配器的句柄
               void *pMem 内存块的指针
			   u32 *pdwType - 内存块类型

返回值说明： 成功返回真，否则假
===========================================================*/
API BOOL OalMTypeGet(IN HANDLE hAlloc, IN void *pMem, OUT u32 *pdwType);

/*===========================================================
函数名： OalMBlockNumGet
功能： 得到指定类型，指定大小的内存块的个数
算法实现：
引用全局变量： 未引用
输入参数说明： HANDLE hAlloc － 内存分配器的句柄
               u32 dwBlockType － 内存块类型
			   u32 dwBlockSize - 内存块大小。
								 对于直通内存块，该参数不起作用，返回所有的直通内存块个数
			   u32 *pdwBlockNum － 返回内存块个数

返回值说明： 成功返回真，否则假
===========================================================*/
API BOOL OalMBlockNumGet(IN HANDLE hAlloc, IN u32 dwBlockType, IN u32 dwBlockSize, OUT u32 *pdwBlockNum);


/*===========================================================
函数名： OalMemChk
功能： 检查所有内存库中所有内存是否被破坏，有就打印告警信息
	   该接口仅供调试用，正式版本不要使用 ！！！！！！
算法实现：
引用全局变量：
输入参数说明：

返回值说明：
===========================================================*/
API BOOL OalMemChk(void);


/*=============================================== 系统性能统计 ================================================*/

#define CPU_STATTIME_SHORT  1   //CPU统计间隔，短时间 （s）
#define CPU_STATTIME_MID  10    //CPU统计间隔，中等时间 （s）
#define CPU_STATTIME_LONG 60    //CPU统计间隔，长时间 （s）

/*===========================================================
函数名： CpuUsgGet
功能：得到cpu使用率
算法实现：
引用全局变量： 未引用
输入参数说明： u32 *pdwShortTimeUsg － 短时间的使用率
         u32 *pdwMidTimeUsg － 中等时间的使用率
         u32 *pdwLongTimeUsg － 长时间的使用率

返回值说明： 成功返回 OAL_OK， 失败返回错误码
===========================================================*/
API u32 CpuUsgGet(IN u32 *pdwShortTimeUsg, IN u32 *pdwMidTimeUsg, IN u32 *pdwLongTimeUsg);


/*===========================================================
函数名： MemUsgGet
功能：得到内存使用率
算法实现：
引用全局变量： 未引用
输入参数说明： u32 *pdwTotalMem － 内存总量(Mega Bytes)
         u32 *pdwFreeMem － 空闲内存总量(Mega Bytes)
         u32 *pdwUsg － 内存使用率

返回值说明： 成功返回 OAL_OK， 失败返回错误码
===========================================================*/
API u32 MemUsgGet(IN u32 *pdwTotalMem, IN u32 *pdwFreeMem, IN u32 *pdwUsg);


#ifdef _LINUX_
/*===========================================================
函数名： ExcpCatchSet
功能：设置崩溃转储
算法实现：
引用全局变量： 未引用
输入参数说明： pStrExeFileName － 可执行文件的名字
			   pStrCoreDumpName - core dump 文件名，最长不超过128个字符
返回值说明： 成功,返回TRUE;失败,返回FALSE
===========================================================*/
API BOOL ExcpCatchSet(IN s8 *pStrExeFileName, IN s8 *pStrCoreDumpName);
#endif

/*=================================================================
函 数 名: OalHealthCheck
功    能: 检查oal是否健康
输入参数:
dwLen -- 输入缓冲长度
返回值:    返回  pbyBuf

说明:  健康, OAL_OK; 健康, 错误码
=================================================================*/
u32 OalHealthCheck(void);


#ifdef __cplusplus
}
#endif

#endif //_OAL_H_
