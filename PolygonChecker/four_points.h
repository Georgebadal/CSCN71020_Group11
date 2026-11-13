#ifndef FOUR_POINTS_H
#define FOUR_POINTS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    int isRectangle;
    double perimeter;
    double area;
} FourPointResult;

// Checks the four points and calculates the results
int analyzeFourPoints(const Point points[4], FourPointResult *result);

// Takes user input and displays the results
int runFourPointFeature(void);

#ifdef __cplusplus
}
#endif

#endif