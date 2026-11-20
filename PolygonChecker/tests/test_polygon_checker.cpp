
#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <limits>
#include <unistd.h>

// Link the C functions correctly with GoogleTest
extern "C" {
#include "triangleSolver.h"
#include "four_points.h"
}

// Simulates keyboard input for analyzeTriangle()
static bool runTriangleWithInput(const char* input, float result[4]) {
    FILE* file = tmpfile();

    if (file == nullptr) {
        return false;
    }

    if (fputs(input, file) == EOF) {
        fclose(file);
        return false;
    }

    rewind(file);

    int originalStdin = dup(STDIN_FILENO);

    if (originalStdin < 0) {
        fclose(file);
        return false;
    }

    if (dup2(fileno(file), STDIN_FILENO) < 0) {
        close(originalStdin);
        fclose(file);
        return false;
    }

    clearerr(stdin);
    analyzeTriangle(result);

    bool restored = dup2(originalStdin, STDIN_FILENO) >= 0;

    close(originalStdin);
    fclose(file);
    clearerr(stdin);

    return restored;
}

// ==================================================
// TRIANGLE TYPE TESTS
// ==================================================

class TriangleTypeTests : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        // Prevent buffered input from leaking between tests
        setvbuf(stdin, nullptr, _IONBF, 0);
    }

    void checkType(const char* input, int expectedType) {
        float result[4] = {};

        ASSERT_TRUE(runTriangleWithInput(input, result));
        EXPECT_FLOAT_EQ(static_cast<float>(expectedType), result[0]);

        if (expectedType == 1) {
            EXPECT_FLOAT_EQ(0.0f, result[1]);
            EXPECT_FLOAT_EQ(0.0f, result[2]);
            EXPECT_FLOAT_EQ(0.0f, result[3]);
        }
    }
};

TEST_F(TriangleTypeTests, ValidEquilateralTriangle) {
    checkType("5\n5\n5\n", 2);
}

TEST_F(TriangleTypeTests, ValidIsoscelesTriangle) {
    checkType("5\n5\n6\n", 3);
}

TEST_F(TriangleTypeTests, ValidScaleneTriangle) {
    checkType("3\n4\n5\n", 4);
}

TEST_F(TriangleTypeTests, InvalidTriangleSides) {
    checkType("1\n2\n10\n", 1);
}

TEST_F(TriangleTypeTests, TriangleInequalityBoundary) {
    checkType("1\n2\n3\n", 1);
}

TEST_F(TriangleTypeTests, ZeroLengthSide) {
    EXPECT_FALSE(validateTriangleSide(0));
}

TEST_F(TriangleTypeTests, NegativeSideLength) {
    EXPECT_FALSE(validateTriangleSide(-5));
}

TEST_F(TriangleTypeTests, PositiveSideLength) {
    EXPECT_TRUE(validateTriangleSide(5));
}

// ==================================================
// TRIANGLE INSIDE ANGLE TESTS
// ==================================================

class TriangleAngleTests : public ::testing::Test {};

TEST_F(TriangleAngleTests, EquilateralTriangleAngles) {
    float angles[3] = {};

    getAngleInput(5, 5, 5, angles);

    EXPECT_NEAR(60.0, angles[0], 0.01);
    EXPECT_NEAR(60.0, angles[1], 0.01);
    EXPECT_NEAR(60.0, angles[2], 0.01);
}

TEST_F(TriangleAngleTests, IsoscelesTriangleAngles) {
    float angles[3] = {};

    getAngleInput(5, 5, 6, angles);

    EXPECT_NEAR(angles[0], angles[1], 0.01);
    EXPECT_NEAR(180.0,
                angles[0] + angles[1] + angles[2],
                0.01);
}

TEST_F(TriangleAngleTests, ScaleneTriangleAngles) {
    float angles[3] = {};

    getAngleInput(4, 5, 6, angles);

    EXPECT_GT(angles[0], 0);
    EXPECT_GT(angles[1], 0);
    EXPECT_GT(angles[2], 0);

    EXPECT_NEAR(180.0,
                angles[0] + angles[1] + angles[2],
                0.01);
}

TEST_F(TriangleAngleTests, RightTriangleAngles) {
    float angles[3] = {};

    getAngleInput(3, 4, 5, angles);

    EXPECT_NEAR(36.87, angles[0], 0.01);
    EXPECT_NEAR(53.13, angles[1], 0.01);
    EXPECT_NEAR(90.00, angles[2], 0.01);
}

TEST_F(TriangleAngleTests, AnglesSumTo180) {
    float angles[3] = {};

    getAngleInput(7, 8, 9, angles);

    EXPECT_NEAR(180.0,
                angles[0] + angles[1] + angles[2],
                0.01);
}

TEST_F(TriangleAngleTests, LargestSideHasLargestAngle) {
    float angles[3] = {};

    getAngleInput(3, 4, 5, angles);

    EXPECT_GT(angles[2], angles[0]);
    EXPECT_GT(angles[2], angles[1]);
}

// ==================================================
// FOUR-POINT FUNCTIONALITY TESTS
// ==================================================

class FourPointTests : public ::testing::Test {
protected:
    void checkRectangle(const Point points[4],
                        double expectedPerimeter,
                        double expectedArea) {

        FourPointResult result = {};

        ASSERT_EQ(1, analyzeFourPoints(points, &result));
        EXPECT_EQ(1, result.isRectangle);
        EXPECT_NEAR(expectedPerimeter, result.perimeter, 1e-6);
        EXPECT_NEAR(expectedArea, result.area, 1e-6);
    }
};

TEST_F(FourPointTests, ValidRectangleInOrder) {
    Point points[4] = {
        {1, 1}, {1, 7}, {3, 7}, {3, 1}
    };

    checkRectangle(points, 16.0, 12.0);
}

TEST_F(FourPointTests, ValidRectangleOutOfOrder) {
    Point points[4] = {
        {1, 1}, {1, 7}, {3, 1}, {3, 7}
    };

    checkRectangle(points, 16.0, 12.0);
}

TEST_F(FourPointTests, AllPossiblePointOrders) {
    std::array<Point, 4> corners = {{
        {1, 1}, {1, 7}, {3, 1}, {3, 7}
    }};

    std::array<int, 4> order = {0, 1, 2, 3};
    int count = 0;

    do {
        Point points[4];

        for (int i = 0; i < 4; i++) {
            points[i] = corners[order[i]];
        }

        FourPointResult result = {};

        ASSERT_EQ(1, analyzeFourPoints(points, &result));
        EXPECT_EQ(1, result.isRectangle);
        EXPECT_NEAR(16.0, result.perimeter, 1e-6);
        EXPECT_NEAR(12.0, result.area, 1e-6);

        count++;

    } while (std::next_permutation(
        order.begin(), order.end()
    ));

    EXPECT_EQ(24, count);
}

TEST_F(FourPointTests, RotatedRectangle) {
    Point points[4] = {
        {0, 0}, {4, 2}, {3, 4}, {-1, 2}
    };

    double perimeter =
        2.0 * (std::sqrt(20.0) + std::sqrt(5.0));

    checkRectangle(points, perimeter, 10.0);
}

TEST_F(FourPointTests, SquareIsRectangle) {
    Point points[4] = {
        {0, 0}, {4, 4}, {0, 4}, {4, 0}
    };

    checkRectangle(points, 16.0, 16.0);
}

TEST_F(FourPointTests, NegativeCoordinates) {
    Point points[4] = {
        {-3, -2}, {-3, 2}, {3, -2}, {3, 2}
    };

    checkRectangle(points, 20.0, 24.0);
}

TEST_F(FourPointTests, DecimalCoordinates) {
    Point points[4] = {
        {0.5, 0.5}, {0.5, 2.5},
        {3.5, 0.5}, {3.5, 2.5}
    };

    checkRectangle(points, 10.0, 6.0);
}

TEST_F(FourPointTests, ParallelogramNotRectangle) {
    Point points[4] = {
        {0, 0}, {4, 0}, {6, 3}, {2, 3}
    };

    FourPointResult result = {};

    ASSERT_EQ(1, analyzeFourPoints(points, &result));
    EXPECT_EQ(0, result.isRectangle);

    double perimeter =
        2.0 * (4.0 + std::sqrt(13.0));

    EXPECT_NEAR(perimeter, result.perimeter, 1e-6);
    EXPECT_DOUBLE_EQ(0.0, result.area);
}

TEST_F(FourPointTests, TrapezoidNotRectangle) {
    Point points[4] = {
        {0, 0}, {6, 0}, {4, 3}, {1, 3}
    };

    FourPointResult result = {};

    ASSERT_EQ(1, analyzeFourPoints(points, &result));
    EXPECT_EQ(0, result.isRectangle);
    EXPECT_GT(result.perimeter, 0.0);
    EXPECT_DOUBLE_EQ(0.0, result.area);
}

TEST_F(FourPointTests, DuplicatePoints) {
    Point points[4] = {
        {0, 0}, {4, 0}, {4, 0}, {0, 3}
    };

    FourPointResult result = {};

    EXPECT_EQ(0, analyzeFourPoints(points, &result));
    EXPECT_EQ(0, result.isRectangle);
}

TEST_F(FourPointTests, InvalidNaNCoordinate) {
    double nanValue =
        std::numeric_limits<double>::quiet_NaN();

    Point points[4] = {
        {0, 0}, {4, 0}, {4, nanValue}, {0, 3}
    };

    FourPointResult result = {};

    EXPECT_EQ(0, analyzeFourPoints(points, &result));
}

TEST_F(FourPointTests, InfiniteCoordinate) {
    double infinity =
        std::numeric_limits<double>::infinity();

    Point points[4] = {
        {0, 0}, {4, 0}, {4, 3}, {infinity, 3}
    };

    FourPointResult result = {};

    EXPECT_EQ(0, analyzeFourPoints(points, &result));
}

TEST_F(FourPointTests, NullPoints) {
    FourPointResult result = {};

    EXPECT_EQ(0, analyzeFourPoints(nullptr, &result));
}

TEST_F(FourPointTests, NullResult) {
    Point points[4] = {
        {0, 0}, {4, 0}, {4, 3}, {0, 3}
    };

    EXPECT_EQ(0, analyzeFourPoints(points, nullptr));
}

TEST_F(FourPointTests, CollinearPointsNotRectangle) {
    Point points[4] = {
        {0, 0}, {1, 0}, {2, 0}, {3, 0}
    };

    FourPointResult result = {};

    ASSERT_EQ(1, analyzeFourPoints(points, &result));
    EXPECT_EQ(0, result.isRectangle);
    EXPECT_DOUBLE_EQ(0.0, result.area);
}

TEST_F(FourPointTests, PerimeterAndAreaCalculation) {
    Point points[4] = {
        {0, 0}, {10, 0}, {10, 5}, {0, 5}
    };

    checkRectangle(points, 30.0, 50.0);
}

TEST_F(FourPointTests, InvalidInputResetsResults) {
    Point points[4] = {
        {0, 0}, {0, 0}, {3, 3}, {3, 0}
    };

    FourPointResult result = {1, 100.0, 100.0};

    EXPECT_EQ(0, analyzeFourPoints(points, &result));
    EXPECT_EQ(0, result.isRectangle);
    EXPECT_DOUBLE_EQ(0.0, result.perimeter);
    EXPECT_DOUBLE_EQ(0.0, result.area);
}
