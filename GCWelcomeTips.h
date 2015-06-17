#pragma once
class GCWelcomeTips
{
public:
	GCWelcomeTips(void);
	~GCWelcomeTips(void);
	
	static void initializeFile(FILE * hFile);

	static const TCHAR szSection[];
	static const TCHAR szIntFilePos[];
	static const TCHAR szTimeStamp[];
	static const TCHAR szIntStartup[];

};

