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
static int readNumber(const char *prompt, double *value, int num) {
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

        if (input[0] == '\n' && num == 0) {
            printf("Enter X: ");
            continue;
        }
        if (input[0] == '\n') {
            printf("Enter a valid number. \n");
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
// Returns 1 if successful, 0 if an error occurs (e.g., invalid points)
int analyzeFourPoints(const Point points[4], FourPointResult *result) {

    if (points == NULL || result == NULL) {
        return 0;
    }

    result->isRectangle = 0;
    result->perimeter = 0;
    result->area = 0;

    Point ordered[4];
    double centerX = 0;
    double centerY = 0;

    // Validate the points and check for duplicates
    for (int i = 0; i < 4; i++) {

        if (!isfinite(points[i].x) || !isfinite(points[i].y)) {
            return 0;
        }

        for (int j = 0; j < i; j++) {
            if (points[i].x == points[j].x &&
                points[i].y == points[j].y) {
                return 0;
            }
        }

        ordered[i] = points[i];

        centerX += points[i].x / 4.0;
        centerY += points[i].y / 4.0;
    }

    // Arrange the four points around the center
    // This allows the points to be entered in any order
    for (int i = 0; i < 3; i++) {

        for (int j = i + 1; j < 4; j++) {

            double angleI = atan2(
                ordered[i].y - centerY,
                ordered[i].x - centerX
            );

            double angleJ = atan2(
                ordered[j].y - centerY,
                ordered[j].x - centerX
            );

            if (angleI > angleJ) {
                Point temp = ordered[i];
                ordered[i] = ordered[j];
                ordered[j] = temp;
            }
        }
    }

    double dx[4], dy[4], lengths[4];
    double perimeter = 0;

    // Connect the four points and calculate side lengths
    for (int i = 0; i < 4; i++) {

        int next = (i + 1) % 4;

        dx[i] = ordered[next].x - ordered[i].x;
        dy[i] = ordered[next].y - ordered[i].y;

        if (!isfinite(dx[i]) || !isfinite(dy[i])) {
            return 0;
        }

        lengths[i] = hypot(dx[i], dy[i]);

        if (!isfinite(lengths[i]) || lengths[i] <= 0) {
            return 0;
        }

        perimeter += lengths[i];

        if (!isfinite(perimeter)) {
            return 0;
        }
    }

    // Check if all four corners have 90 degree angles
    int rectangle = 1;

    for (int i = 0; i < 4; i++) {

        int next = (i + 1) % 4;

        double dot =
            (dx[i] / lengths[i]) * (dx[next] / lengths[next]) +
            (dy[i] / lengths[i]) * (dy[next] / lengths[next]);

        if (fabs(dot) > TOLERANCE) {
            rectangle = 0;
            break;
        }
    }

    result->perimeter = perimeter;
    result->isRectangle = rectangle;

    // Calculate area if the shape is a rectangle
    if (rectangle) {

        result->area = lengths[0] * lengths[1];

        if (!isfinite(result->area)) {
            result->isRectangle = 0;
            result->perimeter = 0;
            result->area = 0;
            return 0;
        }
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
            if (i == 0) {
                if (!readNumber("", &points[i].x, i)) {
                return 0;
                }
        

                if (!readNumber("Enter Y: ", &points[i].y, i)) {
                    return 0;
                }
            }
            else {
                if (!readNumber("Enter X: ", &points[i].x, i)) {
                    return 0;
                }
        

                if (!readNumber("Enter Y: ", &points[i].y, i)) {
                    return 0;
                }
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