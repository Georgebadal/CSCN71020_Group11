#include "four_points.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#define INPUT_SIZE 128
#define TOLERANCE 1e-9

// Accepts and validates user input
static int readNumber(const char *prompt, double *value) {
    char input[INPUT_SIZE];

    while (1) {
        char *end;

        printf("%s", prompt);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 0;
        }

        // Prevent input from being too long
        if (strchr(input, '\n') == NULL && !feof(stdin)) {
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            printf("Input is too long.\n");
            continue;
        }

        errno = 0;
        double number = strtod(input, &end);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (end == input || *end != '\0' ||
            errno == ERANGE || !isfinite(number)) {

            printf("Invalid input. Enter a valid number.\n");
            continue;
        }

        *value = number;
        return 1;
    }
}

// Checks if four points form a rectangle
int analyzeFourPoints(const Point points[4],
                      FourPointResult *result) {

    double dx[4], dy[4], lengths[4];
    double perimeter = 0;
    int rectangle = 1;

    if (points == NULL || result == NULL) {
        return 0;
    }

    result->isRectangle = 0;
    result->perimeter = 0;
    result->area = 0;

    // Validate all four points
    for (int i = 0; i < 4; i++) {

        if (!isfinite(points[i].x) ||
            !isfinite(points[i].y)) {
            return 0;
        }

        // Check for duplicate points
        for (int j = 0; j < i; j++) {

            if (points[i].x == points[j].x &&
                points[i].y == points[j].y) {
                return 0;
            }
        }
    }

    // Connect the four points into lines
    for (int i = 0; i < 4; i++) {

        int j = (i + 1) % 4;

        dx[i] = points[j].x - points[i].x;
        dy[i] = points[j].y - points[i].y;

        if (!isfinite(dx[i]) || !isfinite(dy[i])) {
            return 0;
        }

        // Calculate the length of each line
        lengths[i] = hypot(dx[i], dy[i]);

        if (!isfinite(lengths[i]) || lengths[i] <= 0) {
            return 0;
        }

        perimeter += lengths[i];

        if (!isfinite(perimeter)) {
            return 0;
        }
    }

    // Check if all four angles are 90 degrees
    for (int i = 0; i < 4; i++) {

        int j = (i + 1) % 4;

        double dot =
            (dx[i] / lengths[i]) * (dx[j] / lengths[j]) +
            (dy[i] / lengths[i]) * (dy[j] / lengths[j]);

        if (fabs(dot) > TOLERANCE) {
            rectangle = 0;
            break;
        }
    }

    result->perimeter = perimeter;
    result->isRectangle = rectangle;

    // Calculate area only if it is a rectangle
    if (rectangle) {

        double area = lengths[0] * lengths[1];

        if (!isfinite(area)) {
            result->isRectangle = 0;
            result->perimeter = 0;
            return 0;
        }

        result->area = area;
    }

    return 1;
}

// Runs the four-point feature
int runFourPointFeature(void) {

    Point points[4];
    FourPointResult result;

    printf("\n--- Four Point Rectangle Checker ---\n");
    printf("Enter four points in order around the shape.\n");

    while (1) {

        for (int i = 0; i < 4; i++) {

            printf("\nPoint %d:\n", i + 1);

            if (!readNumber("Enter X: ", &points[i].x)) {
                return 0;
            }

            if (!readNumber("Enter Y: ", &points[i].y)) {
                return 0;
            }
        }

        if (analyzeFourPoints(points, &result)) {
            break;
        }

        printf("\nInvalid points. Please try again.\n");
    }

    printf("\n--- Results ---\n");
    printf("Perimeter: %.2f\n", result.perimeter);

    if (result.isRectangle) {

        printf("The points form a rectangle.\n");
        printf("Area: %.2f\n", result.area);

    } else {

        printf("The points do not form a rectangle.\n");
    }

    return 1;
}