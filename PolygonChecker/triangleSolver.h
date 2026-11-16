#ifndef TRIANGLESOLVER_H
#define TRIANGLESOLVER_H

#include <stdbool.h>

bool validateTriangleSide(int side);
void clearInputBuffer(void);
int getTriangleSideInput(int sideNumber);
void getAngleInput(int side1, int side2, int side3, int angles[3]);
void analyzeTriangle(int result[4]);

#endif