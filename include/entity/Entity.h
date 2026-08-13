//
// Created by komvu on 2024-12-19.
//
#pragma once

#ifndef WINTHER_ENGINE_ENTITY_H
#define WINTHER_ENGINE_ENTITY_H


#include <string>
#include <memory>
#include <utility>
#include <physics/Transform.h>
#include <SDL3/SDL.h>
#include <component/Animation.h>
#include <physics/RigidBody.h>


namespace Modwin
{
	class Properties
	{
		public:
			Properties(SDL_FlipMode flipMode, float x, float y, int width, int height, std::string textureId,
					std::string source) :
					x(x), y(y), flipMode(flipMode), w(width), h(height),
					m_TextureID(std::move(textureId)), m_Source(std::move(source))
			{
			};

			Properties(const Properties&) = default;
			Properties& operator=(const Properties&) = default;

			float x, y;
			SDL_FlipMode flipMode;
			int w, h;
			std::string m_TextureID, m_Source;
	};


	class Entity
	{
		public:
			Entity(std::string tag, size_t id, const Properties& properties);

			Entity(const Entity&) = delete;
			Entity& operator=(const Entity&) = delete;

			virtual const std::string& GetTag() const noexcept;

			virtual void Draw();

			virtual void Update(float dt);

			size_t GetId() const noexcept;

			bool Exists() const noexcept;

			virtual ~Entity() = default;

			std::unique_ptr<RigidBody> m_RigidBody;
			std::unique_ptr<Animation> m_Animation;
			std::unique_ptr<Transform> m_Transform;
			Properties m_Properties;
			const std::string m_TextureID;
			const size_t m_Id;
			const std::string m_Tag;
			bool m_Exists;


//		const Component* m_Component[];

	};


} // Modwin

#endif //WINTHER_ENGINE_ENTITY_H
