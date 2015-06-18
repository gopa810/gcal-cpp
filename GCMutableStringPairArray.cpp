#include "stdafx.h"
#include "GCMutableStringPairArray.h"
#include "GCStringPair.h"

GCMutableStringPairArray::GCMutableStringPairArray(void)
{
	count = 0;
	size = 32;
	arr = (GCStringPair **)new strpair_ptr[size];
	for(int i = 0; i < size; i++)
		arr[i] = NULL;
}


GCMutableStringPairArray::~GCMutableStringPairArray(void)
{
	if (count > 0)
	{
		RemoveAllObjects();
	}


	delete arr;
}

void GCMutableStringPairArray::AddObject(GCStringPair * obj)
{
	CheckSpace(count + 1, count, 1);
	arr[count] = obj;
	count++;
}

void GCMutableStringPairArray::InsertObject(int idx, GCStringPair * obj)
{
	if (idx >= 0 && idx <= count)
	{
		CheckSpace(count + 1, idx, 1);
		arr[idx] = obj;
		count++;
	}
}

void GCMutableStringPairArray::RemoveObject(int idx)
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

void GCMutableStringPairArray::RemoveAllObjects(void)
{
	for(int i = 0; i < count; i++)
	{
		delete arr[i];
		arr[i] = NULL;
	}

	count = 0;
}

void GCMutableStringPairArray::CheckSpace(int newSize, int newIdx, int insSize)
{
	if (newSize >= size)
	{
		int newOffset = 0;
		GCStringPair ** newArr = (GCStringPair **) new strpair_ptr[newSize + 16];
		for(int i = 0; i < count; i++)
		{
			if (i == newIdx)
				newOffset = insSize;
			newArr[i + newOffset] = arr[i];
		}
		for(int i = count + insSize; i < newSize + 16; i++)
		{
			newArr[i] = NULL;
		}
		delete arr;
		arr = newArr;
		size = newSize + 16;
	}
}

GCStringPair * GCMutableStringPairArray::ObjectAtIndex(int idx)
{
	if (idx >= 0 && idx < count)
	{
		return (GCStringPair *)arr[idx];
	}

	return NULL;
}

int GCMutableStringPairArray::Count(void)
{
	return count;
}

