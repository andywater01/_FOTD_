#include "EndScreen.h"

EndScreen::EndScreen(std::string name)
	: Scene(name)
{
}

void EndScreen::InitScene(float windowWidth, float windowHeight)
{
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	//Camera Entity
	{
		//Creates Camera Entity
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		//ECS::AttachComponent<HorizontalScroll>(entity);
		//ECS::AttachComponent<VerticalScroll>(entity);
		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Sets up the Identifier
		unsigned int bitHolder = EntityIdentifier::CameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Camera");
		ECS::SetIsMainCamera(entity, true);
	}

	{
		auto entityBack = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entityBack);
		ECS::AttachComponent<Transform>(entityBack);

		//Sets up components
		std::string BackGround = "YouDied.png"; //400, 200
		ECS::GetComponent<Sprite>(entityBack).LoadSprite(BackGround, 380, 200);
		ECS::GetComponent<Transform>(entityBack).SetPosition(vec3(0.f, 0.f, 100.f));

		//Setup up the Identifier
		unsigned int bitHolder = 0x0;
		ECS::SetUpIdentifier(entityBack, bitHolder, "End entity");
	}

}
