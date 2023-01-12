#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <queue>
#include "Node.h"

void Findpath(MISNode *node, int srcID, int destID);
void Outputpath(MISNode *node, std::vector<std::vector<int>> & inputStream);
std::vector<std::vector<int>> GetinputStream();

int main() {
	int NumOfNode = 0;
	int NumOfEdge = 0;

	std::vector<std::vector<int>> inputStream;							//輸入題目要求
	inputStream = GetinputStream();
	NumOfNode = inputStream[0][0];												//Node數量在[0][0]
	NumOfEdge = inputStream[0][1];												//Edge數量在[0][1]
	
																				/*
	Node *sensor = new Node[NumOfNode];
	for (int i = 0; i < NumOfNode; i++) {
		sensor[i].inputID(i);																		//存一下他們的ID
		sensor[i].inputNeighbor(NumOfEdge, inputStream);		//存一下他們的鄰居
	}
	*/

	MISNode *sensor = new MISNode[NumOfNode];
	for (int i = 0; i < NumOfNode; i++) {
		sensor[i].inputID(i);																		//存一下他們的ID
		sensor[i].inputNeighbor(NumOfEdge, inputStream);		//存一下他們的鄰居
	}

	bool flag = true;
	while (flag) {
		for (int index = 0; index < NumOfNode; index++) {
			sensor[index].compareNodeID(sensor);
			if (sensor[index].getMIS() == true) {
				std::vector<int> neigh = sensor[index].getneighbor();
				for (int j = 0; j < neigh.size(); j++)
					sensor[neigh[j]].modifystatus();
			}
		}
		flag = false;
		for (int index = 0; index < NumOfNode; index++)
			flag = flag | ((sensor[index].getMIS() == false) && (sensor[index].getstatus() == 1));
	}

	std::vector <int >CDS;
	std::vector <int >NotInCDS;
	for (int index = 0; index < NumOfNode; index++) {
		if (sensor[index].getMIS() == true)
			CDS.push_back(index);															//把MISnode(現在在CDS裡)存進vector
	}

	for (int i = 0; i < CDS.size(); i++) {
		BFS(sensor, CDS[i], NumOfNode);													//把CDS裡的node做BFS
		sensor[CDS[i]].AODV(sensor, NumOfNode, CDS);						//做AODV去連接兩個MIS節點
		sensor[CDS[i]].setvisited();																//設MISnode為visited
	}

	CDS.clear();																								//先清空CDS裡的東西
	for (int i = 0; i < NumOfNode; i++) {
		if (sensor[i].getvisit() == true)
			CDS.push_back(i);																			//把i放入CDS裡	
		else
			NotInCDS.push_back(i);
	}

	for (int i = 0; i < NotInCDS.size(); i++)
	{
		sensor[NotInCDS[i]].inputRoutingTable(sensor, NumOfNode);		//先建NotInCDS的RoutingTable(一定要先建)
		//sensor[NotInCDS[i]].showRoutingTable();
	}

	for (int i = 0 ; i < CDS.size() ; i++) {
		BFS(sensor, CDS[i], NumOfNode);													//把CDS裡的node做BFS
		sensor[CDS[i]].inputRoutingTable(sensor,NumOfNode);			//再建CDS的RoutingTable
		//sensor[CDS[i]].showRoutingTable();
		sensor[CDS[i]].setproxy();																	//設一下CDS的proxy
	}
	

	Outputpath(sensor, inputStream);
	delete[] sensor;
	sensor = NULL;

	system("pause\n");
	return 0;
}

void  Outputpath(MISNode *node, std::vector<std::vector<int>> & inputStream) {
	int edgecount = inputStream[0][1];
	int requestcount = inputStream[edgecount + 1][0];
	//inputStream >> requestCount;
	for (int i = 0; i < requestcount; i++) {
		int flowID = inputStream[edgecount + i + 2][0];
		int srcID = inputStream[edgecount + i + 2][1];;
		int destID = inputStream[edgecount + i + 2][2];;
		//inputStream >> flowID >> srcID >> destID;
		std::cout << flowID << '\t';
		Findpath(node, srcID, destID);
	}
}

void Findpath(MISNode *node, int srcID, int destID) {
	int currID = srcID;
	do {
		if (srcID == destID && node[srcID].getvisit() == false ) {
			std::cout << srcID << '\t' << node[srcID].getproxy() << '\t';
			break;
		}
		else if (node[srcID].getproxy() == node[destID].getproxy()) {
			std::cout << srcID << '\t' << node[srcID].getproxy() << '\t';
			break;
		}
		else if (node[currID].getproxy() == node[destID].getproxy()  ) {
			std::cout << currID << '\t';
			break;
		}
		
		else {
			std::cout << currID << '\t';
			currID = node[currID].sendTo(destID);
		}
	} while (currID != destID);
	std::cout << destID << std::endl;
}

std::vector<std::vector<int>> GetinputStream() {
	std::vector<std::vector<int>> inputStream;
	std::vector<int> count(2);
	//load node flows
	std::cin >> count[0] >> count[1];
	inputStream.push_back(count);
	for (int i = 0; i < count[1]; i++) {
		std::vector<int> flow(3);
		std::cin >> flow[0] >> flow[1] >> flow[2];
		inputStream.push_back(flow);
	}
	//load inputStream flows
	std::vector<int> requestcount(1);
	std::cin >> requestcount[0];
	inputStream.push_back(requestcount);
	for (int i = 0; i < requestcount[0]; i++) {
		std::vector<int> requestflow(3);
		std::cin >> requestflow[0] >> requestflow[1] >> requestflow[2];
		inputStream.push_back(requestflow);
	}
	return inputStream;
}