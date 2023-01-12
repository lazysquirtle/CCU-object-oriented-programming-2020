#include<iostream>
#include <fstream>
#include <queue>
#include "Node.h"

void MISNode::compareNodeID( MISNode* arg) {
	int selfid = id;
	int min = selfid;
	std::vector<int >Neighbor = this ->neighbor ;
	std::vector<int >Neighbor2 ;

	for (int i = 0 ; i < Neighbor.size(); i++) {
		if (arg[Neighbor[i]].getstatus() != 0)
			Neighbor2.push_back(Neighbor[i]);
	}

	for (int i = 0 ; i < Neighbor2.size(); i++) {
		if (min > Neighbor2[i])
			min = Neighbor2[i];
		else
			continue;
	}
	if (min == selfid)
		MIS = true;
}

void MISNode::modifystatus() {
	this->status = 0 ;
}

int MISNode::getid() {
	return this->id;
}

bool MISNode::getMIS() {
	return this->MIS;
}

bool MISNode::getvisit() {
	return this->visit;
}

int MISNode:: getstatus()  {
	return this->status;
}

int MISNode::getproxy() {
	return this->proxy;
}
std::vector<int> MISNode::getneighbor() {
	return this->neighbor;
}

void MISNode::setvisited() {
	this->visit = true;
}

void MISNode::setproxy() {
	this->proxy = this->id;
}

void BFS(MISNode *node, int start, int nodecount) {
	std::vector<bool> visited(nodecount, false);
	std::queue<MISNode> nodeQueue;
	node[start].tag = 0;
	nodeQueue.push(node[start]);
	while (!nodeQueue.empty()) {

		MISNode &currNode = nodeQueue.front();
		int size = (currNode.neighbor).size();
		visited[currNode.id] = true;
		for (int i = 0; i < size; i++) {
			MISNode &neighborr = node[currNode.neighbor[i]];
			if (!visited[neighborr.id]) {
				neighborr.tag = currNode.tag + 1;
				visited[neighborr.id] = true;
				nodeQueue.push(neighborr);
			}
		}
		nodeQueue.pop();
	}
}

void MISNode::inputRoutingTable(MISNode *node, int nodecount) {
	int srcID = this->id;																//srcID會是現在傳進來的節點id
	
	if (node[srcID].visit == false) {
		int proxy;
		std::vector <int >VecNeighbor = node[srcID].getneighbor();

		for (int i = 0; i < VecNeighbor.size(); i++) {
			if (node[VecNeighbor[i]].visit == true) {
				proxy = VecNeighbor[i];
				break;
			}
		}

		for (int dest = 0; dest < nodecount; dest++) {
			std::vector<int> routing(2, -1);
			routing[0] = dest;												//RoutingTable第一欄放目的地
			routing[1] = proxy;												//RoutingTable第二欄放proxy
			this->routingTable.push_back(routing);
		}

		node[srcID].proxy = proxy;
	}

    else {
		for (int dest = 0; dest < nodecount; dest++) {
			if (dest != srcID) {													//目的地不為自己id的話，且dest是visited
				std::vector<int> routing(2, -1);
				int present;
				
				if (node[dest].getvisit() == true)					
					present = dest;
				else
					present = node[dest].proxy;
				
				while (present != srcID || present == -1) {
					int minID = -1;																												//要找最小的ID
					for (int i = 0; i < node[present].neighbor.size(); i++) {
						const MISNode &neighbor = node[node[present].neighbor[i]];

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

				routing[0] = dest;												//RoutingTable第一欄放目的地
				routing[1] = present;											//RoutingTable第二欄放下個前往的節點
				this->routingTable.push_back(routing);
			}
		}
	}

	
}


void MISNode::AODV(MISNode* node, int nodecount,std::vector<int >ConnDS) {
	int SourceID = this->id;
	for (int DestIndex = 0; DestIndex < ConnDS.size(); DestIndex++) {
		if (ConnDS[DestIndex] != SourceID) {
			int present = ConnDS[DestIndex];
			while (present != SourceID || present == -1){
				int minID = -1;
				//if (node[present].tag > 3)
					//break;

				for (int i = 0; i < node[present].neighbor.size(); i++) {
					const MISNode &neighbor = node[node[present].neighbor[i]];

					if (neighbor.tag == node[present].tag - 1) {
						if (minID == -1) {																	//這代表第一次找
							minID = neighbor.id;
						}
						else if (minID > neighbor.id) {											//這代表找到比第一次找的還小的node
							minID = neighbor.id;
						}
					}
				}

				if (minID == SourceID)
					break;
				present = minID;
				if (node[present].visit == false  )
					node[present].setvisited() ;

			}
		}
	}
}