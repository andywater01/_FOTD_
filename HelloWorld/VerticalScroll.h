#pragma once
#include "Camera.h"
#include "ECS.h"

class VerticalScroll
{
public:
	VerticalScroll();

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
	float m_scrollCorrection = 1;
};

//Sends vertical scrolling camera TO json file
inline void to_json(nlohmann::json& j, const VerticalScroll& vertScroll)
{
	j["Offset"] = vertScroll.GetOffset();
}

//Sends vertical scrolling camera TO json file
inline void from_json(const nlohmann::json& j, VerticalScroll& vertScroll)
{
	vertScroll.SetOffset(j["Offset"]);

}


