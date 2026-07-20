//
// Created by komvu on 2024-12-27.
//

#include "character/Player.h"

namespace Winther
{


	void Player::Update(float deltaTime)
	{
		m_RigidBody->ResetForce();
		m_RigidBody->ResetFriction();
		m_Transform->TranslateX(m_RigidBody->GetPosition().X);
		m_Transform->TranlateY(m_RigidBody->GetPosition().Y);
		m_Animation->Update();
		m_RigidBody->Update(deltaTime);
		if (m_RigidBody->GetVelocity().X == 0 && m_RigidBody->GetVelocity().Y == 0)
		{
//			m_RigidBody->ApplyForceX(0);
//			m_RigidBody->ApplyForceY(0);
			m_Animation->SetValues("player", 2, 2, 250);
		}



		if (IsWalkingRight())
		{
			if(IsWalkingLeft())
				return;
			m_RigidBody->ResetForce();

			m_RigidBody->ApplyForceX(3);
			m_Animation->SetValues("player", 3, 9, 70);
			m_RigidBody->Update(deltaTime);

			return;
		}

		if (IsWalkingLeft())
		{
			m_RigidBody->ResetForce();

			m_RigidBody->ApplyForceX(-2);
			m_Animation->SetValues("player", 1, 9, 60);
			m_RigidBody->Update(deltaTime);
			return;
		}

		if (InputHandler::GetInstance()->GetKeyDown(m_Config->GetMoveUpKey()))
		{
			if(InputHandler::GetInstance()->GetKeyDown(m_Config->GetMoveDownKey()))
				return;
			m_RigidBody->ResetForce();
			m_RigidBody->ApplyForceY(-2);
			m_Animation->SetValues("player", 0, 9, 80);
			m_RigidBody->Update(deltaTime);
			return;
		}

		if (InputHandler::GetInstance()->GetKeyDown(m_Config->GetMoveDownKey()))
		{
			m_RigidBody->ResetForce();
			m_RigidBody->ApplyForceY(3);
			m_Animation->SetValues("player", 2, 9, 80);
			m_RigidBody->Update(deltaTime);
			return;
		}

//		m_Transform->TranslateX(m_RigidBody->GetPosition().x);
//		m_Transform->TranlateY(m_RigidBody->GetPosition().y);
//		m_Animation->Update();
//		m_RigidBody->Update(deltaTime);
	}


//	Player::Player(std::string tag, size_t id, Properties properties)
//	{
//		m_TextureID = id;
//	};
	void Player::Draw()
	{
//		SDL_Log("Player is being drawn with properties X = %f Y = %f texture = %s, source = %s , H = %d, W = %d", m_Properties.x, m_Properties.y, m_TextureID.c_str(), m_Properties.m_Source.c_str(), m_Properties.h, m_Properties.w );
		if (m_Exists)
		{
			m_Animation->Draw(m_Transform->GetX(), m_Transform->GetY(), m_Properties.w, m_Properties.h);
		}
	}

	bool Player::IsWalkingLeft() const
	{
		return InputHandler::GetInstance()->GetKeyDown(m_Config->GetMoveLeftKey());
	}

	bool Player::IsWalkingRight() const
	{
		return InputHandler::GetInstance()->GetKeyDown(m_Config->GetMoveRightKey());
	}
}
