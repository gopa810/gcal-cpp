#pragma once

typedef void * dict_ptr;
class GCMutableDictionary;

class GCMutableArray
{
	GCMutableDictionary ** arr;
	int count;
	int size;

	void CheckSpace(int size, int newIdx, int insSize);
public:
	GCMutableArray(void);
	~GCMutableArray(void);


	int Count(void);
	GCMutableDictionary * ObjectAtIndex(int idx);
	void AddObject(GCMutableDictionary * obj);
	void InsertObject(int idx, GCMutableDictionary * obj);
	void RemoveObject(int idx);
	void RemoveAllObjects(void);

	void SortByStringKey(char * key);
	void SortByIntKey(char * key);
	void SortByStringKey(char * key, int desc);
	void SortByIntKey(char * key, int desc);

};

