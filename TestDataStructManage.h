/*
 * TestDataStructManage.h
 *
 *  Created on: 13 Sep 2016
 *      Author: osboxes
 */

#ifndef _HEADER_TESTDATASTRUCTMANAGE_H_
#define _HEADER_TESTDATASTRUCTMANAGE_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Oal.h"      //功能类头文件

// #define            //Case状态码
// test fun1
#define VALID    20
#define INVALID  21

// test fun2
#define REPEAT 22
#define NODEOUTOFLIST 23
#define NULLNODE 24

// test fun3
#define VALIDNULL 25
#define NOTINIT 26


class TestDataStructManage : public CppUnit::TestFixture{
	CPPUNIT_TEST_SUITE(TestDataStructManage);
	CPPUNIT_TEST(testEListInit);  //要测的函数(14个)
	CPPUNIT_TEST(testEListInsert);
	CPPUNIT_TEST(testEListInsertFirst);
	CPPUNIT_TEST(testEListRemoveFirst);
	CPPUNIT_TEST(testEListGetFirst);
	CPPUNIT_TEST(testEListInsertLast);
	CPPUNIT_TEST(testEListRemoveLast);
	CPPUNIT_TEST(testEListGetLast);
	CPPUNIT_TEST(testEListRemove);
	CPPUNIT_TEST(testEListClear);
	CPPUNIT_TEST(testEListNext);
	CPPUNIT_TEST(testEListPre);
	CPPUNIT_TEST(testEListIsEmpty);
	CPPUNIT_TEST(testEListSize);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();      //测每个函数时的初始化操作
	void tearDown();   //测每个函数时的清理操作

protected:
	void testEListInit();   //每个函数表示成一个测试Suite，加入CPPUNIT
	void testEListInsert();
	void testEListInsertFirst();
	void testEListRemoveFirst();
	void testEListGetFirst();
	void testEListInsertLast();
	void testEListRemoveLast();
	void testEListGetLast();
	void testEListRemove();
	void testEListClear();
	void testEListNext();
	void testEListPre();
	void testEListIsEmpty();
	void testEListSize();

private:  //（公共部分）使用的变量
	EList* tListThree;               //已初始化，数据为1,2,3
	EList* tListNull;               //已初始化，空链表
	EList* tListNullNotInit;        //未初始化，空链表
	ENode* tENodeTemp = NULL;       //临时使用
	ENode* tENodeInsert;            //待插入结点，数据为4
	ENode* tFirstOfListNull;
	ENode* tLastOfListNull;
	ENode* tFirstOfListNullNotInit;
	ENode* tLastOfListNullNotInit;
	ENode* tNodeArray;
	ENode* tFirstOfListThree;
	ENode* tLastOfListThree;

	BOOL EListInitCases(u8 P1); //每个函数的测试Suite实现，在ASSERT中使用。这里用参数分类。
	BOOL EListInsertCases(u8 P1,u8 P2,u8 P3);
	BOOL EListInsertFirstCases(u8 P1, u8 P2);
	BOOL EListRemoveFirstCases(u8 P1);
	BOOL EListGetFirstCases(u8 P1);
	BOOL EListInsertLastCases(u8 P1,u8 P2);
	BOOL EListRemoveLastCases(u8 P1);
	BOOL EListGetLastCases(u8 P1);
	BOOL EListRemoveCases(u8 P1,u8 P2);
	BOOL EListClearCases(u8 P1);
	BOOL EListNextCases(u8 P1,u8 P2);
	BOOL EListPreCases(u8 P1,u8 P2);
	BOOL EListIsEmptyCases(u8 P1);
	BOOL EListSizeCases(u8 P1);
};

#endif /* HEADER_TESTDATASTRUCTMANAGE_H_ */
