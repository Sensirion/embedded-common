#ifndef SENSIRION_TEST_SETUP_H
#define SENSIRION_TEST_SETUP_H

#include <stdio.h>

#include "CppUTest/TestHarness.h"

#define CHECK_ZERO(actual) CHECK_EQUAL(0, (actual))
#define CHECK_ZERO_TEXT(actual, text) CHECK_EQUAL_TEXT(0, (actual), (text))

int main(int argc, char** argv);

#endif /* SENSIRION_TEST_SETUP_H */
