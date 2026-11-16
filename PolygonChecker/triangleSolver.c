#include <stdio.h>
#include <stdbool.h>
#include "triangleSolver.h"

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

char* analyzeTriangle() {
	char* result = "";
	int sides[3] = { getTriangleSideInput(1), getTriangleSideInput(2), getTriangleSideInput(3) };
	
	if (sides[0] <= 0 || sides[1] <= 0 || sides[2] <= 0) {
		result = "Not a triangle";
	}
	else if (sides[0] == sides[1] && sides[0] == sides[2]) {
		result = "Equilateral triangle";
	}
	else if ((sides[0] == sides[1] && sides[0] != sides[2]) || 
		(sides[0] == sides[2] && sides[0] != sides[1]))
	{
		result = "Isosceles triangle";
	}
	else {
		result = "Scalene triangle";
	}

	return result;
}