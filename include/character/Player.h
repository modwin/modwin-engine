//
// Created by komvu on 2024-12-27.
//

#ifndef WINTHER_ENGINE_PLAYER_H
#define WINTHER_ENGINE_PLAYER_H
#include "Character.h"
#include <SDL3/SDL.h>
#include <physics/RigidBody.h>
#include <input/InputHandler.h>



namespace Modwin{

	class Player: public Character
	{
	public:

		Player(std::string tag, size_t id, Properties properties) : Character(std::move(tag), id, properties)
		{
			m_Config = new Config;
			m_Properties = properties;
			m_Animation = new Animation();
			m_Animation->SetValues(properties.m_TextureID, 2, 2, 120, properties.flipMode);
			m_RigidBody = new RigidBody();
			m_RigidBody->ApplyFriction({0, 0});
			m_Transform = new Transform(340, 256);

		};
		virtual ~Player(){};
		void Draw() override;
		void Update(float dt) override;
		bool IsWalkingLeft() const;
		bool IsWalkingRight() const;


		Config* m_Config;
	};

}



#endif //WINTHER_ENGINE_PLAYER_H
