#include "Matrix.h"
#include <gtest/gtest.h>

using namespace Homework;

TEST(matrixTest, emptyMatrix) {
    Matrix<int, -1> matrix;

    ASSERT_EQ(0, matrix.size());
}

TEST(matrixTest, nonEmptyMatrix) {
    //given
    Matrix<int, -1, 5> matrix;

    //when
    matrix[2][5][4][3][1] = 10;
    matrix[2][5][6][3][1] = 20;
    matrix[3][5][6][3][1] = 30;

    //then
    ASSERT_EQ(3, matrix.size());
    ASSERT_EQ(10, static_cast<int>(matrix[2][5][4][3][1]));
    ASSERT_EQ(20, static_cast<int>(matrix[2][5][6][3][1]));
    ASSERT_EQ(30, static_cast<int>(matrix[3][5][6][3][1]));
}

TEST(matrixTest, eraseElement) {
    //given
    Matrix<int, -1, 5> matrix;

    matrix[2][5][4][3][1] = 10;
    matrix[2][5][6][3][1] = 20;
    matrix[3][5][6][3][1] = 30;

    ASSERT_EQ(3, matrix.size());

    //when
    matrix[2][5][6][3][1] = -1;

    //then
    ASSERT_EQ(2, matrix.size());
    ASSERT_EQ(10, static_cast<int>(matrix[2][5][4][3][1]));
    ASSERT_EQ(-1, static_cast<int>(matrix[2][5][6][3][1]));
    ASSERT_EQ(30, static_cast<int>(matrix[3][5][6][3][1]));
}

TEST(matrixTest, assignElement) {
    //given
    Matrix<int, -1, 3> matrix;
    int value = 217;

    //when
    ((matrix[100][100][100] = 314) = 0) = value;

    //then
    ASSERT_EQ(value, static_cast<int>(matrix[100][100][100]));
}
