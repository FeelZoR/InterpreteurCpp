/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TestInstPour.h
 * Author: feelzor
 *
 * Created on Oct 28, 2019, 3:51:08 PM
 */

#ifndef TESTINSTPOUR_H
#define TESTINSTPOUR_H

#include <cppunit/extensions/HelperMacros.h>

class TestInstPour : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestInstPour);

    CPPUNIT_TEST(testPour);
    CPPUNIT_TEST(testPourErreur);

    CPPUNIT_TEST_SUITE_END();

public:
    TestInstPour();
    virtual ~TestInstPour();
    void setUp();
    void tearDown();

private:
    void testPour();
    void testPourErreur();
};

#endif /* TESTINSTPOUR_H */

