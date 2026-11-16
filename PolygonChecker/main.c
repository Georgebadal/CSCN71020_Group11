#include <stdio.h>
#include <stdbool.h>

#include "main.h"
#include "triangleSolver.h"
#include "four_points.h"


int side = 0;

int main() {
	bool continueProgram = true;
	while (continueProgram) {
		printWelcome();

		int shapeChoice = printShapeMenu();

		switch (shapeChoice) {
		case 1:
			printf("Triangle selected.\n");
			int result[4];
			analyzeTriangle(result);

			if (result[0] == 1) {
				printf("Not a triangle.\n");
			}
			else {
				if (result[0] == 2) {
					printf("Equilateral triangle.\n");
				}
				else if (result[0] == 3) {
					printf("Isosceles triangle.\n");
				}
				else if (result[0] == 4) {
					printf("Scalene triangle.\n");
				}
				printf("The angles of the triangle are: %d, %d, %d\n", result[1], result[2], result[3]);
			}
			break;
		case 2:
  			printf("Quadrilateral selected.\n");
   		    runFourPointFeature();
   	    	break;
		case 0:
			continueProgram = false;
			break;
		default:
			printf("Invalid value entered.\n");
			break;
		}
	}
	return 0;
}

void printWelcome() {
	printf("\n");
	printf(" **********************\n");
	printf("**     Welcome to     **\n");
	printf("**   Polygon Checker  **\n");
	printf(" **********************\n");
}

int printShapeMenu() {
	printf("1. Triangle\n");
	printf("2. Quadrilateral\n");
	printf("0. Exit\n");

	int shapeChoice;

	printf("Enter number: ");
	scanf("%1o", &shapeChoice);

	return shapeChoice;
}

// int* getTriangleSides(int* triangleSides) {
// 	printf("Enter the three sides of the triangle: ");
// 	for (int i = 0; i < 3; i++) {
// 		scanf("%d", &triangleSides[i]);
// 	}
// 	return triangleSides;
// }

// int* getFourPoints(int* fourPoints) {
// 	printf("Enter the four points (x1 y1 x2 y2): ");
// 	for (int i = 0; i < 4; i++) {
// 		scanf("%d", &fourPoints[i]);
// 	}
// 	return fourPoints;
//}