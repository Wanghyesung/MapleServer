#pragma once
class MemoryBlock;

#define MAX_POOLING_SIZE 2048

class Knight
{
public:
	Knight() :ll(0), hp(100), tl('t'), tem(4214)
	{
		int a = 10;
	};
	Knight(long long _ll, int _hp, char _t, int _tem) :
		ll(_ll), hp(_hp), tl(_t), tem(_tem)
	{
		int a = 10;
	}
	~Knight() {}
public:
	long long ll;
	int hp;
	char tl;
	int tem;
};


class MemoryHeader
{
	//���� ������� �޸� ũ�� Ȯ�ο�
public:
	MemoryHeader(int _iSize) : m_iSize(_iSize) {};
	~MemoryHeader() {};

	static void* AttachHeader(MemoryHeader* _pHeader, int _iSize)
	{
		//��ü�� �̹� Ȯ���� �޸𸮿� �����Ͽ� �޸� �Ҵ��� ������带 ���Դϴ�.
		new(_pHeader)MemoryHeader(_iSize);
		return reinterpret_cast<void*>(_pHeader + 1);
	}

	//������� �޸𸮿� ���ũ�� ��ŭ �̵��� �޸� ��� ��ȯ
	static MemoryHeader* DetachHeader(void* _ptr)
	{
		//MemoryHeader��ŭ �ڷ� ���� ���� �ּ� ��ȯ
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(_ptr) - 1;
		return header;
	}

public:
	int m_iSize;
};

enum MemorySize
{
	COUNT_32	=  2048,
	COUNT_64	=  1024,
	COUNT_128	=  512,
	COUNT_256	=  256,
	COUNT_512	=  128,
	COUNT_1024	=  64,
	COUNT_2048	=  32,

	MAX_SIZE = 32   *   COUNT_32	+ 
			   64   *   COUNT_64	+ 
			   128  *   COUNT_128	+ 
			   256  *   COUNT_256	+ 
			   512  *   COUNT_512	+ 
			   1024 *   COUNT_1024	+ 
	           2048 *   COUNT_2048,
};



class MemoryPool
{

public:
	MemoryPool();
	~MemoryPool();

public:
	MemoryHeader* Pop(size_t _size);
	void Push(MemoryHeader* _ptr);

private:
	//����� ũ���� ���۸� ã�� �뵵
	vector<MemoryBlock*> m_vecMemory;
	BYTE m_Buffer[MAX_SIZE];
};



