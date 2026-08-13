#ifndef MODWIN_ENGINE_PLAYER_H
#define MODWIN_ENGINE_PLAYER_H

#include "Character.h"
#include <memory>
#include <physics/RigidBody.h>
#include <cfg/Config.h>



namespace Modwin{

	class Player: public Character
	{
	public:

		Player(std::string tag, size_t id, Properties properties) : Character(std::move(tag), id, properties)
		{
			m_Animation = std::make_unique<Animation>();
			m_Animation->SetValues(properties.m_TextureID, 2, 2, 120, properties.flipMode);
			m_RigidBody = std::make_unique<RigidBody>();
			m_RigidBody->ApplyFriction({0, 0});

		};
		~Player() override = default;
		void Draw() override;
		void Update(float dt) override;
		bool IsWalkingLeft() const;
		bool IsWalkingRight() const;


		Config m_Config;
	};

}



#endif //MODWIN_ENGINE_PLAYER_H
