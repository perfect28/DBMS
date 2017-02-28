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
	int page_id;//ҳ��
	int frame_id;//���������
	int dirty;//������λ
	BCB * next;
};

/*
��page_id����prot��0,1������
case 0:��
ͨ��page_id�ҵ�BCB��
����ҵ���˵����ҳ���ڴ��У�Hit+1���ƶ�LRU��Ӧframe��ͷ��
���û�У�IO+1���ж��ڴ��Ƿ�����(LRU.size)
��������
��1��ɾ��LRU���һ���ڵ㣬��ɾ�����Ӧ��BCB
��2��ɾ��BCBʱ����dirtyΪtrue,IO+1
��3���ڶ�Ӧframe�в����µ�BCB
��4������ftop[frame_id] = page_id
��5���ƶ�LRU��Ӧframe��ͷ��
��δ��
��1���ڶ�Ӧframe�в����µ�BCB
��2������ftop[frame_id] = page_id
��3���ƶ�LRU��Ӧframe��ͷ��
case 1:д
����������ƣ�������������
��1����page_id���ڴ��У���ô�޸Ķ�ӦBCB��dirtyΪtrue
��2����page_id�����ڴ��У�������BCB��dirty��Ϊtrue
*/

BMgr::BMgr()
{
	IO_Num = 0;
	Hit_Num = 0;
	memset(ftop, -1, sizeof(ftop));
	for (int i = 0; i < BufferSize; i++)
		ptof[i] = new BCB(0, 0, false);
};

//�ڻ�������һ��ҳ��
int BMgr::FixPage(int page_id, int prot)
{
	int ha = Hash(page_id);
	BCB *bcb = ptof[ha]->next;
	while (bcb != NULL)
	{
		if (bcb->page_id == page_id)
			//�ҵ���˵����ҳ���ڴ���
			//ͬʱ�޸�LRU
			//�����д���������轫dirtyλ��Ϊtrue
		{
			Hit_Num++;
			LRU.remove(bcb->frame_id);
			LRU.push_front(bcb->frame_id);
			if (prot == 1) bcb->dirty = true;
			return bcb->frame_id;
		}
		bcb = bcb->next;
	}
	//δ�ҵ�����Ҫ�Ӵ����ж�ȡ��IO + 1���ж��ڴ��Ƿ�����(LRU.size)
	IO_Num++;

	int frame_id = -1;
	//�ҳ�����frame,���Ի���Ӵ����ж�ȡ������
	if (LRU.size() == BufferSize)//�ڴ�����
	{
		/*
		��1��ɾ��LRU���һ���ڵ㣬��ɾ�����Ӧ��BCB
		��2��ɾ��BCBʱ����dirtyΪtrue,IO+1
		*/
		frame_id = SelectVictim();
	}
	else
	{
		for (int id = 0; id < BufferSize; id++)
		if (ftop[id] == -1)//��ʾframe[i]��δ��ʹ��
		{
			frame_id = id;
			break;
		}
	}

	/*
	��1����page_id��ӦBCB�в���(frame,page)
	��2������ftop[frame_id] = page_id
	��3���ƶ�LRU��Ӧframe��ͷ��
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

//ɾ��LRU���һ���ڵ㣬��ɾ�����Ӧ��BCB
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
ɾ��page_id��Ӧ��BCB
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
����frame_id�ҵ�page_id��Ȼ���ҵ���Ӧ
��BCB,���޸����е�dirtyֵ
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