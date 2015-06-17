#include "stdafx.h"
#include "NSMutableArray.h"


NSMutableArray::NSMutableArray(void)
{
	count = 0;
	size = 32;
	arr = new void_ptr[size];
	for(int i = 0; i < size; i++)
		arr[i] = NULL;
}


NSMutableArray::~NSMutableArray(void)
{
	RemoveAllObjects();

	delete arr;
}

void NSMutableArray::AddObject(void * obj)
{
	CheckSpace(count + 1, count, 1);
	arr[count] = obj;
	count++;
}

void NSMutableArray::InsertObject(int idx, void * obj)
{
	if (idx >= 0 && idx <= count)
	{
		CheckSpace(count + 1, idx, 1);
	}
}

void NSMutableArray::RemoveObject(int idx)
{
	int newOffset = 0;
	if (idx >= 0 && idx < count)
	{
		delete arr[idx];
		arr[idx] = NULL;
	}

	for(int i = idx + 1; i < count; i++)
	{
		arr[i - 1] = arr[i];
	}
	
	arr[count] = NULL;
	count--;

}

void NSMutableArray::RemoveAllObjects(void)
{
	for(int i = 0; i < count; i++)
	{
		delete arr[i];
		arr[i] = NULL;
	}

	count = 0;
}

void NSMutableArray::CheckSpace(int newSize, int newIdx, int insSize)
{
	if (newSize >= size)
	{
		int newOffset = 0;
		void_ptr * newArr = new void_ptr[newSize + 16];
		for(int i = 0; i < count; i++)
		{
			if (i == newIdx)
				newOffset = insSize;
			newArr[i + newOffset] = arr[i];
		}
		for(int i = count + insSize; i < newSize + 16; i++)
			newArr[i] = NULL;
		delete arr;
		arr = newArr;
		size = newSize + 16;
	}
}

void * NSMutableArray::ObjectAtIndex(int idx)
{
	if (idx >= 0 && idx < count)
	{
		return arr[idx];
	}

	return NULL;
}

int NSMutableArray::Count(void)
{
	return count;
}
