#include <bits/stdc++.h>
using namespace std;

int main () {

	#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	freopen("error.txt", "w", stderr);
	#endif 

	int numSkewers, windowSize;
	cin >> numSkewers >> windowSize;

	// corner case:
	if (windowSize >= numSkewers) {
		cout << "1\n1" << endl;
		return 0;
	}

	int coveredOneSpan = 2*windowSize + 1;
	int remaining = numSkewers % coveredOneSpan;

	int numActions, firstSkewer, lastSkewer;
	if (remaining == 0) {
		numActions = numSkewers/coveredOneSpan;
		firstSkewer = windowSize + 1; // after a gap of k skewers in beginning
		int skewerPos = firstSkewer;

		cout << numActions << endl;
		while (skewerPos <= numSkewers) {
			cout << skewerPos << " ";
			skewerPos += coveredOneSpan;
		}
	}
	else {
		numActions = numSkewers/coveredOneSpan + 1;
		
		cout << numActions << endl;

		if (remaining <= windowSize) {
			int leftAndRight = coveredOneSpan + remaining;
			if (leftAndRight % 2 == 0) {
				firstSkewer = leftAndRight/2 - windowSize;
			}
			else {
				firstSkewer = 1 + leftAndRight/2 - windowSize;
			}
			lastSkewer = numSkewers - (leftAndRight/2 - windowSize) + 1;
			cout << firstSkewer << " ";
			int skewerPos = firstSkewer + coveredOneSpan;
			while (skewerPos < lastSkewer) {
				cout << skewerPos << " ";
				skewerPos += coveredOneSpan;
			}
			cout << lastSkewer << " ";
		}
		else {
			cout << remaining - windowSize << " ";
			int skewerPos = remaining - windowSize + coveredOneSpan;
			while (skewerPos <= numSkewers) {
				cout << skewerPos << " ";
				skewerPos += coveredOneSpan;
			}
		}
		cout << endl;
	}

	return 0;
}