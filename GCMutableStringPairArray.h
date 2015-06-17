#pragma once

typedef void * strpair_ptr;
class GCStringPair;

class GCMutableStringPairArray
{
	GCStringPair ** arr;
	int count;
	int size;

	void CheckSpace(int size, int newIdx, int insSize);

public:
	GCMutableStringPairArray(void);
	~GCMutableStringPairArray(void);

	int Count(void);
	GCStringPair * ObjectAtIndex(int idx);
	void AddObject(GCStringPair * obj);
	void InsertObject(int idx, GCStringPair * obj);
	void RemoveObject(int idx);
	void RemoveAllObjects(void);

};

