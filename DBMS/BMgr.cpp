#include "BMgr.h"
#include <cstring>
struct BCB
{
	BCB(int page_id, int frame_id, bool dirty)
	{
		this->page_id = page_id;
		this->frame_id = frame_id;
		this->dirty = dirty;
		this->next = NULL;
	}
	int page_id;//页号
	int frame_id;//缓冲区块号
	int dirty;//脏数据位
	BCB * next;
};

/*
对page_id进行prot（0,1）操作
case 0:读
通过page_id找到BCB，
如果找到，说明此页在内存中，Hit+1，移动LRU对应frame至头部
如果没有，IO+1，判断内存是否已满(LRU.size)
若已满，
（1）删除LRU最后一个节点，并删除其对应的BCB
（2）删除BCB时，若dirty为true,IO+1
（3）在对应frame中插入新的BCB
（4）更新ftop[frame_id] = page_id
（5）移动LRU对应frame至头部
若未满
（1）在对应frame中插入新的BCB
（2）更新ftop[frame_id] = page_id
（3）移动LRU对应frame至头部
case 1:写
与读操作类似，有两方面增加
（1）若page_id在内存中，那么修改对应BCB的dirty为true
（2）若page_id不在内存中，新增的BCB的dirty设为true
*/

BMgr::BMgr()
{
	IO_Num = 0;
	Hit_Num = 0;
	memset(ftop, -1, sizeof(ftop));
	for (int i = 0; i < BufferSize; i++)
		ptof[i] = new BCB(0, 0, false);
};

//在缓冲区找一个页框
int BMgr::FixPage(int page_id, int prot)
{
	int ha = Hash(page_id);
	BCB *bcb = ptof[ha]->next;
	while (bcb != NULL)
	{
		if (bcb->page_id == page_id)
			//找到，说明该页在内存中
			//同时修改LRU
			//如果是写操作，还需将dirty位设为true
		{
			Hit_Num++;
			LRU.remove(bcb->frame_id);
			LRU.push_front(bcb->frame_id);
			if (prot == 1) bcb->dirty = true;
			return bcb->frame_id;
		}
		bcb = bcb->next;
	}
	//未找到，需要从磁盘中读取，IO + 1，判断内存是否已满(LRU.size)
	IO_Num++;

	int frame_id = -1;
	//找出闲置frame,用以缓存从磁盘中读取的内容
	if (LRU.size() == BufferSize)//内存已满
	{
		/*
		（1）删除LRU最后一个节点，并删除其对应的BCB
		（2）删除BCB时，若dirty为true,IO+1
		*/
		frame_id = SelectVictim();
	}
	else
	{
		for (int id = 0; id < BufferSize; id++)
		if (ftop[id] == -1)//表示frame[i]还未被使用
		{
			frame_id = id;
			break;
		}
	}

	/*
	（1）在page_id对应BCB中插入(frame,page)
	（2）更新ftop[frame_id] = page_id
	（3）移动LRU对应frame至头部
	*/
	BCB *ptr = ptof[ha];
	BCB *new_bcb = new BCB(page_id, frame_id, false);
	if (prot == 1) new_bcb->dirty = true;
	new_bcb->next = ptr->next;
	ptr->next = new_bcb;

	ftop[frame_id] = page_id;
	LRU.push_front(frame_id);

	return frame_id;
}

//删除LRU最后一个节点，并删除其对应的BCB
int BMgr::SelectVictim()
{
	int frame_id = LRU.back();
	LRU.pop_back();
	int page_id = ftop[frame_id];
	RemoveBCB(ptof[Hash(page_id)], page_id);
	return frame_id;
}


int BMgr::Hash(int page_id)
{
	return page_id%BufferSize;
}

/*
删除page_id对应的BCB
*/
void BMgr::RemoveBCB(BCB * ptr, int page_id)
{
	BCB *head = ptr->next;
	while (head != NULL)
	{
		if (head->page_id == page_id)
		{
			if (head->dirty) IO_Num++;
			ptr->next = head->next;
			delete head;
			break;
		}
		ptr = head;
		head = head->next;
	}
}

/*
根据frame_id找到page_id，然后找到相应
的BCB,再修改其中的dirty值
*/
void BMgr::SetDirty(int frame_id)
{
	int page_id = ftop[frame_id];
	int ha = Hash(page_id);
	BCB *bcb = ptof[ha]->next;
	while (bcb != NULL)
	{
		if (bcb->frame_id == frame_id)
		{
			bcb->dirty = true;
			break;
		}
		bcb = bcb->next;
	}
}