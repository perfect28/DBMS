#ifndef BMGR_H
#define BMGR_H

#include <list>
using namespace std;

#define BufferSize 1024
struct BCB;

class BMgr
{
public:
	BMgr();
	// Interface functions
	int FixPage(int page_id, int prot);//��page_id����prot����
	// Internal Functions
	int SelectVictim();//ѡ��Ҫ�û���frame
	int Hash(int page_id);//����page_id��Ӧ��frame_id��BCB
	void RemoveBCB(BCB * ptr, int page_id);//ɾ��page_id��Ӧ��BCB
	void SetDirty(int frame_id);//дframe_id
	int Get_IO_Num(){ return IO_Num; }
	int Get_Hit_Num(){ return Hit_Num; }
private:
	// Hash Table
	int ftop[BufferSize];//���浽page��ӳ�䣬����page��Ӧ�����frame_id
	BCB* ptof[BufferSize];//page�������ӳ�䣬���ڶ��page����ӳ�䵽һ��frame,���Է���BCBָ��

	list<int> LRU;
	int IO_Num;//IO����
	int Hit_Num;//���д���
};
# endif