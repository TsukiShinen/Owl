#pragma once

namespace Owl
{
	enum class RendererApi
	{
		None = 0,
		OpenGL = 1,
	};

	class Renderer
	{
	public:
		static RendererApi GetApi() { return s_RendererApi; }
	private:
		static RendererApi s_RendererApi;
	};
	
}
