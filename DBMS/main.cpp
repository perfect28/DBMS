#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <windows.h>

#include "BMgr.h"
#include "DSMgr.h"
void split(string s, string delim, vector<string>& ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != string::npos)
	{
		ret.push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		ret.push_back(s.substr(last, index - last));
	}
}


int main()
{
	BMgr *bmgr = new BMgr();
	string filename = "data-5w-50w-zipf.txt";
	ifstream fin(filename);
	string line;
	double TimeStart = GetTickCount();
	
	int cnt = 0;
	while (!fin.eof()) {
		cout << ++cnt << endl;
		getline(fin, line);
		//vector<string> res;
		//split(line, ",", res);
		//int prot = stoi(res[0]);
		//int page_id = stoi(res[1]);
		int prot = stoi(line.substr(0, 1));
		int page_id = stoi(line.substr(2, line.size() - 2));
		int frame_id = bmgr->FixPage(page_id, prot);
	}
	cout << "IO_Num: " << bmgr->Get_IO_Num() << endl;
	cout << "Hit_Num: " << bmgr->Get_Hit_Num() << endl;

	double TimeEnd = GetTickCount();
	double TimeUsed = TimeEnd - TimeStart;
	cout << "time cost:" << TimeUsed<< endl;

	return 0;
}