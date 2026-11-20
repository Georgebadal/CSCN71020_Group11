#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "triangleSolver.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool validateTriangleSide(int side) {
    return side > 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getTriangleSideInput(int sideNumber) {
    int side;
    char next_char;

    while (true) {
        printf("Enter length of side %d: ", sideNumber);

        if (scanf("%d%c", &side, &next_char) == 2 && next_char == '\n') {
            if (validateTriangleSide(side)) {
                return side; 
            } else {
                printf("Invalid input. Side length must be a positive integer.\n");
            }
        } else {
            printf("Invalid input. Please enter a valid integer (no letters or symbols).\n");
            if (next_char != '\n') {
                clearInputBuffer();
            }
        }
    }
}

void getAngleInput(int side1, int side2, int side3, float angles[3]) {
    angles[0] = (acos((pow(side2, 2) + pow(side3, 2) - pow(side1, 2)) / (2.0 * side2 * side3)) * (180.0 / M_PI));
    angles[1] = (acos((pow(side1, 2) + pow(side3, 2) - pow(side2, 2)) / (2.0 * side1 * side3)) * (180.0 / M_PI));
    angles[2] = (acos((pow(side1, 2) + pow(side2, 2) - pow(side3, 2)) / (2.0 * side1 * side2)) * (180.0 / M_PI));
}

void analyzeTriangle(float result[4]) {
    int sides[3] = { getTriangleSideInput(1), getTriangleSideInput(2), getTriangleSideInput(3) };
    
    if (sides[0] <= 0 || sides[1] <= 0 || sides[2] <= 0 ||
        (sides[0] + sides[1] <= sides[2]) ||
        (sides[0] + sides[2] <= sides[1]) ||
        (sides[1] + sides[2] <= sides[0])) {
        result[0] = 1;
        result[1] = result[2] = result[3] = 0; 
        return;
    }
    else if (sides[0] == sides[1] && sides[1] == sides[2]) {
        result[0] = 2;
    }
    else if (sides[0] == sides[1] || sides[0] == sides[2] || sides[1] == sides[2]) {
        result[0] = 3;
    }
    else {
        result[0] = 4;
    }

    float angles[3];
    getAngleInput(sides[0], sides[1], sides[2], angles);
    for (int i = 0; i < 3; i++) {
        result[i + 1] = angles[i];
    }
}

