#pragma once

typedef void * void_ptr;

template <class T> class NSMutableArray
{
	void_ptr * arr;
	int count;
	int size;

	inline void CheckSpace(int newSize, int newIdx, int insSize)
	{
		if (newSize >= size)
		{
			int newOffset = 0;
			void_ptr * newArr = new void_ptr[newSize + 16 + insSize];
			for(int i = 0; i < count; i++)
			{
				if (i == newIdx)
					newOffset = insSize;
				newArr[i + newOffset] = arr[i];
			}
			for(int i = count + insSize; i < newSize + 16 + insSize; i++)
				newArr[i] = NULL;
			delete arr;
			arr = newArr;
			size = newSize + 16 + insSize;
		}
	}

public:
	NSMutableArray(void)
	{
		count = 0;
		size = 32;
		arr = new void_ptr[size];
		for(int i = 0; i < size; i++)
			arr[i] = NULL;
	}

	~NSMutableArray(void)
	{
		RemoveAllObjects();
		delete arr;
	}



	int Count(void)
	{
		return count;
	}

	T * ObjectAtIndex(int idx)
	{
		if (idx >= 0 && idx < count)
		{
			return (T *)arr[idx];
		}

		return NULL;
	}

	void AddObject(T * obj)
	{
		CheckSpace(count + 1, count, 1);
		arr[count] = obj;
		count++;
	}

	void InsertObject(int idx, T * obj)
	{
		if (idx >= 0 && idx <= count)
		{
			CheckSpace(count + 1, idx, 1);
			arr[idx] = obj;
		}
	}

	void SetObjectAtIndex(int idx, T * obj)
	{
		while(count <= idx)
		{
			AddObject(NULL);
		}

		arr[idx] = obj;
	}

	void RemoveObject(int idx)
	{
		int newOffset = 0;
		if (idx >= 0 && idx < count)
		{
			T * item = (T *)arr[idx];
			delete item;
			arr[idx] = NULL;
		}

		for(int i = idx + 1; i < count; i++)
		{
			arr[i - 1] = arr[i];
		}
	
		arr[count] = NULL;
		count--;

	}

	void RemoveAllObjects(void)
	{
		for(int i = 0; i < count; i++)
		{
			T * item = (T *)arr[i];
			delete item;
			arr[i] = NULL;
		}

		count = 0;
	}

};

