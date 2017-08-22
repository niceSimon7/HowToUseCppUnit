#include "TestDataStructManage.h"
#include "Oal.h"
#include <iostream>
using namespace std;

// class test set
void TestDataStructManage::setUp() {
	OalInit();
	//待插入指针，数据为4
	tENodeInsert = (ENode*) malloc(sizeof(ENode));
	if (tENodeInsert != NULL)
		*tENodeInsert = {4,NULL,NULL};

	//分配空间，初始化空链表
	tFirstOfListNull = (ENode*) malloc(sizeof(ENode));
	tLastOfListNull = (ENode*) malloc(sizeof(ENode));
	tListNull = (EList*) malloc(sizeof(EList));
	if (tListNull != NULL && tFirstOfListNull != NULL && tLastOfListNull != NULL) {
		*tFirstOfListNull = {NULL,NULL,NULL};
		*tLastOfListNull = {NULL,NULL,NULL};
		*tListNull = {sizeof(EList), *tFirstOfListNull, *tLastOfListNull, 0};
		//EListInit(tListNull); 初始化操作
		tFirstOfListNull->pSuc = tLastOfListNull;
		tLastOfListNull->pPre = tFirstOfListNull;
	}

	//分配空间，不初始化空链表
	tFirstOfListNullNotInit = (ENode*) malloc(sizeof(ENode));
	tLastOfListNullNotInit = (ENode*) malloc(sizeof(ENode));
	tListNull = (EList*) malloc(sizeof(EList));
	if (tListNull != NULL && tFirstOfListNullNotInit != NULL && tLastOfListNullNotInit != NULL) {
		*tFirstOfListNullNotInit = {NULL,NULL,NULL};
		*tLastOfListNullNotInit = {NULL,NULL,NULL};
		*tListNull = {sizeof(EList), *tFirstOfListNull, *tLastOfListNull, 0};
		//不进行初始化操作
	}

	//分配空间，初始化一个含3个数据的链表1,2,3
	tNodeArray = (ENode*) malloc(sizeof(ENode) * 3);
	if (tNodeArray != NULL) {
		tNodeArray[0] = {1, NULL, NULL};
		tNodeArray[1] = {2, NULL, NULL};
		tNodeArray[2] = {3, NULL, NULL};
	}

	tFirstOfListThree = (ENode*) malloc(sizeof(ENode));
	tLastOfListThree = (ENode*) malloc(sizeof(ENode));
	if(tFirstOfListThree!=NULL && tLastOfListThree!=NULL){
		*tFirstOfListThree = {NULL,NULL,NULL};
		*tLastOfListThree = {NULL,NULL,NULL};
	}

	tListThree = (EList*) malloc(sizeof(EList));
	if (tListThree != NULL) {
		//初始化含数据结点的链表
		tFirstOfListThree->pSuc = &tNodeArray[0];
		tNodeArray[0].pSuc = &tNodeArray[1];
		tNodeArray[1].pSuc = &tNodeArray[2];
		tNodeArray[2].pSuc = tLastOfListThree;

		tLastOfListThree->pPre = &tNodeArray[2];
		tNodeArray[2].pPre = &tNodeArray[1];
		tNodeArray[1].pPre = &tNodeArray[0];
		tNodeArray[0].pPre = tFirstOfListThree;

		*tListThree = {sizeof(EList), *tFirstOfListThree, *tLastOfListThree, 3};
	}

}

// class test clean
void TestDataStructManage::tearDown(){
	free(tENodeInsert);
	tENodeInsert =NULL;

	free(tFirstOfListNull);
	tFirstOfListNull =NULL;
	free(tLastOfListNull);
	tLastOfListNull =NULL;
	free(tListNull);
	tListNull =NULL;

	free(tFirstOfListNullNotInit);
	tFirstOfListNullNotInit =NULL;
	free(tLastOfListNullNotInit);
	tLastOfListNullNotInit =NULL;
	free(tListNullNotInit);
	tListNullNotInit =NULL;

	free(tNodeArray);
	tNodeArray =NULL;
	free(tFirstOfListThree);
	tFirstOfListThree =NULL;
	free(tLastOfListThree);
	tLastOfListThree =NULL;
	free(tListThree);
	tListThree =NULL;

	OalExit();

}

// test fun1
void TestDataStructManage::testEListInit(){
	CPPUNIT_ASSERT(EListInitCases(VALID));
	CPPUNIT_ASSERT(EListInitCases(INVALID));
}


BOOL TestDataStructManage::EListInitCases(u8 P1){
	switch(P1)
	{
	case VALID: //一个有效的未初始化链表指针
	{
		EListInit(tListNullNotInit);
		bool cond1 = tFirstOfListNullNotInit->pSuc == tLastOfListNullNotInit;
		bool cond2 = tFirstOfListNullNotInit->pPre == NULL;
		bool cond3 = tLastOfListNullNotInit->pPre == tFirstOfListNullNotInit;
		bool cond4 = tFirstOfListNullNotInit->pSuc == NULL;
		if (cond1 && cond2 && cond3 && cond4) //?未初始化
			return TRUE;
		else
			return FALSE;
		break;
	}
	case INVALID: //一个无效的链表指针
	{
		EList* tInValidList = (EList*) malloc(sizeof(EList));
		EListInit(tInValidList); //？产生什么影响
		break;
	}
	default:
		return FALSE; break;
	}

}

// test fun2
void TestDataStructManage::testEListInsert(){
	CPPUNIT_ASSERT(EListInsertCases(VALID, VALID, VALID));
	CPPUNIT_ASSERT(EListInsertCases(INVALID, VALID, VALID));
	CPPUNIT_ASSERT(EListInsertCases(VALID, VALID, REPEAT));
	CPPUNIT_ASSERT(EListInsertCases(VALID, VALID, NODEOUTOFLIST));
	CPPUNIT_ASSERT(EListInsertCases(VALID, INVALID, VALID));
	CPPUNIT_ASSERT(EListInsertCases(VALID, VALID, NULLNODE));
}

BOOL TestDataStructManage::EListInsertCases(u8 P1,u8 P2,u8 P3){
	// case1
	if (P1 == VALID && P2 == VALID && P3 == VALID) {
		//一个有效的链表指针 tListThree
		//参考结点为数据为2的结点 &tNodeArray[1]
		//一个有效的带插入指针 tNodeInsert
		//返回值检验
		bool cond1 = EListInsert(tListThree, &tNodeArray[1], tENodeInsert);
		//从头至尾检验
		tENodeTemp = tListThree->tFirst.pSuc; //数据为1的结点
		bool cond2 = tENodeTemp->dwUseMagic == 1;
		tENodeTemp = tENodeTemp->pSuc;   //数据为2的结点
		bool cond3 = tENodeTemp->dwUseMagic == 2;
		tENodeTemp = tENodeTemp->pSuc;   //数据为4的结点
		bool cond4 = tENodeTemp->dwUseMagic == 4;
		tENodeTemp = tENodeTemp->pSuc;   //数据为3的结点
		bool cond5 = tENodeTemp->dwUseMagic == 3;
		//从尾至头检验
		tENodeTemp = tListThree->tLast.pPre; //数据为3的结点
		bool cond6 = tENodeTemp->dwUseMagic == 3;
		tENodeTemp = tENodeTemp->pPre;    //数据为4的结点
		bool cond7 = tENodeTemp->dwUseMagic == 4;
		tENodeTemp = tENodeTemp->pPre;    //数据为2的结点
		bool cond8 = tENodeTemp->dwUseMagic == 2;
		tENodeTemp = tENodeTemp->pPre;    //数据为1的结点
		bool cond9 = tENodeTemp->dwUseMagic == 1;

		bool SumCond = cond1 && cond2 && cond3 && cond4 && cond5 && cond6
				&& cond7 && cond8 && cond9;
		if (SumCond)
			return TRUE ;
	}

	// case2
	if (P1 == INVALID && P2 == VALID && P3 == VALID) {
		bool cond1 = EListInsert(tListNullNotInit, &tNodeArray[1],
				tENodeInsert);
		return !cond1;
	}

	// case3
	if (P1 == VALID && P2 == VALID && P3 == REPEAT) {
		ENode* tENodeTemp = (ENode*) malloc(sizeof(ENode));
		*tENodeTemp = {3,NULL,NULL};
		bool cond1 = EListInsert(tListThree, &tNodeArray[1], tENodeTemp);
		free(tENodeTemp);
		tENodeTemp = NULL;

		return !cond1;
	}

	// case4
	if (P1 == VALID && P2 == INVALID && P3 == VALID) {
		bool cond1 = EListInsert(tListThree, tFirstOfListThree, tENodeInsert);
		return !cond1;
	}

	// case5
	if (P1 == VALID && P2 == VALID && P3 == NODEOUTOFLIST) {
		ENode* tENodeTemp = (ENode*) malloc(sizeof(ENode));
		*tENodeTemp = {3,NULL,NULL};
		bool cond1 = EListInsert(tListThree, tENodeTemp, tENodeInsert);
		free(tENodeTemp);
		tENodeTemp = NULL;

		return !cond1;
	}

	// case6
	if (P1 == VALID && P2 == VALID && P3 == NULLNODE) {
		bool cond1 = EListInsert(tListThree, &tNodeArray[1], tENodeTemp);
		return !cond1;
	}

}

// test fun3
void TestDataStructManage::testEListInsertFirst(){
	CPPUNIT_ASSERT(EListInsertFirstCases(VALID, VALID));
	CPPUNIT_ASSERT(EListInsertFirstCases(VALIDNULL, VALID));
	CPPUNIT_ASSERT(EListInsertFirstCases(NOTINIT, VALID));
	CPPUNIT_ASSERT(EListInsertFirstCases(VALID, REPEAT));
	CPPUNIT_ASSERT(EListInsertFirstCases(VALID, INVALID));
}

BOOL TestDataStructManage::EListInsertFirstCases(u8 P1, u8 P2){
	// case1
	if (P1 = VALID, P2 = VALID) {
		bool cond1 = EListInsertFirst(tListThree, tENodeInsert);
		//从头至尾检验
		tENodeTemp = tListThree->tFirst.pSuc; //数据为4的结点
		bool cond2 = tENodeTemp->dwUseMagic == 4;
		tENodeTemp = tENodeTemp->pSuc;   //数据为1的结点
		bool cond3 = tENodeTemp->dwUseMagic == 1;
		tENodeTemp = tENodeTemp->pSuc;   //数据为2的结点
		bool cond4 = tENodeTemp->dwUseMagic == 2;
		tENodeTemp = tENodeTemp->pSuc;   //数据为3的结点
		bool cond5 = tENodeTemp->dwUseMagic == 3;
		//从尾至头检验
		tENodeTemp = tListThree->tLast.pPre; //数据为3的结点
		bool cond6 = tENodeTemp->dwUseMagic == 3;
		tENodeTemp = tENodeTemp->pPre;    //数据为2的结点
		bool cond7 = tENodeTemp->dwUseMagic == 2;
		tENodeTemp = tENodeTemp->pPre;    //数据为1的结点
		bool cond8 = tENodeTemp->dwUseMagic == 1;
		tENodeTemp = tENodeTemp->pPre;    //数据为4的结点
		bool cond9 = tENodeTemp->dwUseMagic == 4;

		bool SumCond = cond1 && cond2 && cond3 && cond4 && cond5
					&& cond6 && cond7 && cond8 && cond9;
		if (SumCond)
			return TRUE;
	}

	// case2
	if (P1 = VALIDNULL, P2 = VALID){
		bool cond1 = EListInsertFirst(tListNull, tENodeInsert);
		//从头至尾检验
		tENodeTemp = tListThree->tFirst.pSuc; //数据为4的结点
		bool cond2 = tENodeTemp->dwUseMagic == 4;
		//从尾至头检验
		tENodeTemp = tListThree->tLast.pPre; //数据为4的结点
		bool cond3 = tENodeTemp->dwUseMagic == 4;

		bool SumCond = cond1 && cond2 && cond3;
		if (SumCond)
			return TRUE;
	}

	// case3
	if (P1 = NOTINIT, P2 = VALID){
		bool cond1 = EListInsertFirst(tListNullNotInit, tENodeInsert);
		return !cond1;
	}

	// case4
	if (P1 = VALID, P2 = REPEAT) {
		ENode* tENodeTemp = (ENode*) malloc(sizeof(ENode));
		*tENodeTemp = {3,NULL,NULL};
		bool cond1 = EListInsertFirst(tListThree, tENodeTemp);
		free(tENodeTemp);
		tENodeTemp = NULL;

		return !cond1;
	}

	// case5
	if (P1 = VALID, P2 = INVALID){
		bool cond1 = EListInsertFirst(tListNull, tENodeTemp);
		return !cond1;
	}
}

// test fun4
void TestDataStructManage::testEListRemoveFirst(){
	CPPUNIT_ASSERT(EListRemoveFirstCases(VALID));
	CPPUNIT_ASSERT(EListRemoveFirstCases(VALIDNULL));
	CPPUNIT_ASSERT(EListRemoveFirstCases(NOTINIT));
}

BOOL TestDataStructManage::EListRemoveFirstCases(u8 P1){
	// case1
	if (P1 == VALID) {
		bool cond1 = EListRemoveFirst(tListThree);
		//从头至尾检验
		tENodeTemp = tListThree->tFirst.pSuc; //数据为2的结点
		bool cond2 = tENodeTemp->dwUseMagic == 2;
		tENodeTemp = tENodeTemp->pSuc;   //数据为3的结点
		bool cond3 = tENodeTemp->dwUseMagic == 3;
		//从尾至头检验
		tENodeTemp = tListThree->tLast.pPre; //数据为3的结点
		bool cond4 = tENodeTemp->dwUseMagic == 3;
		tENodeTemp = tENodeTemp->pPre;    //数据为2的结点
		bool cond5 = tENodeTemp->dwUseMagic == 2;
		bool SumCond = cond1 && cond2 && cond3 && cond4 && cond5;

		if (SumCond)
			return TRUE;
	}

	// case2
	if (P1 == VALIDNULL){
		bool cond1 = EListRemoveFirst(tListNull);
		return !cond1;
	}

	// case3
	if (P1 == NOTINIT){
		bool cond1 = EListRemoveFirst(tListNullNotInit);
		return !cond1;
	}

}

// test fun5
void TestDataStructManage::testEListGetFirst() {
	CPPUNIT_ASSERT(EListGetFirstCases(VALID));
	CPPUNIT_ASSERT(EListGetFirstCases(VALIDNULL));
	CPPUNIT_ASSERT(EListGetFirstCases(NOTINIT));
}

BOOL TestDataStructManage::EListGetFirstCases(u8 P1) {
	// case1
	if (P1 == VALID) {
		tENodeTemp = EListGetFirst(tListThree);
		if (tENodeTemp->dwUseMagic == 1)
			return TRUE ;
	}

	// case2
	if (P1 == VALIDNULL) {
		tENodeTemp = EListGetFirst(tListNull);
		if (tENodeTemp == NULL)
			return TRUE ;
	}

	// case3
	if (P1 == NOTINIT) {
		tENodeTemp = EListGetFirst(tListNull);
		if (tENodeTemp == NULL)
			return TRUE ;
	}
}

// test fun6
void TestDataStructManage::testEListInsertLast(){
	CPPUNIT_ASSERT(EListInsertLastCases(VALID, VALID));
	CPPUNIT_ASSERT(EListInsertLastCases(VALIDNULL, VALID));
	CPPUNIT_ASSERT(EListInsertLastCases(NOTINIT, VALID));
	CPPUNIT_ASSERT(EListInsertLastCases(VALID, REPEAT));
	CPPUNIT_ASSERT(EListInsertLastCases(VALID, NULLNODE));
}

BOOL TestDataStructManage::EListInsertLastCases(u8 P1,u8 P2){
	// case1
if (P1 == VALID && P2 == VALID) {
	bool cond1 = EListInsertLast(tListThree, tENodeInsert);
	//从头至尾检验
	tENodeTemp = tListThree->tFirst.pSuc; //数据为1的结点
	bool cond2 = tENodeTemp->dwUseMagic == 1;
	tENodeTemp = tENodeTemp->pSuc;   //数据为2的结点
	bool cond3 = tENodeTemp->dwUseMagic == 2;
	tENodeTemp = tENodeTemp->pSuc;   //数据为3的结点
	bool cond4 = tENodeTemp->dwUseMagic == 3;
	tENodeTemp = tENodeTemp->pSuc;   //数据为4的结点
	bool cond5 = tENodeTemp->dwUseMagic == 4;
	//从尾至头检验
	tENodeTemp = tListThree->tLast.pPre; //数据为4的结点
	bool cond6 = tENodeTemp->dwUseMagic == 4;
	tENodeTemp = tENodeTemp->pPre;    //数据为3的结点
	bool cond7 = tENodeTemp->dwUseMagic == 3;
	tENodeTemp = tENodeTemp->pPre;    //数据为2的结点
	bool cond8 = tENodeTemp->dwUseMagic == 2;
	tENodeTemp = tENodeTemp->pPre;    //数据为1的结点
	bool cond9 = tENodeTemp->dwUseMagic == 1;

	bool SumCond = cond1 && cond2 && cond3 && cond4 && cond5 && cond6 && cond7
			&& cond8 && cond9;
	if (SumCond)
		return TRUE ;
}

	// case2
	if (P1 == VALIDNULL && P2 == VALID){
		bool cond1 = EListInsertLast(tListNull, tENodeInsert);
		//从头至尾检验
		tENodeTemp = tListThree->tFirst.pSuc; //数据为4的结点
		bool cond2 = tENodeTemp->dwUseMagic == 4;
		//从尾至头检验
		tENodeTemp = tListThree->tLast.pPre; //数据为4的结点
		bool cond3 = tENodeTemp->dwUseMagic == 4;

		bool SumCond = cond1 && cond2 && cond3;
		if (SumCond)
			return TRUE;
	}

	// case3
	if (P1 == NOTINIT && P2 == VALID){
		bool cond1 = EListInsertLast(tListNullNotInit, tENodeInsert);
		return !cond1;
	}

	// case4
	if (P1 == VALID && P2 == REPEAT){
		ENode* tENodeTemp = (ENode*) malloc(sizeof(ENode));
		*tENodeTemp = {2,NULL,NULL};
		bool cond1 = EListInsertLast(tListThree, tENodeTemp);
		free(tENodeTemp);
		tENodeTemp = NULL;

		return !cond1;
	}

	// case5
	if (P1 == VALID && P2 == NULLNODE){
		bool cond1 = EListInsertLast(tListThree, tENodeTemp);
		return !cond1;
	}

}

// test fun7
void TestDataStructManage::testEListRemoveLast(){
	CPPUNIT_ASSERT(EListRemoveLastCases(VALID));
	CPPUNIT_ASSERT(EListRemoveLastCases(VALIDNULL));
	CPPUNIT_ASSERT(EListRemoveLastCases(NOTINIT));
}

BOOL TestDataStructManage::EListRemoveLastCases(u8 P1){
	// case1
	if (P1 == VALID){
		bool cond1 = EListRemoveLast(tListThree);
		//从头至尾检验
		tENodeTemp = tListThree->tFirst.pSuc; //数据为1的结点
		bool cond2 = tENodeTemp->dwUseMagic == 1;
		tENodeTemp = tENodeTemp->pSuc;   //数据为2的结点
		bool cond3 = tENodeTemp->dwUseMagic == 2;
		//从尾至头检验
		tENodeTemp = tListThree->tLast.pPre; //数据为2的结点
		bool cond4 = tENodeTemp->dwUseMagic == 2;
		tENodeTemp = tENodeTemp->pPre;    //数据为1的结点
		bool cond5 = tENodeTemp->dwUseMagic == 1;

		bool SumCond = cond1 && cond2 && cond3 && cond4 && cond5;
		if (SumCond)
			return TRUE;
	}

	// case2
	if (P1 == VALIDNULL){
		bool cond1 = EListRemoveLast(tListNull);
		return !cond1;
	}

	// case3
	if (P1 == NOTINIT){
		bool cond1 = EListRemoveLast(tListNullNotInit);
		return !cond1;
	}
}


// test fun8
void TestDataStructManage::testEListGetLast(){
	CPPUNIT_ASSERT(1==1);
}

BOOL TestDataStructManage::EListGetLastCases(u8 P1){

}


// test fun9
void TestDataStructManage::testEListRemove(){
	CPPUNIT_ASSERT(1==1);
}

BOOL TestDataStructManage::EListRemoveCases(u8 P1,u8 P2){

}

// test fun10
void TestDataStructManage::testEListClear(){
	CPPUNIT_ASSERT(1==1);
}

BOOL TestDataStructManage::EListClearCases(u8 P1){

}

// test fun11
void TestDataStructManage::testEListNext(){
	CPPUNIT_ASSERT(1==1);
}

BOOL TestDataStructManage::EListNextCases(u8 P1,u8 P2){

}

// test fun12
void TestDataStructManage::testEListPre(){
	CPPUNIT_ASSERT(1==1);
}

BOOL TestDataStructManage::EListPreCases(u8 P1,u8 P2){

}

// test fun13
void TestDataStructManage::testEListIsEmpty(){
	CPPUNIT_ASSERT(1==1);
}

BOOL TestDataStructManage::EListIsEmptyCases(u8 P1){

}

// test fun14
void TestDataStructManage::testEListSize(){
	CPPUNIT_ASSERT(1==1);
}

BOOL TestDataStructManage::EListSizeCases(u8 P1){

}
