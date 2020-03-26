#include "VerticalScroll.h"

VerticalScroll::VerticalScroll()
{
}

void VerticalScroll::Update()
{
	auto height = 1500; //ECS::GetComponent<Sprite>(background).GetHeight();
	float topScrollLimit = (m_cam->GetOrthoSize().z - m_cam->GetOrthoSize().y) / 2 + (height / 2 - m_scrollCorrection);
	float bottomScrollLimit = (m_cam->GetOrthoSize().z - m_cam->GetOrthoSize().y) / -2 - (height / 2 - m_scrollCorrection);

	//Reset camera position within scroll limit
	//Top
	if (m_focus->GetPosition().y > topScrollLimit)
		m_cam->SetPositionY(topScrollLimit);
	//Bottom
	if (m_focus->GetPosition().y < bottomScrollLimit)
		m_cam->SetPositionY(bottomScrollLimit);

	//Above focus
	if (m_focus->GetPosition().y > (m_cam->m_localPosition.y + m_offset))
	{
		//Calculate the amount the focus has "pushed" the camera UP by
		float difference = m_focus->GetPosition().y - (m_cam->m_localPosition.y + m_offset);

		//Adjust the camera
		if (m_cam->GetPosition().y < topScrollLimit)
		{
			float newY = m_cam->GetPositionY() + difference;
			vec3 newPos = vec3(m_cam->GetPosition().x, newY, m_cam->GetPosition().z);
			m_cam->SetPosition(newPos);
		}
	}
	//Below focus
	if (m_focus->GetPosition().y < (m_cam->m_localPosition.y - m_offset))
	{
		//Calculate the amount the focus has "pushed" the camera DOWN by
		float difference = m_focus->GetPosition().y - (m_cam->m_localPosition.y - m_offset);

		//Adjust the camera
		if (m_cam->GetPosition().y > bottomScrollLimit)
		{
			float newY = m_cam->GetPositionY() + difference;
			vec3 newPos = vec3(m_cam->GetPosition().x, newY, m_cam->GetPosition().z);
			m_cam->SetPosition(newPos);
		}
	}
}

Camera* VerticalScroll::GetCam() const
{
	return m_cam;
}

Transform* VerticalScroll::GetFocus() const
{
	return m_focus;
}

float VerticalScroll::GetOffset() const
{
	return m_offset;
}

void VerticalScroll::SetCam(Camera* cam)
{
	m_cam = cam;
}

void VerticalScroll::SetFocus(Transform* focus)
{
	m_focus = focus;
}

void VerticalScroll::SetOffset(float offset)
{
	m_offset = offset;
}

void VerticalScroll::SetBackground(unsigned int entity)
{
	background = entity;
}
