#pragma once
#include "VertexArray.h"
#include "glm/glm.hpp"

namespace Owl
{
	class RendererApi
	{
	public:
		virtual ~RendererApi() = default;

		enum class Api
		{
			None = 0, OpenGl = 1
		};
	public:
		virtual void SetClearColor(const glm::vec4& pColor) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& pVertexArray) = 0;

		static Api GetApi() { return s_Api; }
	private:
		static Api s_Api;
	};
	
}
