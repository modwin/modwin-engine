//
// Created by komvu on 2024-12-27.
//

#ifndef WINTHER_ENGINE_PLAYER_H
#define WINTHER_ENGINE_PLAYER_H
#include <character/Character.h>
#include <SDL3/SDL.h>



namespace Winther{

	class Player: public Character
	{
	public:

		Player(std::string tag, size_t id, Properties* properties) : Character(std::move(tag), id, properties), m_Tag(tag)
		{
			m_Properties = properties;
			m_Animation = new Animation();
			m_Animation->SetProperties(properties->m_TextureID, 3, 9, 80, properties->flipMode);
			m_Exists = true;
			m_Id = id;
		};
		void Update(float dt);
		void Draw();

		~Player();
		Properties* m_Properties;
		Animation* m_Animation;
		size_t m_Id;
		const std::string m_Tag;
		bool m_Exists;


	private:


	};

}



#endif //WINTHER_ENGINE_PLAYER_H
