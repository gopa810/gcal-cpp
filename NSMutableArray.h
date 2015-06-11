#pragma once

typedef void * void_ptr;

class NSMutableArray
{
	void_ptr * arr;
	int count;
	int size;

	void CheckSpace(int size, int newIdx, int insSize);
public:
	NSMutableArray(void);
	~NSMutableArray(void);


	int Count(void);
	void * ObjectAtIndex(int idx);
	void AddObject(void * obj);
	void InsertObject(int idx, void * obj);
	void RemoveObject(int idx);
	void RemoveAllObjects(void);
};

