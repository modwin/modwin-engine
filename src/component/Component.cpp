//
// Created by komvu on 2024-12-20.
//

#include "component/Component.h"

namespace Winther{



	bool Component::operator()(const Component& c1, const Component& c2) const
	{
		return c1.m_tag == c2.m_tag;
	}

}
