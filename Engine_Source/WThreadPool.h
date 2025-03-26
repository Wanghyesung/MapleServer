#pragma once

#include "Engine.h"
#include "WResources.h"

namespace W
{
	class ThreadPool
	{
	public:
		static void Initiailize(int _iThreadCount);
		static void Shutdown();

		static void Enqueu(std::function<void()> _func);

		template <typename T>
		static void LoadingResource(const std::wstring& _strResourceName , const std::wstring& _strPath);
		template <typename T>
		static void DeleteResource(const std::wstring& _strResourceName);

		static void Joinable(); //���ŷ �Լ�

	private:
		static std::vector<std::thread> m_vecWorker;
		static std::queue<std::function<void()>> m_queueTasks;

		static std::mutex m_mutex;
		static std::condition_variable m_CV;//�۾� ������ ������
		static std::atomic<bool> m_bRunnig;

		static std::mutex m_completeMutex; //�ܺ� ��� �������
		static std::condition_variable m_completeCV;
		static std::atomic<int> m_iWorkCount;
	};
	template<typename T>
	inline void ThreadPool::LoadingResource(const std::wstring& _strResourceName, const std::wstring& _strPath)
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queueTasks.push([=]()
				{
					//template���� template�Լ� ȣ��� template ���
					Resources::template Load<T>(_strResourceName, _strPath);
				});
		}
		m_iWorkCount.fetch_add(1);
		m_CV.notify_all();	
	}
	template<typename T>
	inline void ThreadPool::DeleteResource(const std::wstring& _strResourceName)
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queueTasks.push([=]()
				{
					//template���� template�Լ� ȣ��� template ���
					Resources::template Release<T>(_strResourceName);
				});
		}
		m_iWorkCount.fetch_add(1);
		m_CV.notify_all();
	}
}
