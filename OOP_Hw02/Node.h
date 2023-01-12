#include <vector>
#include <fstream>

#ifndef NODE_H_
#define NODE_H_

class Node {
public:
	Node() { };																																				//�غc�l
	void inputID(int inputid);																													//��JID
	void inputNeighbor(int edgecount, std::vector<std::vector<int>> & inputStream);		//�ؤ@�UNODE�j�����H
	void showNeighborhood();
	void showRoutingTable();
	int sendTo(int destID);
	void inputRoutingTable(Node *node, int nodecount);
	friend void BFS(Node *node, int start, int nodecount);
	~Node() {};
protected:
	std::vector<std::vector<int>> routingTable;
	std::vector<int> neighbor;
	unsigned int id;
	int tag;
};

class MISNode :public  Node{
public :
	MISNode() : status(1),proxy(-2) ,MIS(false),visit(false){};
	void compareNodeID( MISNode* arg);
	void modifystatus();								//����active��inactive���A
	void setvisited();										//�аO�Ivisited or not
	void setproxy();
	int getid();
	int getstatus();
	int getproxy();
	bool getMIS();
	bool getvisit();


	std::vector <int> getneighbor();
	friend void BFS(MISNode *node, int start, int nodecount);
	void inputRoutingTable(MISNode *node, int nodecount);
	void AODV(MISNode* node, int nodecount,std::vector<int> ConnectedDS);

private:
	int status;												// 1�N��Active ,0�N��inactive
	int proxy;
	bool MIS;
	bool visit;
};

#endif