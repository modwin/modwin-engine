//
// Created by komvu on 2024-12-20.
//
#pragma once
#ifndef WINTHER_ENGINE_COMPONENT_H
#define WINTHER_ENGINE_COMPONENT_H

#include <string>
#include <utility>
#include "entity/Entity.h"
namespace Winther
{

	class Component
	{
	private:

		const std::string m_tag;
		bool operator() (const Component& c1, const Component& c2) const;


	public:
		Component() = default;

		Component(std::string tag): m_tag(std::move(tag)){};

		~Component() = default;





	};


}




#endif //WINTHER_ENGINE_COMPONENT_H
