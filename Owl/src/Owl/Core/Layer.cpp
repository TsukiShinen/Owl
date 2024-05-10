#include "opch.h"
#include "Layer.h"

namespace Owl
{
	Layer::Layer(std::string pName)
		: m_DebugName(std::move(pName))
	{
	}
}
