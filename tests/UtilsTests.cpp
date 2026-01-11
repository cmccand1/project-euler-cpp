//
// Created by Clint McCandless on 9/17/25.
//
#include "euler/util.hpp"
#include "gtest/gtest.h"
#include <limits>


class UtilsTests : public ::testing::Test {
  public:
    UtilsTests() {
      // initialization;
      // can also be done in SetUp()
    }

    void SetUp() override {
      // initialization or some code to run before each test
    }

    void TearDown() override {
      // code to run after each test;
      // can be used instead of a destructor,
      // but exceptions can be handled in this function only
    }

    ~UtilsTests() override {
      //resources cleanup, no exceptions allowed
    }

    // shared user data
};

// Parameterized tests for digit_sum edge cases
using boost::multiprecision::cpp_int;

struct DigitSumCase {
  std::string input;
  cpp_int expected;
};

class DigitSumParamTest : public ::testing::TestWithParam<DigitSumCase> {
};

TEST_P(DigitSumParamTest, digit_sum) {
  const auto &[input, expected] = GetParam();
  EXPECT_EQ(digit_sum(cpp_int(input)), expected) << "input=" << input;
}

INSTANTIATE_TEST_SUITE_P(
  DigitSumEdgeCases,
  DigitSumParamTest,
  ::testing::Values(
    DigitSumCase{"0", 0},
    DigitSumCase{"-0", 0},
    DigitSumCase{"5", 5},
    DigitSumCase{"10", 1},
    DigitSumCase{"-123", 6},
    DigitSumCase{"909090", 27},
    DigitSumCase{"1000000000000000000000000000000", 1}, // 1e30
    DigitSumCase{"99999999999999999999", 9ULL * 20}, // 20 digits of 9 -> 180
    DigitSumCase{"-99999999999999999999", 9ULL * 20},
    DigitSumCase{"18446744073709551615", 87}, // max uint64 value
    DigitSumCase{"1234567890", 45}
  )
);

// Parameterized tests for factorial
struct FactorialCase {
  int n;
  cpp_int expected;
};

class FactorialParamTest : public ::testing::TestWithParam<FactorialCase> {
};

TEST_P(FactorialParamTest, factorial_values) {
  const auto &[n, expected] = GetParam();
  EXPECT_EQ(factorial(n), expected) << "n=" << n;
}

INSTANTIATE_TEST_SUITE_P(
  FactorialBasicCases,
  FactorialParamTest,
  ::testing::Values(
    FactorialCase{0, cpp_int(1)},
    FactorialCase{1, cpp_int(1)},
    FactorialCase{2, cpp_int(2)},
    FactorialCase{3, cpp_int(6)},
    FactorialCase{4, cpp_int(24)},
    FactorialCase{5, cpp_int(120)},
    FactorialCase{10, cpp_int(3628800)},
    FactorialCase{20, cpp_int("2432902008176640000")}
  )
);


// Parameterized tests for digit_count (unsigned long long)
struct DigitCountULLCase {
  unsigned long long n;
  std::size_t expected;
};

class DigitCountULLParamTest : public ::testing::TestWithParam<DigitCountULLCase> {
};

TEST_P(DigitCountULLParamTest, digit_count_ull) {
  const auto &[n, expected] = GetParam();
  EXPECT_EQ(digit_count(n), expected) << "n=" << n;
}

INSTANTIATE_TEST_SUITE_P(
  DigitCountULLCases,
  DigitCountULLParamTest,
  ::testing::Values(
    DigitCountULLCase{0ULL, 1},
    DigitCountULLCase{1ULL, 1},
    DigitCountULLCase{9ULL, 1},
    DigitCountULLCase{10ULL, 2},
    DigitCountULLCase{99ULL, 2},
    DigitCountULLCase{100ULL, 3},
    DigitCountULLCase{std::numeric_limits<unsigned long long>::max(), 20}
  )
);

// Parameterized tests for digit_count (cpp_int)
struct DigitCountBigCase {
  std::string input;
  std::size_t expected;
};

class DigitCountBigParamTest : public ::testing::TestWithParam<DigitCountBigCase> {
};

TEST_P(DigitCountBigParamTest, digit_count_cpp_int) {
  const auto &[input, expected] = GetParam();
  EXPECT_EQ(digit_count(cpp_int(input)), expected) << "input=" << input;
}

INSTANTIATE_TEST_SUITE_P(
  DigitCountBigCases,
  DigitCountBigParamTest,
  ::testing::Values(
    DigitCountBigCase{"0", 1},
    DigitCountBigCase{"1", 1},
    DigitCountBigCase{"9", 1},
    DigitCountBigCase{"10", 2},
    DigitCountBigCase{"99", 2},
    DigitCountBigCase{"1000", 4},
    DigitCountBigCase{"18446744073709551615", 20},
    DigitCountBigCase{"123456789012345678901234567890", 30},
    DigitCountBigCase{"-12345", 5},
    DigitCountBigCase{"-123", 3},
    DigitCountBigCase{"-1", 1},
    DigitCountBigCase{"-0", 1}
  )
);

// Parameterized tests for is_prime
struct PrimeCase {
  std::size_t n;
  bool expected;
};

class IsPrimeParamTest : public ::testing::TestWithParam<PrimeCase> {
};

TEST_P(IsPrimeParamTest, is_prime_values) {
  const auto &[n, expected] = GetParam();
  EXPECT_EQ(is_prime(n), expected) << "n=" << n;
}

INSTANTIATE_TEST_SUITE_P(
  IsPrimeBasicCases,
  IsPrimeParamTest,
  ::testing::Values(
    PrimeCase{0, false},
    PrimeCase{1, false},
    PrimeCase{2, true},
    PrimeCase{3, true},
    PrimeCase{4, false},
    PrimeCase{5, true},
    PrimeCase{6, false},
    PrimeCase{7, true},
    PrimeCase{8, false},
    PrimeCase{9, false},
    PrimeCase{10, false},
    PrimeCase{11, true},
    PrimeCase{12, false},
    PrimeCase{13, true},
    PrimeCase{16, false},
    PrimeCase{17, true},
    PrimeCase{19, true},
    PrimeCase{20, false},
    PrimeCase{23, true},
    PrimeCase{25, false},
    PrimeCase{29, true},
    PrimeCase{31, true},
    PrimeCase{49, false},
    PrimeCase{97, true},
    PrimeCase{99, false},
    PrimeCase{101, true}
  )
);
