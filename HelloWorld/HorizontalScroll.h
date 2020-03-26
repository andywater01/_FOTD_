#pragma once

#include "Camera.h"
#include "ECS.h"

class HorizontalScroll
{
public:
	HorizontalScroll();

	//Makes sure that the camera is following the focus
	void Update();

	//Getters
	Camera* GetCam() const;
	Transform* GetFocus() const;
	float GetOffset() const;

	//Setters
	void SetCam(Camera* cam);
	void SetFocus(Transform* focus);
	void SetOffset(float offset);
	void SetBackground(unsigned int entity);

private:
	Camera* m_cam = nullptr;
	Transform* m_focus = nullptr;

	unsigned int background;

	float m_offset = 0;

	//Scroll correction
	float m_scrollCorrection = 2;
};

//Sends vertical scrolling camera TO json file
inline void to_json(nlohmann::json& j, const HorizontalScroll& horiScroll)
{
	j["Offset"] = horiScroll.GetOffset();
}

//Sends vertical scrolling camera TO json file
inline void from_json(const nlohmann::json& j, HorizontalScroll& horiScroll)
{
	horiScroll.SetOffset(j["Offset"]);
}

