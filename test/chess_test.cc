#include "gtest/gtest.h"
#include "gmock/gmock.h"

/*!
 * This class is responsible for running all tests and is the
 * main entry point for the test suite. To generate an XML test 
 * report, specify the command line argument --gtest_output 
 * xml:path_to_file. To run a subset of test cases, specify the 
 * command line argument --gtest_filter which accepts colon delimited
 * regex strings indicating which tests to run.
 */
int main( int argc, char **argv ) {
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
