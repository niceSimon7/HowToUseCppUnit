# -*- coding: utf-8 -*-
"""
config.txt 配置文件
类名
函数1：Case数量
函数2：Case数量(不要有空的行)

示例：
TestDataStructManage
EListInsert:6
EListInsertFirst:6

"""
# %%
IN_FILE = '''E:\\config.txt'''

f0 = open(IN_FILE, 'r')
CLASSNAME = f0.readline().strip()
CLASSNAME_UPPER = CLASSNAME.upper()
OUT_FILE1 = "E:\\" + CLASSNAME + ".h"
OUT_FILE2 = "E:\\" + CLASSNAME + ".cpp"

# breaks
brick1 = '''    CPPUNIT_TEST(testFUNNAMECaseNUM);\n'''
brick2 = '''    void testFUNNAMECaseNUM();\n'''
brick3 = '''void CLASSNAME::testFUNNAMECaseNUM(){

    bool bCondResult = true;
    CPPUNIT_ASSERT(bCondResult);
}

'''

out1 = r'''
*ifndef _HEADER_CUPPER_H_
*define _HEADER_CUPPER_H_

*include <cppunit/TestFixture.h>
*include <cppunit/extensions/HelperMacros.h>

class CLASSNAME : public CppUnit::TestFixture{
    CPPUNIT_TEST_SUITE(CLASSNAME);
    // CPPUNIT_FUN

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

private:
    // TESTCASE_FUN

};

*endif
'''
out1 = out1.replace('*', r'#')

out2 = r'''// class test set
void CLASSNAME::setUp() {

}

// class test clean
void CLASSNAME::tearDown() {

}

    // TESTCASE_FUN

'''

for _ in f0:
    FUNNAME = _.split(':', 1)[0].strip()
    TOTAL = int(_.split(':', 1)[1].strip())
    CPPUNIT_FUN = '''    // CPPUNIT_FUN\n'''
    TESTCASE_FUN = '''    // TESTCASE_FUN\n'''
    TESTCASE_FUN_IMPLE = '''    // TESTCASE_FUN\n'''
    for NUM in range(1, TOTAL + 1):
        temp = brick1
        CPPUNIT_FUN = CPPUNIT_FUN + temp.replace('NUM', str(NUM)).replace('FUNNAME', FUNNAME)
        temp = brick2
        TESTCASE_FUN = TESTCASE_FUN + temp.replace('NUM', str(NUM)).replace('FUNNAME', FUNNAME)
        temp = brick3
        TESTCASE_FUN_IMPLE = TESTCASE_FUN_IMPLE + temp.replace('NUM', str(NUM)).replace('FUNNAME', FUNNAME)

    # insert in right location
    index = out1.rfind('CPPUNIT_FUN')
    out1 = out1[:index - 7] + CPPUNIT_FUN + out1[index - 7:]
    index = out1.rfind('TESTCASE_FUN')
    out1 = out1[:index - 7] + TESTCASE_FUN + out1[index - 7:]
    index = out2.rfind('TESTCASE_FUN')
    out2 = out2[:index - 7] + TESTCASE_FUN_IMPLE + out2[index - 7:]

f0.close()

f1 = open(OUT_FILE1, 'w')
out1 = out1.replace('CLASSNAME', CLASSNAME).replace('CUPPER', CLASSNAME_UPPER)
f1.write(out1)
f1.close()

f2 = open(OUT_FILE2, 'w')
out2 = out2.replace('CLASSNAME', CLASSNAME)
f2.write(out2)
f2.close()

