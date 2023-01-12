#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
vector <vector <unsigned int >>VectorStorePath ;

class NODE {																													//����NODE
public:
	NODE();																														//�غc�l
	unsigned int  SendTo(unsigned int Dst,unsigned int NodeMinus);			//�@��禡�A�\��p�W�r
	unsigned int  GetValue(void);
	unsigned int  GetDistance(void);
	void StoreNeighbor(unsigned int NeighborNode);
	void SetValue(unsigned int);
	void SetDistance(unsigned int);
	void MakeARoutingTable(NODE* ptr,unsigned int Node);
	vector<unsigned int>* GetNeighbor(void);
	//void PrintRoutingTable(unsigned int Node);

private:																															//����򥻸��
	unsigned int ID;
	unsigned int Distance;
	vector <unsigned int > Neighbor;
	vector  <vector <unsigned  int>> RoutingTable;
};

NODE::NODE() {
	ID = 0;																														//ID
	Distance = 0;																											//�Z���Y�@��node���Z��
}

void NODE::StoreNeighbor(unsigned int NeighborNode) {							//�o��node�M����node�O�F�~
	Neighbor.push_back(NeighborNode);
}

void NODE::SetValue(unsigned int SelfID) {
	ID = SelfID;
}

void NODE::SetDistance(unsigned int DistanceToSource) {
	Distance = DistanceToSource;
}

unsigned int NODE::GetValue() {
	return  ID;
}

unsigned int NODE::GetDistance() {
	return Distance;
}

vector<unsigned int>* NODE::GetNeighbor() {															//�o��F�~������
	vector <unsigned int >* VectorOut = new vector<unsigned int>;
	*VectorOut = Neighbor;
	return VectorOut;
}

unsigned int  NODE::SendTo(unsigned int Dst,unsigned int NodeMinus) {		//�Ѯv�����禡
	unsigned int result = 0;
	for (int i = 0; i < NodeMinus; i++) {
		if (RoutingTable[i][0] == Dst)
			result = RoutingTable[i][1];
	}
	return result;
}
/*
void NODE::PrintRoutingTable(unsigned int Node) {
	for (int i = 0; i < Node-1 ; i++) {
		for (int j = 0; j < 2; j++)
			cout << RoutingTable[i][j] << '\t';
		cout << endl;
	}
	cout << endl << endl;
}
*/
void NODE::MakeARoutingTable(NODE* ptr,unsigned int Node) {						//��table�Atable�� -���I--�U�Ӹ`�I-2��
			
	vector <vector <unsigned int >>OuterVector;									
	for (int i = 0; i < Node; i++) {

		if ((ptr + i)->GetValue() == ID)																				//���Ϋز��I���ۤv�����C
			continue;

		vector <unsigned int>InnerVec;
		unsigned int Dis = (ptr + i)->GetDistance();

		if ((ptr + i)->Distance == 1) {																					//�Z����1(�F�~)�N�����s��next
			InnerVec.push_back(i);
			InnerVec.push_back(i);
			OuterVector.push_back(InnerVec);
		}
		else {																																//�Z������1�N�h��F�~�Z����1��
			vector <unsigned int >*fuck;
			vector <unsigned int >::iterator iter;
			fuck = (ptr + i)->GetNeighbor();																			//fuck ���V�@�ƾF�~


			for (iter = (*fuck).begin(); iter != (*fuck).end(); ) {
				if ((ptr + *iter)->Distance == 1) {																	//�p�G���F�~�Z����1���F
					InnerVec.push_back(i);
					InnerVec.push_back(*iter);
					OuterVector.push_back(InnerVec);															//�s�i�h
					break;
				}
				else {
					if ((ptr + *iter)->Distance == Dis - 1) {														//�p�G���䤣��
						fuck = (ptr + *iter)->GetNeighbor();														//��̤pID������
						iter = (*fuck).begin();																					//���]�F�~(�F�~�ܦ��̤pID������)
						Dis = Dis - 1;
						continue;
					}
					else
						iter++;
				}
			}
		}

	}
	RoutingTable = OuterVector;
}

void ContructNodeNeighbor(NODE *ptr, unsigned int Node , unsigned int Path);
void BFS(unsigned int Source,unsigned int Node, NODE *ptr);
NODE* CreateObject(unsigned int NumOfNode);


int main() {

	NODE* ptr = NULL;
	unsigned int NodeNumber = 0, PathNumber = 0;									//�@����J			
	cin >> NodeNumber  >>PathNumber ;

	for (unsigned int i = 0 ; i < PathNumber ; i++) {											
		unsigned int Path, Start, End ;  
		vector <unsigned int > VectorStore;
		cin >> Path >> Start >> End;
		VectorStore.push_back(Path);
		VectorStore.push_back(Start);
		VectorStore.push_back(End);
		VectorStorePath.push_back(VectorStore);
	}

	ptr = CreateObject(NodeNumber) ;															//�ھڿ�J�إߪ���B����
	ContructNodeNeighbor(ptr, NodeNumber, PathNumber) ;
	
	for (unsigned int i = 0; i < NodeNumber; i++) {
		BFS(i, NodeNumber, ptr);																			//����BFS��إ�TABLE
		(ptr + i)->MakeARoutingTable(ptr,NodeNumber);
		//(ptr + i)->PrintRoutingTable(NodeNumber);
	}
	
	unsigned int PathYouWant = 0;  
	cin >> PathYouWant ;
	
	vector <vector<unsigned int>>VecSrcAndDst ;
	for (unsigned int i = 0; i < PathYouWant; i++) {										//��J�A�n��path
		
		unsigned int No = 0, Start = 0, End = 0;
		cin >> No >> Start >> End;
		vector<unsigned int >Vec;
		Vec.push_back(No);
		Vec.push_back(Start);
		Vec.push_back(End);
		VecSrcAndDst.push_back(Vec);
	}

	for (unsigned int i = 0; i < PathYouWant ; i++) {										//����|�Ϊ�
		unsigned int Cur_ID = VecSrcAndDst[ i ][1];										//Source
		unsigned int Dst_ID = VecSrcAndDst[ i ][2];											//Destination
		cout << i << '\t';
		while (Cur_ID != Dst_ID) {																			//Source!=Destination
			cout << Cur_ID << '\t' ;
			Cur_ID = (ptr + Cur_ID)->SendTo(Dst_ID, NodeNumber-1);		//���
		}
		cout << Cur_ID << endl;
	}

	return 0;
}

NODE*  CreateObject(unsigned int NumOfNode) {									//create the object  NumberOfNode 

	NODE *PTR = new NODE[NumOfNode];													//ptr point to the six node's head 
	for (unsigned int i = 0; i < NumOfNode; i++)
		(PTR + i)->SetValue(i);
	return PTR;
}

void ContructNodeNeighbor(NODE* ptr,unsigned int Node,unsigned int Path) {						//	record node neighbor  
	
	for (int i = 0; i < Path; i++) {
		for (unsigned int j = 0; j < Node; j++) {
			if (VectorStorePath[i][1] == (ptr + j)->GetValue())
				(ptr + j)->StoreNeighbor(VectorStorePath[i][2]);
		}
		for (unsigned int j = 0; j < Node; j++) {
			if (VectorStorePath[i][2] == (ptr + j)->GetValue())
				(ptr + j)->StoreNeighbor(VectorStorePath[i][1]);
		}

	}
}

void BFS(unsigned int Source,unsigned  int Node, NODE* ptr) {														//BFS
	
	for(unsigned int i = 0 ; i < Node; i++)
		(ptr + i)->SetDistance(0);

	bool *visited = new bool[Node];
	for (unsigned int i = 0; i < Node; i++)
		visited[i] = false;

	queue <unsigned int > CuteQueue;
	visited[Source] = true;
	CuteQueue.push(Source);

	vector<unsigned int> ::iterator i;
	while (!CuteQueue.empty()) {

		Source = CuteQueue.front();
		CuteQueue.pop();

		unsigned int count = ((ptr + Source)->GetDistance()) + 1;
		vector <unsigned int >* Neighbor;
		Neighbor = ((ptr + Source)->GetNeighbor());

		for (i = (*Neighbor).begin(); i != (*Neighbor).end(); i++) {
			if (!visited[*i]) {
				visited[*i] = true;
				CuteQueue.push(*i);
				(ptr + *i)->SetDistance(count);
			}
		}
	}
}