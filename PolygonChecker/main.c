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
			float result[4];
			analyzeTriangle(result);
			printf("\n");
			if (result[0] == 1) {
				printf("This is not a triangle.\n");
			}
			else {
				printf("This is an ");
				if (result[0] == 2) {
					printf("equilateral triangle. ");
				}
				else if (result[0] == 3) {
					printf("isosceles triangle. ");
				}
				else if (result[0] == 4) {
					printf("scalene triangle. ");
				}
				printf("\nThe angles of the triangle are: \n1: %.2f \n2: %.2f \n3: %.2f\n", result[1], result[2], result[3]);
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