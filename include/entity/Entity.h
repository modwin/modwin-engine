//
// Created by komvu on 2024-12-19.
//
#pragma once

#ifndef WINTHER_ENGINE_ENTITY_H
#define WINTHER_ENGINE_ENTITY_H


#include <string>
#include "component/Component.h"
#include <map>
#include <utility>
#include <physics/Transform.h>
#include <SDL3/SDL.h>
#include <graphics/TextureManager.h>
#include <component/Animation.h>
#include <physics/RigidBody.h>
#include <cfg/Config.h>


namespace Modwin
{
	class Properties
	{
		public:
			Properties(SDL_FlipMode flipMode, float x, float y, int width, int height, std::string textureId,
					std::string source) :
					m_Source(std::move(source)), flipMode(flipMode), x(x), y(y), w(width), h(height),
					m_TextureID(std::move(textureId))
			{
			};


			Properties(const Properties& properties) : x(properties.x), y(properties.y), flipMode(properties.flipMode),
														w(properties.w), m_TextureID(properties.m_TextureID),h(properties.h),
														m_Source(properties.m_Source)
			{
			};

			Properties& operator=(Properties& other)
			{
				return other;
			}

			float x, y;
			SDL_FlipMode flipMode;
			int w, h;
			const std::string m_TextureID, m_Source;
	};


	class Entity
	{
		public:
			Entity(std::string tag, size_t id, const Properties& properties);

			Entity(const Entity& other);

			virtual const std::string& GetTag();

			virtual void Draw();

			virtual void Update(float dt);

			void Clean() const;

			size_t GetId() const;

			bool Exists() const;

			~Entity();

		public:
			RigidBody* m_RigidBody;
			Animation* m_Animation;
			Transform* m_Transform;
			Properties m_Properties;
			const std::string m_TextureID;
			const size_t m_Id;
			const std::string m_Tag;
			bool m_Exists;


//		const Component* m_Component[];

	};


} // Modwin

#endif //WINTHER_ENGINE_ENTITY_H
