#pragma once
class RecvBuffer
{
public:
	RecvBuffer(int _iBufferSize);
	~RecvBuffer();

	BYTE* GetReadPos() { return &m_vecRecvBuffer[m_iReadPos]; }
	BYTE* GetWritePos() { return &m_vecRecvBuffer[m_iWritePos]; }
	int DataSize() { return m_iWritePos - m_iReadPos; }
	int FreeSize() { return m_iCapacity - m_iWritePos; }


	bool Read(int _iNumOfBytes);
	bool Write(int _iNumOfBytes);

	bool Clear();

private:
	vector<BYTE> m_vecRecvBuffer;


	int m_iCapacity;//�뷮
	int m_iBufferSize;//��ü ������
	int m_iReadPos;//���� �о���� ��ġ
	int m_iWritePos;//���ݱ��� �� ��ġ

};

