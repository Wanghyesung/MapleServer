#pragma once


class MemoryBlock
{
public:
	MemoryBlock(size_t _size, BYTE* _buffer, size_t _count);
	~MemoryBlock();

public:
	class MemoryHeader* Pop();
	void Push(MemoryHeader* _ptr);


private:
	size_t m_size;//�� ������
	size_t m_memoryCount; //��� ��������
	BYTE* m_Buffer;//�޸� Ǯ���� �Ҵ����� �� ����

/*
���� ����Ʈ ��� ����/ť�� �޸� ������ �л�Ǿ� ĳ�� ģȭ������ ���� �� ����.
��尡 �������� �޸� ������ ��ġ���� �ʱ� ����. �迭 ��� ����/ť�� ��ȯ
ĳ�� ��Ʈ�� ����.
�޸� ���� �ӵ� ���.
*/
	RWLock m_Lock;
	vector<BYTE*> m_Memroy;//���� ����Ʈ�� �� �����ŭ ������ ����
	MemoryPool* m_pOwner;


	friend class MemoryPool;
};

