#include <bits/stdc++.h>
using namespace std;

// returns matrix sum
// sum[i][j] denotes the sum of matrix values from (0, 0) to (i, j)
vector <vector <int> > getSum(vector <vector <int> > matrix) {

	int rows = matrix.size();
	int cols;
	if (rows != 0) {
		cols = matrix[0].size();
	}
	else {
		cols = 0;
	}

	// intialize sum matrix with size and all values as zero
	vector <vector <int> > sum(rows, vector <int> (cols, 0));
	for (int row = 0; row < rows; row ++) {
		for (int col = 0; col < cols; col ++) {
			// intitialze current cell's value as the sum
			sum[row][col] = matrix[row][col];

			if (col > 0) sum[row][col] += sum[row][col - 1]; // sum to the left
			if (row > 0) sum[row][col] += sum[row - 1][col]; // sum at the top
			if (row > 0 && col > 0) sum[row][col] -= sum[row - 1][col - 1]; // common sum
		}
	}

	return sum;
}

int main () {

	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	freopen("error.txt", "w", stderr);
	#endif 

	int t;
	cin >> t;

	for (int t_no = 0; t_no < t; t_no ++) {

		int rows, cols, priceLimit;
		cin >> rows >> cols >> priceLimit;

		vector <vector <int> > price(rows, vector <int> (cols));

		for (int row = 0; row < rows; row ++) {
			for (int col = 0; col < cols; col ++) {
				cin >> price[row][col];
			}
		}

		// get the sum matrix (dp matrix), sum[i][j] denotes the sum from (0, 0) to (i, j)
		vector <vector <int> > sum = getSum(price);

		// print the sum matrix to error console, add a forward slash in beginning of next line to uncomment
		/*/
		for (int row = 0; row < rows; row ++) {
			for (int col = 0; col < cols; col ++) {
				cerr << sum[row][col] << " ";
			}
			cerr << endl;
		}
		//*/

		int maxArea = 0, minPrice = INT_MAX;
		// consider all possible rectangles from (i, j) to (k, l)

		int topLeftX, topLeftY, bottomRightX, bottomRightY;
		for (int i = 0; i < rows; i ++) {
			for (int j = 0; j < cols; j ++) {
				for (int k = i; k < rows; k ++) {
					for (int l = j; l < cols; l ++) {

						// find price of this rectangle
						int priceRectangle = sum[k][l];
						if (j > 0) priceRectangle -= sum[k][j - 1];
						if (i > 0) priceRectangle -= sum[i - 1][l];
						if (i > 0 && j > 0) priceRectangle += sum[i - 1][j - 1];

						// update results maxArea and minPrice
						if (priceRectangle <= priceLimit) {
							// find area of this rectangle
							int area = (k - i + 1)*(l - j + 1);
							if (area > maxArea) {
								maxArea = area;
								minPrice = priceRectangle;
								topLeftX = i;
								topLeftY = j;
								bottomRightX = k;
								bottomRightY = l;
							}
							else if (area == maxArea && priceRectangle < minPrice) {
								minPrice = priceRectangle;
								topLeftX = i;
								topLeftY = j;
								bottomRightX = k;
								bottomRightY = l;
							}
						}
					}
				}
			}
		} // O(N^4) solution

		if (maxArea == 0) {
			minPrice = 0;
		}
		// cerr << topLeftX << " " << topLeftY << " " << bottomRightX << " " << bottomRightY << endl;
		cout << "Case #" << t_no + 1 << ": " << maxArea << " " << minPrice << endl;

	}

	return 0;
}