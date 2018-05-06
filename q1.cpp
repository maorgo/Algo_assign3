#include <list>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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

int * getShortestPathsCounter(list<int> ** neighbors, int vertices, int startVertex) {
	int *minimalTrack = new int[vertices];
	int *trackCounter = new int[vertices];
	// Init the minimalTrack, trackCounter arrays
	for (int i = 0; i < vertices; i++) {
		minimalTrack[i] = -1;
		trackCounter[i] = 0;
	}
	startVertex--;
	// Set the path size from start vertex to the start vertex as 0.
	minimalTrack[startVertex] = 0;
	trackCounter[startVertex] = 1;

	for (int i = startVertex; i < vertices; i++) {
		// Iterate the list of neighbors for vertex i
		for (list<int>::iterator iter = neighbors[i]->begin(); iter != neighbors[i]->end(); iter++) {
			// If its the first time we arrive at the vertex, mark it as 1 with minimal distance of its father + 1
			if (minimalTrack[*iter-1] == -1 && minimalTrack[i] != -1) {
				minimalTrack[*iter - 1] = minimalTrack[i] + 1;
				if (trackCounter[i] > 1)
					trackCounter[*iter - 1] += trackCounter[i];
				else
					trackCounter[*iter - 1] = 1;
			}
			else if (minimalTrack[*iter - 1] != -1 && minimalTrack[*iter - 1] == minimalTrack[i] + 1) {
				if (trackCounter[i] < 2)
					trackCounter[*iter - 1] += 1;
				else
					trackCounter[*iter - 1] += trackCounter[i];
			}
		}
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