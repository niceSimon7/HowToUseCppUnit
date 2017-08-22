/*
 * TestOalMain.cpp
 *
 *  Created on: 13 Sep 2016
 *      Author: osboxes
*/
#include <iostream>
#include <string>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>

#include "Oal.h"
#include "TestDataStructManage.h"

using namespace std;


//global function-----------------------------------------------
void RunTests(char *xmlFile) {
	CPPUNIT_NS::TestResult testresult;

	CPPUNIT_NS::TestResultCollector collectedresults;
	testresult.addListener(&collectedresults);

	CPPUNIT_NS::BriefTestProgressListener progress;
	testresult.addListener(&progress);

	CPPUNIT_NS::TestRunner testrunner;
	testrunner.addTest(
	CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	testrunner.run(testresult);

	CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults,
			std::cerr);
	compileroutputter.write();

	ofstream xmlFileOut(xmlFile);
	CppUnit::XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	xmlOut.write();

	if ( !collectedresults.wasSuccessful())
		cerr <<"Run Error!"<<endl;

}

int main(int argc, char *argv[]){
	CPPUNIT_TEST_SUITE_REGISTRATION(TestDataStructManage); //可加多个测试类
	//CPPUNIT_TEST_SUITE_REGISTRATION(TestDirectoryManage);
	//CPPUNIT_TEST_SUITE_REGISTRATION(TestMemoryManage);
	//CPPUNIT_TEST_SUITE_REGISTRATION(TestSystemPerform);
	//CPPUNIT_TEST_SUITE_REGISTRATION(TestSystemTime);
	//CPPUNIT_TEST_SUITE_REGISTRATION(TestTaskManage);
	//CPPUNIT_TEST_SUITE_REGISTRATION(TestTimeFunction);

	RunTests("cppTestOalResults.xml"); //+ Run TestCase1

	return 0;
}

