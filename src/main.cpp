#include "Matrix.h"
#include <iostream>

using namespace std;
using namespace Homework;

int main() {
	const size_t matrixSize = 10;
	Matrix<int, -1> matrix;
	for (size_t i = 0; i < matrixSize; ++i) {
		matrix[i][i] = i;
		size_t secondaryDiagonalIndex = matrixSize - i - 1;
		matrix[i][secondaryDiagonalIndex] = secondaryDiagonalIndex;
	}

	cout << "Elements in the range[1..8][1..8]:" << endl;
	for (size_t i = 1; i < matrixSize - 1; ++i) {
		for (size_t j = 1; j < matrixSize - 1; ++j) {
			cout << matrix[i][j] << ' ';
		}
		cout << endl;
	}

	cout << endl;

	cout << "A size of the matrix:" << endl;
	cout << matrix.size() << endl;
	cout << endl;

	cout << "All values with their position:" << endl;
	for (auto& c : matrix) {
		size_t x;
		size_t y;
		int v;
		std::tie(x, y, v) = c;
		std::cout << x << y << v << std::endl;
	}

	cout << endl;

	cout << "A canonical form of '=' operator:" << endl;
	((matrix[100][100] = 314) = 0) = 217;
	cout << matrix[100][100] << endl;
	cout << endl;

	cout << "5D matrix" << endl;
	Matrix<int, -1, 5> matrix5d;
	matrix5d[100][100][100][100][100] = 100;
	cout << matrix5d[100][100][100][100][100] << endl;

	return 0;
}
