#ifndef DSMGR_H
#define DSMGR_H

#include <string>
using namespace std;

#define MAXPAGES 50000
#define FrameSize 4096
struct bFrame;

class DSMgr
{
public:
	DSMgr();
	int OpenFile(string filename);
	int CloseFile();
	bFrame ReadPage(int page_id);
	int WritePage(int frame_id, bFrame frm);
	int Seek(int offset, int pos);
	FILE * GetFile();
	void IncNumPages();
	int GetNumPages();
	void SetUse(int index, int use_bit);
	int GetUse(int index);
private:
	FILE *currFile;
	int numPages;
	int pages[MAXPAGES];
};

#endif