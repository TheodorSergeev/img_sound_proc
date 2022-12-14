#include <gtest/gtest.h>
#include "utils.hpp"
//#include "transforms.hpp"
//#include "parsing.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(UtilsTest, opencv2eigen) {
    cv::Mat image(2, 3, CV_8U, 4);
    MatrixXi test_mat = opencv2eigen(image);

    // Check matrix size
    EXPECT_EQ(test_mat.cols(), 3);
    EXPECT_EQ(test_mat.rows(), 2);

    // Check matrix contents
    EXPECT_EQ(test_mat.minCoeff(), 4);
    EXPECT_EQ(test_mat.maxCoeff(), 4);
}

TEST(UtilsTest, eigen2opencv) {
    MatrixXi image(2, 3);
    image << 4, 4, 4, 
             4, 4, 4;
    cv::Mat test_mat = eigen2opencv(image);

    // Check matrix size
    EXPECT_EQ(test_mat.cols, 3);
    EXPECT_EQ(test_mat.rows, 2);

    // Check matrix contents
    double test_min, test_max;
    cv::minMaxLoc(test_mat, &test_min, &test_max);
    EXPECT_EQ(test_min, 4);
    EXPECT_EQ(test_max, 4);
}