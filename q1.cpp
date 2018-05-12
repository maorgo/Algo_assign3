#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

class vertex {
	// 0 - white, 1 - grey, (-1) - black
	int value, color;
public:
	vertex(int val) { 
		color = 0; 
		value = val;
	}

	int getValue() { return value; }

	int getColor() { return color; }

	void setColor(int c) { color = c; }
};

list<int> ** parseNeighbors(string vertices_string, int vertices) {
	int endID, startID, vertexIndex = 0;
	list<int> **neighborsList = new list<int>* [vertices];
	for (int i = 0; i < vertices; i++)
		neighborsList[i] = new list<int>;
	string neighbors;
	
	while ((endID = vertices_string.find('}')) != std::string::npos) {
		list<int> listOfInts;
		startID = vertices_string.find('{');
		//cout << "Start id: " << startID << ", End id: " << endID << endl;
		neighbors = vertices_string.substr(startID+1, endID-1 - startID);
		//cout << neighbors << endl;

		// Iterate each number in the string and create a node for each number
		for (int i = startID + 1; i < endID; i++) 
			if (vertices_string[i] != ',' && vertices_string[i] != ' ') 
				neighborsList[vertexIndex][0].push_back(vertices_string[i] - 48);

		vertexIndex++;
			
		vertices_string = vertices_string.substr(endID + 1, vertices_string.length());
		
	}
	return neighborsList;
	
}

int * getShortestPathsCounter(list<int> ** neighbors, int numVertices, int startVertex) {
	// todo: Remember to add 1 to indices. Now vertices start at index 0 instead of 1.
	int *minimalTrack = new int[numVertices], *trackCounter = new int[numVertices], currentVertex;
	vertex **vertices = new vertex *[numVertices], *currentNode; 
	// Create the vertices, Init the minimalTrack, trackCounter arrays
	for (int i = 0; i < numVertices; i++) {
		vertices[i] = new vertex(i);
		minimalTrack[i] = NULL;
		trackCounter[i] = 0;
		
	}

	queue<int> neighbors_queue;

	// Set the path size from start vertex to the start vertex as 0.
	startVertex--;
	minimalTrack[startVertex] = 0;
	trackCounter[startVertex] = 1; 
	vertices[startVertex]->setColor(-1);

	// Iterate over the start vertex neighbors
	for (list<int>::iterator it = neighbors[startVertex]->begin(); it != neighbors[startVertex]->end(); it++) {
		// Insert all start's vertex neighbors to the queue
		neighbors_queue.push(*it);
		vertices[*it-1]->setColor(1);

		// Update their value in the arrays
		minimalTrack[*it-1] = 1;
		trackCounter[*it-1] = 1;
	}

	// Iterate over all the neighbors, until the queue is empty (BFS)

	while (!neighbors_queue.empty()) {
		// Get next vertex
		currentNode = vertices[neighbors_queue.front() - 1];
		if (neighbors_queue.front() == numVertices)
			return trackCounter;
		// If its already visited, move to the next vertex in the queue
		if (currentNode->getColor() == -1) {
			neighbors_queue.pop();
			continue;
		}

		// This vertex needs to be processed
		currentVertex = currentNode->getValue();

		// Go through its neighbors
		for (list<int>::iterator it = neighbors[currentVertex]->begin(); it != neighbors[currentVertex]->end(); it++) {
			if (!*it)
				continue;
			if (minimalTrack[currentVertex] + 1 < minimalTrack[*it - 1] || !minimalTrack[*it - 1]) {
				minimalTrack[*it - 1] = minimalTrack[currentVertex] + 1;
				if (trackCounter[currentVertex] > 1)
					trackCounter[*it - 1] += trackCounter[currentVertex];
				else
					trackCounter[*it - 1] = 1;
			}
			else if (minimalTrack[*it - 1] == minimalTrack[currentVertex] + 1) {
				if (trackCounter[currentVertex] == 1)
					trackCounter[*it - 1] ++;
				else
					trackCounter[*it - 1] += trackCounter[currentVertex];
			}

			if (vertices[*it - 1]->getColor() == 0) {
				vertices[*it - 1]->setColor(1);
				neighbors_queue.push(*it);
			}

		}
		vertices[currentVertex]->setColor(-1);
		neighbors_queue.pop();

	}
	return trackCounter;
}

void main() {
	string fileName, line;
	int sourceNode, *distanceResults, numOfNodes, *listOfNeighbors;
	char isContinue = 'y';
	cout << "Please enter a name of an input file :" << endl;
	getline(cin, fileName);

	// todo: Handle file here, if null, exit
	ifstream myfile;
	myfile.open(fileName.c_str());

	if (!myfile.is_open()) {
		cout << "Could not open input file." << endl;
		exit(-1);
	}
	// get the number of vertices
	getline(myfile, line);
	numOfNodes = stoi(line);

	// Parse the list
	getline(myfile, line);
	list<int> **neighborsList = parseNeighbors(line, numOfNodes);

	while (isContinue == 'y') {
	
		cout << "Please enter the source node : " << fileName << endl;
		cin >> sourceNode;
		// Run the algorithm
		distanceResults = getShortestPathsCounter(neighborsList, numOfNodes, sourceNode);
		cout << "The output for v2 is:" << endl;
		for (int i = 0; i < numOfNodes; i++) {
			cout << i + 1 << ": " << distanceResults[i] << ", ";
		}
		cout << endl;
		cout << "Do you want to continue (y / n) :" << endl;
		cin >> isContinue;
	}

	// Close file handler
	myfile.close();
}
