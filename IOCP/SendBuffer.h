#pragma once

class SendBufferChunk;

class SendBuffer : public enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(shared_ptr<SendBufferChunk> _pOwner, BYTE* _buffer, size_t _BufferSize);
	~SendBuffer();

	int GetBufferSize() { return m_iBufferSize; }
	int GetWritePos() { return m_iWritePos; }
	BYTE* GetData() { return m_sendBuffer; }
	void Close(int _iLen);

private:
	int m_iBufferSize;
	int m_iWritePos;
	BYTE* m_sendBuffer;

	//�� ���۴� sendbufferchunk���� �� �����Ա� ������ sendbufferchunk�� ���ڱ� ������� �ȵ� ������ shared�� ������� ���ϰ� ����
	shared_ptr<SendBufferChunk> m_pOwner;
};




