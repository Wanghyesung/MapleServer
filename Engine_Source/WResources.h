#pragma once

#include "WResource.h"

namespace W
{
	class Resources
	{
	public:
		template <typename T>
		static std::shared_ptr<T> Find(const std::wstring& _strKey)
		{
			//���ҽ��ʿ��� �����͸� Ž���Ѵ� �����Ͱ� �ִٸ� �ش絥���͸� ��ȯ�ϰ�
			//�����Ͱ� ���ٸ� end�� ��ȯ���ش�.
			std::map<std::wstring, std::shared_ptr<Resource>>::iterator iter = m_mapResources.begin();

			{
				std::lock_guard<std::mutex> lock(m_mutex);
				iter = m_mapResources.find(_strKey);
			}
			//ã���� �ϴ� �����Ͱ� �����Ѵٸ�
			//�ش�Ÿ������ ����ȯ�Ͽ� ��ȯ
			if (iter != m_mapResources.end())
			{
				return std::dynamic_pointer_cast<T>(iter->second);
			}

			//������ ���ٸ� ���� ��ȯ
			return nullptr;
		}

		template <typename T>
		static std::shared_ptr<T> Load(const std::wstring& _strKey, const std::wstring& _strPath)
		{
			// Ű������ Ž��
			std::shared_ptr<T> resource = Resources::Find<T>(_strKey);
			if (resource != nullptr)
				return resource;

			// �ش� ���ҽ��� ���ٸ�
			resource = std::make_shared<T>();
			if (FAILED(resource->Load(_strPath)))
			{
				assert(false);
				return nullptr;
			}

			resource->SetKey(_strKey);
			resource->SetPath(_strPath);

			{
				std::lock_guard<std::mutex> lock(m_mutex);
				m_mapResources.insert(std::make_pair(_strKey, resource));
			}

			return std::dynamic_pointer_cast<T>(resource);
		}

		template <typename T>
		static void Insert(const std::wstring& _strKey, std::shared_ptr<T> _pResource)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_mapResources.insert(std::make_pair(_strKey, _pResource));
		}

		template <typename T>
		static void Release(const std::wstring& _strKey)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_mapResources.erase(_strKey);
		}
		

	private:
		static std::mutex m_mutex;

		static std::map<std::wstring, std::shared_ptr<Resource>> m_mapResources;
		//���ҽ����� ��쿣 �ε�� ���ҽ��ϳ��� ��ΰ� ���� ������ sharedptr�� ���� ���� �ִ�
		//���ҽ��� ���� �����Ǿ� �׋� ������
	};
}

