#include "WAnimation.h"
#include "WTime.h"
#include "WAnimator.h"

#include "WConstantBuffer.h"
namespace W
{
	Animation::Animation():
		Resource(enums::eResourceType::Animation),
		m_pAnimator(nullptr),
		m_vecSprites{},
		m_iIndex(-1),
		m_fTime(0.f),
		m_bComplete(false)
	{

	}
	Animation::~Animation()
	{

	}
	void Animation::Update()
	{

	}
	void Animation::LateUpdate()
	{
		if (m_bComplete)
			return;

		m_fTime += Time::DeltaTime();

		if (m_vecSprites[m_iIndex].fDuration <= m_fTime)
		{
			++m_iIndex;
			m_fTime = 0.f;

			if (m_vecSprites.size() <= m_iIndex)
			{
				m_iIndex = m_vecSprites.size() - 1;
				m_bComplete = true;
			}
		}
	}
	void Animation::Render()
	{

	}
	void Animation::Create(std::wstring _strName,  Vector2 _vLeftTop, 
		Vector2 _vSize, UINT _iColumnLength, Vector2 _vDivisionSize, Vector2 _vOffset, float _fDuration)
	{
		
	
		for (UINT i = 0; i < _iColumnLength; ++i)
		{
			//전부 uv좌표 기준으로
			Sprite tSprite = {};
			
			tSprite.fDuration = _fDuration;
	
			m_vecSprites.push_back(tSprite);
	
		}
	}
	
	void Animation::Reset()
	{
		m_fTime = 0.f;
		m_bComplete = false;
		m_iIndex = 0;
	}
}