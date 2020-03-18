#include <iostream>
#include <set>
#include <queue>
#include <stack>
using namespace std;

const int N = 6;

int up(int X) {
	return (X - N) < 0 ? -1 : (X - N);
}
int down(int X) {
	return (X + N) > N*N - 1 ? -1 : (X + N);
}
int left(int X) {
	return (X % N) == 0 ? -1 : (X - 1);
}
int right(int X) {
	return ((X + 1) % N) == 0 ? -1 : (X + 1);
}
// all cells in the matrix are encoded according X = row*N + column
int column(int X) {
	return X % N;
}

int row(int X) {
	return X / N;
}
bool member(int X, set<int>& s) {
	if (s.find(X) != s.end())
		return true;
	return false;
}
int value(int X, int matrix[N][N]) {
	int rowX = row(X), colX = column(X);
	return matrix[rowX][colX];
}
bool validNeighbour(int x, set<int>& s, int matrix[N][N]) {
	//ne e izvun duskata, ne e poseten i ne e neprohodim
	return (x > -1) && !member(x, s) && value(x, matrix) != 0;
}

int findAnotherTeleport(int X, int matrix[N][N]) {
	for(int i=0; i<N; i++)
	 for (int j = 0; j < N; j++)
		{
		 int curr = i*N + j;
		 if (matrix[i][j] == 2 && curr != X)
			 return curr;
		}
	return -1;
}

int restorePath(int end, int* parent) {
	stack<int> st;
	int next = end;
	while (next != -1) {
		st.push(next);
		next = parent[next];
	}
	//putqt e zapisan v st
	cout << "Path: ";
	int counter = 0;
	while (!st.empty()) {
		int elem = st.top();
		st.pop();
		counter++;
		cout << elem << " ";
	}

	return counter;
}

int bfs(int matrix[N][N], int start, int end, int* parent) {
	if (value(start,matrix) == 0 || value(end,matrix) == 0 || value(start,matrix) == 2 || value(end,matrix) == 2)
		return -1;

	queue<int> q; set<int> visited;
	q.push(start);
	visited.insert(start);
	parent[start] = -1;

	while (!q.empty()) {
		int X = q.front();
		q.pop();

		if (X == end) {
			return restorePath(end, parent);
		}

		if (value(X,matrix) == 2 && !member(findAnotherTeleport(X,matrix), visited)) {
			int anotherTeleport = findAnotherTeleport(X, matrix);
			q.push(anotherTeleport);
			visited.insert(anotherTeleport);
			parent[anotherTeleport] = X;
		}
		else {
			//push all unvisited neighbours in the queue
			int upNeighbour = up(X);
			int rightNeighbour = right(X);
			int downNeighbour = down(X);
			int leftNeighbour = left(X);

			if (validNeighbour(upNeighbour, visited, matrix)) { q.push(upNeighbour); visited.insert(upNeighbour); parent[upNeighbour] = X; }
			if (validNeighbour(rightNeighbour, visited, matrix)) { q.push(rightNeighbour); visited.insert(rightNeighbour); parent[rightNeighbour] = X; }
			if (validNeighbour(downNeighbour, visited, matrix)) { q.push(downNeighbour); visited.insert(downNeighbour); parent[downNeighbour] = X; }
			if (validNeighbour(leftNeighbour, visited, matrix)) { q.push(leftNeighbour); visited.insert(leftNeighbour); parent[leftNeighbour] = X; }
		}
	}

	return -1;
}

void printMatrix(int matrix[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << matrix[i][j] << " ";
		cout << "\n";
	}
}
int main()
{   
	int matrix[N][N] = { {1,1,0,1,1,1},
						{1,2,0,0,1,1},
						{1,1,1,1,2,1},
						{1,1,1,1,1,1},
						{1,0,0,1,1,1},
						{1,1,1,1,1,1} };

	int parent[N*N];

	printMatrix(matrix);

	int minLength = bfs(matrix, 0, 28, parent);
	cout << "Length of shortest path: " << minLength << "\n";

	return 0;
}
