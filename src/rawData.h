#pragma once

class rawData
{
	struct rawDataHeader
	{
		int size;
		int alloc;
	} ;

private:
	char* data;
	int	dataSize;
	int	dataAlloc;
	int	dataStep;

	int create(int size);

public:
	rawData(int size, int step);
	~rawData();
	const char* get(int index);
	int set(int index, const char* _data, int size);
	int alloc(int size);
};

