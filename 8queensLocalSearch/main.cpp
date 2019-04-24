#include <iostream>
#include <vector>
#include <ctime>
#include <random>

using namespace std;

void fast_search(int);
vector<int> generate_permutation(vector<int>);
bool is_attacked_pos(int, int, int*);
bool is_attacked_neg(int, int, int*);
int check_for_collisions(int, int**);
int** set_up_board(int, int*, int**);
void print_board(int, int**);
void print_queen(int, int*);
void copy_queen(int, int*, int*);
void swap(int&, int&);

int main() {
	int a, n;
	cout << "n: ";
	cin >> n;
	fast_search(n);
	cin >> a;
	return 0;
}


void fast_search(int n) {
	vector<int> pi;
	int swaps_performed;
	int *queen = new int[n];
	int collisions = 1, collisions_after = 1;
	int *swap_queen = new int[n];
	int** board = new int*[n];
	for (int i = 1; i <= n; ++i)
		board[i] = new int[n];

	int** swap_board = new int*[n];
	for (int i = 1; i <= n; ++i)
		swap_board[i] = new int[n];

	// set values
	for (int i = 1; i <= n; ++i) pi.push_back(i);

	// Sosic's and Gu's algorithm
	do {

		cout << "New permutation:\n";
		pi = generate_permutation(pi);

		for (int i = 0; i < pi.size(); ++i) {
			queen[i + 1] = pi.at(i);
		}

		print_queen(n, queen);

		board = set_up_board(n, queen, board);

		print_board(n, board);

		do {
			swaps_performed = 0;

			for (int i = 1; i <= n; ++i) {
				for (int j = i + 1; j <= n; ++j) {
					if (is_attacked_pos(n, i + queen[i], queen) || is_attacked_neg(n, i - queen[i], queen) ||
						is_attacked_pos(n, j + queen[j], queen) || is_attacked_neg(n, j - queen[j], queen)) {//if queen[i] or queen[j] are attacked

						collisions = check_for_collisions(n, board); // check for collisions before swap

						copy_queen(n, queen, swap_queen); // make copy

						swap(swap_queen[i], swap_queen[j]); // swap copy

						swap_board = set_up_board(n, swap_queen, swap_board); // swap board

						collisions_after = check_for_collisions(n, swap_board); // check for collisions after swap

						if (collisions_after < collisions) {

							cout << "The amount of collisions is reduced after the swap.\n";

							collisions = collisions_after;

							copy_queen(n, swap_queen, queen); // make the actual swap

							board = set_up_board(n, queen, board); // make the actual swap

							++swaps_performed; // count swaps

							print_queen(n, queen);

							print_board(n, board);
						}
						else if (collisions_after == collisions) {
							cout << "The amout of collisions after the swap remained the same.The actual swap is not performed.\n\n";
						}
						else {
							cout << "The amount of collisions grew after the swap. No changes are made.\n\n";
						}
					}
				}

			}
		} while (swaps_performed != 0);

	} while (collisions != 0);

	cout << "The Queens behave in well manners!";
}

// shuffles the numbers from 1 to n
vector<int> generate_permutation(vector<int> myvector) {
	srand(unsigned(std::time(0)));

	vector<vector<int>> tried_combinations;
	tried_combinations.push_back(myvector);

	while (find(tried_combinations.begin(), tried_combinations.end(), myvector) != tried_combinations.end())//while myvector is in tried_combinations -> already tried...
	{
		random_shuffle(myvector.begin(), myvector.end()); // updates myvector with the numbers in it shuffled...
	}

	return myvector;
}


// sets up the board according to the queen array
int** set_up_board(int n, int *queen, int **board) {


	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			if (j == queen[i]) {
				board[i][j] = 1; // means: queen
			}
			else {
				board[i][j] = 0; // means: no queen
			}
		}
	}
	return board;
}

int check_for_collisions(int n, int **board) {

	int d_lines = 2 * n; // number of positive/negative diagonal lines + 1 (index 0)
	int *d1 = new int[d_lines] {}; // positive diagonal line
	int *d2 = new int[d_lines] {}; // negative diagonal line

	// check for collisions in positive diagonal lines
	// starts from the upper left corner, moves down vertically
	for (int k = 1; k <= n; ++k) {
		int i = k;
		int j = 1;
		while (i >= 1) {
			if (board[i][j] == 1) {
				++d1[k]; // counts how many queens there are on a slide
			}
			--i;
			++j;
		}
	}
	// continues from the lower left corner, moves towards the end of the row
	for (int k = 2; k <= n; ++k) {
		int i = n;
		int j = k;
		while (j <= n) {
			if (board[i][j] == 1) {
				++d1[k + n - 1];
			}
			--i;
			++j;
		}
	}


	// check for collisions in negative diagonal lines
	// starts from the lower left corner, moves towards the end of the row
	for (int k = 1; k <= n; ++k) {
		int i = n;
		int j = k;
		while (i >= 1) {
			if (board[i][j] == 1) {
				++d2[k];
			}
			--i;
			--j;
		}
	}
	// continues from the lower right corner, moves up vertically
	for (int k = n - 1; k >= 1; --k) {
		int i = k;
		int j = n;
		while (i >= 1) {
			if (board[i][j] == 1) {
				++d2[2 * n - k];

			}
			--i;
			--j;

		}
	}

	int collisions = 0; // counter for collisions

	for (int i = 1; i < d_lines; ++i) {
		if (d1[i] > 1) {
			collisions += d1[i] - 1; // the number of queens (if there are more than 1) - 1 = number of collisions
		}
		if (d2[i] > 1) {
			collisions += d2[i] - 1;
		}
	}

	return collisions;
}

// checks if the queen is attacked on the positive diagonal line
bool is_attacked_pos(int n, int sum, int *queens) {
	int sum_counter = 0;
	for (int i = 1; i < n; ++i) {
		if (i + queens[i] == sum) { // the sum of the indexes is constant on any positive diagonal line
			sum_counter += 1; // counts how many queens' indexes produce the given constant
		}
	}
	if (sum_counter > 1) { // if there are more than one, it means they are attacking each other
		return true;
	}
	else {
		return false;
	}
}

// checks if queen is attacked on the negative diagonal line
bool is_attacked_neg(int n, int diff, int *queens) {
	int diff_counter = 0;
	for (int i = 1; i < n; ++i) {
		if (i - queens[i] == diff) { // the difference of the indexes is constant on any negative diagonal line
			diff_counter += 1; // counts the amount of queens on the diagonal
		}
	}
	if (diff_counter > 1) {
		return true;
	}
	else {
		return false;
	}
}

void print_board(int n, int **board) {
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			cout << board[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void print_queen(int n, int *array) {
	for (int i = 1; i <= n; ++i) {
		cout << array[i] << " ";
	}
	cout << "\n\n";
}

void copy_queen(int n, int *a, int *b) {
	for (int i = 1; i <= n; ++i) {
		b[i] = a[i];
	}
}

void swap(int &a, int &b) {
	int c;
	c = a;
	a = b;
	b = c;
}