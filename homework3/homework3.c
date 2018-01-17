#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

#define MAX 100

struct Vertex {
	char label;
	bool visited;
};

//queue variables

int queue[MAX];
int rear = -1;
int front = 0;
int queueItemCount = 0;

//graph variables

//array of vertices
struct Vertex* lstVertices[MAX];

//adjacency matrix
int adjMatrix[MAX][MAX];

//all shortest paths
int allShortestPaths[MAX][MAX];

//path array
int path[MAX];
int pathCount = 0;
int allPathCount = 0; // all shortest path counts

//vertex count
int vertexCount = 0;

//queue functions
void insert(int data) {
	queue[++rear] = data;
	queueItemCount++;
}

int removeData() {
	queueItemCount--;
	return queue[front++];
}

bool isQueueEmpty() {
	return queueItemCount == 0;
}

//graph functions

//add vertex to the vertex list
void addVertex(char label) {
	struct Vertex* vertex = (struct Vertex*) malloc(sizeof(struct Vertex));
	vertex->label = label;
	vertex->visited = false;
	lstVertices[vertexCount++] = vertex;
}

//add edge to edge array
void addEdge(int start, int end) {
	adjMatrix[start][end] = 1;
	adjMatrix[end][start] = 1;
}

//display the vertex
void displayVertex(int vertexIndex) {
	printf("%c ", lstVertices[vertexIndex]->label);
}

//add to shortest path
void addToPath(int vetexIndex) {
	path[pathCount++] = vetexIndex;
}

void removeLastFromPath() {
	pathCount--;
}

//add to shortest paths array
void addToAllPaths(int vetexIndex) {
	int i;
	for (i = 0; i < pathCount; ++i) {
		allShortestPaths[allPathCount][path[i]] = 1;
//		printf(" %c" , lstVertices[path[i]]->label);
	}
//	printf(" %c\n" , lstVertices[vetexIndex]->label);
	allShortestPaths[allPathCount][vetexIndex] = 1;
	allPathCount++;
}

//get the adjacent unvisited vertex
int getAdjUnvisitedVertex(int vertexIndex) {
	int i;

	for (i = 0; i < vertexCount; i++) {
		if (adjMatrix[vertexIndex][i] == 1 && lstVertices[i]->visited == false)
			return i;
	}

	return -1;
}

void breadthFirstSearch(int from, int till) {
	int i;

	//mark first node as visited

	lstVertices[0]->visited = true;
	//display the vertex
//	displayVertex(0);

//add first vertex
//	addToPath(0);

	int unvisitedVertex;

	while (!isQueueEmpty()) {
		//get the unvisited vertex of vertex which is at front of the queue
		int tempVertex = removeData();
		addToPath(tempVertex);
		//no adjacent vertex found
		while ((unvisitedVertex = getAdjUnvisitedVertex(tempVertex)) != -1) {
			lstVertices[unvisitedVertex]->visited = true;
//			displayVertex(unvisitedVertex);
			addToAllPaths(unvisitedVertex);
			insert(unvisitedVertex);
		}
//		removeLastFromPath();
	}

	//queue is empty, search is complete, reset the visited flag
	for (i = 0; i < vertexCount; i++) {
		lstVertices[i]->visited = false;
	}
}

void calcBetweeness(int from, int till) {
	int i, j, tempBetweeness = 0;
	// for every vertax
	for (i = from; i < till; ++i) {
		for (j = 0; j < MAX; ++j) {
			if (allShortestPaths[j][i] == 1)
				tempBetweeness++;
		}
		printf("vertex %d betweeness = %d\n", i, tempBetweeness);
		tempBetweeness = 0;
	}
}

int main() {
	int i, j;
	srand(time(NULL));

	//generate the matrix
	for (i = 0; i < MAX; i++) // set adjacency {
			{
		for (j = 0; j < MAX; j++) // matrix to 0
			adjMatrix[i][j] = rand() % (1 + 1 - 0) + 0;
		;
	}
	for (i = 0; i < MAX; ++i) {
		addVertex((char) i);
	}
	//insert all vertex index in queue
	for (i = 0; i < MAX; ++i) {
		insert(i);
	}

	int  tid;
	/* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel num_threads(4)

	{
		tid = omp_get_thread_num();
//		printf("%d\n", tid);
		/* Obtain thread number */
		tid = omp_get_thread_num();
		int from = 25 * tid;
		int till = 25 * tid + 25;
		breadthFirstSearch(from, till);
#pragma omp barrier //all threads wait as they should		calcBetweeness(from, till);//		return 0;
	}}
