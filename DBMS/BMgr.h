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
	int FixPage(int page_id, int prot);//对page_id进行prot操作
	// Internal Functions
	int SelectVictim();//选择要置换的frame
	int Hash(int page_id);//返回page_id对应的frame_id的BCB
	void RemoveBCB(BCB * ptr, int page_id);//删除page_id对应的BCB
	void SetDirty(int frame_id);//写frame_id
	int Get_IO_Num(){ return IO_Num; }
	int Get_Hit_Num(){ return Hit_Num; }
private:
	// Hash Table
	int ftop[BufferSize];//缓存到page的映射，返回page对应缓存的frame_id
	BCB* ptof[BufferSize];//page到缓存的映射，由于多个page可能映射到一个frame,所以返回BCB指针

	list<int> LRU;
	int IO_Num;//IO次数
	int Hit_Num;//命中次数
};
# endif