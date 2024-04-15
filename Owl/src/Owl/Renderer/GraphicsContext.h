#pragma once

namespace Owl
{
	class GraphicsContext
	{
	public:
		GraphicsContext() = default;
		
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
	};
	
}
