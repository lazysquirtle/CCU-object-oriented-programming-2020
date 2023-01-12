#include<iostream>
#include <fstream>
#include <queue>
#include "Node.h"

void Node::inputID(int inputid) {
	id = inputid;
	tag = 0;
}
void Node::inputNeighbor(int edgecount, std::vector<std::vector<int>> & inputStream) {
		for (int i = 0; i < edgecount; i++) {
				int linkID = inputStream[i + 1][0];										//inputStream�Ĥ@��O��LINK�s��
				int FirstNode = inputStream[i + 1][1];								//inputStream�ĤG��O��Ĥ@�Ӹ`�I
				int SecondNode = inputStream[i + 1][2];							//inputStream�ĤT��O��ĤG�Ӹ`�I
				if (id == FirstNode) {
						neighbor.push_back(SecondNode);
				}
				else if (id == SecondNode) {
						neighbor.push_back(FirstNode);
				}
		}
}

void Node::showNeighborhood() {
	std::cout << "Tag" << std::endl;
	std::cout << tag << std::endl;
	std::cout << "Link" << "ID" << std::endl;

	for (int i = 0; i < neighbor.size(); i++) {
		std::cout << neighbor[i] << std::endl;
	}
}

void Node::showRoutingTable() {
	std::cout << "dest" << "next" << std::endl;
	for (int i = 0; i < routingTable.size(); i++) {
		std::cout << routingTable[i][0] << routingTable[i][1] << std::endl;
	}
}

void Node::inputRoutingTable(Node *node, int nodecount) {
	int srcID = this->id;																//srcID�|�O�{�b�Ƕi�Ӫ��`�Iid
	for (int dest = 0; dest < nodecount; dest++) {
		if (dest != srcID) {																	//�ت��a�����ۤvid����
			std::vector<int> routing(2, -1);
			int present = dest;
			while (present != srcID || present == -1) {
					int minID = -1;															//�n��̤p��ID
					for (int i = 0; i < node[present].neighbor.size(); i++) {
							const Node &neighbor = node[node[present].neighbor[i]];
					
							if (neighbor.tag == node[present].tag - 1) {
								if (minID == -1) { 
									minID = neighbor.id;
								}
								else if (minID > neighbor.id) {
									minID = neighbor.id;
								}
							}
					}

					if (minID == srcID) 
						break;
					present = minID;
			 }
			routing[0] = dest;												//RoutingTable�Ĥ@���ت��a
			routing[1] = present;											//RoutingTable�ĤG���U�ӫe�����`�I
			this->routingTable.push_back(routing);
		}

	}
}

int Node::sendTo(int destID) {
	if (destID > id) {
		return routingTable[destID - 1][1];
	}
	else if (destID < id) {
		return routingTable[destID][1];
	}
	else {
		return id;
	}
}

void BFS(Node *node, int start, int nodecount) {
	std::vector<bool> visited(nodecount, false);
	std::queue<Node> nodeQueue;
	node[start].tag = 0;
	nodeQueue.push(node[start]);
	while (!nodeQueue.empty()) {

		Node &currNode = nodeQueue.front();
		int size = (currNode.neighbor).size();
		visited[currNode.id] = true;
		for (int i = 0; i < size; i++) {
			Node &neighborr = node[currNode.neighbor[i]];
			if (!visited[neighborr.id]) {
				neighborr.tag = currNode.tag + 1;
				visited[neighborr.id] = true;
				nodeQueue.push(neighborr);
			}
		}
		nodeQueue.pop();
	}
}