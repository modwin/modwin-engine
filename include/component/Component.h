#pragma once
#ifndef MODWIN_ENGINE_COMPONENT_H
#define MODWIN_ENGINE_COMPONENT_H

#include <string>
#include <utility>

namespace Modwin
{
	class Component
	{
		const std::string m_tag;

		bool operator()(const Component& c1, const Component& c2) const;

		public:
			Component() = default;

			Component(std::string tag) : m_tag(std::move(tag))
			{
			};

			~Component() = default;
	};
}


#endif //MODWIN_ENGINE_COMPONENT_H
