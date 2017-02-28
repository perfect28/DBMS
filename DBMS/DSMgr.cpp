#include "DSMgr.h"

struct bFrame
{
	char field[FrameSize];
};

DSMgr::DSMgr()
{

}
int DSMgr::OpenFile(string filename)
{
	return 0;
}
int DSMgr::CloseFile()
{
	return 0;
}
bFrame DSMgr::ReadPage(int page_id)
{
	bFrame frame;
	return frame;
}
int DSMgr::WritePage(int frame_id, bFrame frm)
{
	return 0;
}
int DSMgr::Seek(int offset, int pos)
{
	return 0;
}
FILE * DSMgr::GetFile()
{
	FILE* file = NULL; 
	return file;
}
void DSMgr::IncNumPages()
{
}
int DSMgr::GetNumPages()
{
	return 0;
}
void DSMgr::SetUse(int index, int use_bit)
{
}
int DSMgr::GetUse(int index)
{
	return 0;
}