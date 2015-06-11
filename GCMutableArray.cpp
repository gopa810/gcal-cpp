#include "stdafx.h"
#include "GCMutableArray.h"
#include "GCMutableDictionary.h"

GCMutableArray::GCMutableArray(void)
{
	count = 0;
	size = 32;
	arr = (GCMutableDictionary **)new dict_ptr[size];
}


GCMutableArray::~GCMutableArray(void)
{
	RemoveAllObjects();

	delete arr;
}

void GCMutableArray::AddObject(GCMutableDictionary * obj)
{
	CheckSpace(count + 1, count, 1);
	arr[count] = obj;
	count++;
}

void GCMutableArray::InsertObject(int idx, GCMutableDictionary * obj)
{
	if (idx >= 0 && idx <= count)
	{
		CheckSpace(count + 1, idx, 1);
		arr[idx] = obj;
		count++;
	}
}

void GCMutableArray::RemoveObject(int idx)
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

void GCMutableArray::RemoveAllObjects(void)
{
	for(int i = 0; i < count; i++)
	{
		delete arr[i];
		arr[i] = NULL;
	}

	count = 0;
}

void GCMutableArray::CheckSpace(int newSize, int newIdx, int insSize)
{
	if (newSize >= size)
	{
		int newOffset = 0;
		GCMutableDictionary ** newArr = (GCMutableDictionary **) new dict_ptr[newSize + 16];
		for(int i = 0; i < count; i++)
		{
			if (i == newIdx)
				newOffset = insSize;
			newArr[i + newOffset] = arr[i];
		}
		delete arr;
		arr = newArr;
		size = newSize + 16;
	}
}

GCMutableDictionary * GCMutableArray::ObjectAtIndex(int idx)
{
	if (idx >= 0 && idx < count)
	{
		return (GCMutableDictionary *)arr[idx];
	}

	return NULL;
}

int GCMutableArray::Count(void)
{
	return count;
}


void GCMutableArray::SortByStringKey(char * key)
{
	SortByStringKey(key, 1);
}

void GCMutableArray::SortByIntKey(char * key)
{
	SortByIntKey(key, 1);
}


//
// desc   1 : ascending
//       -1 : descending
//
void GCMutableArray::SortByStringKey(char * key, int desc)
{
	int res;

	for(int m = 0; m < count - 1; m++)
	{
		for(int n = m + 1; n < count; n++)
		{
			res = GCMutableDictionary::compareStrings(arr[m]->stringForKey(key), arr[n]->stringForKey(key));
			res *= desc;
			if (res > 0)
			{
				GCMutableDictionary * d = arr[m];
				arr[m] = arr[n];
				arr[n] = d;
			}
		}
	}
}

void GCMutableArray::SortByIntKey(char * key, int desc)
{
	int res;
	for(int m = 0; m < count - 1; m++)
	{
		for(int n = m + 1; n < count; n++)
		{
			res = arr[m]->intForKey(key) - arr[n]->intForKey(key);
			res *= desc;
			if (res > 0)
			{
				GCMutableDictionary * d = arr[m];
				arr[m] = arr[n];
				arr[n] = d;
			}
		}
	}
}


