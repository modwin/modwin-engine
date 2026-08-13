#include "component/Component.h"

namespace Modwin{



	bool Component::operator()(const Component& c1, const Component& c2) const
	{
		return c1.m_tag == c2.m_tag;
	}

}
