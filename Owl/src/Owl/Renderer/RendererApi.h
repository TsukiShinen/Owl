#pragma once
#include "VertexArray.h"
#include "glm/glm.hpp"

namespace Owl
{
	class RendererApi
	{
	public:
		enum class Api
		{
			None = 0, OpenGl = 1
		};

		virtual ~RendererApi() = default;

		virtual void Init() = 0;
		virtual void SetViewport(int pI, int pY, uint32_t pWidth, uint32_t pUint32) = 0;

		virtual void SetClearColor(const glm::vec4& pColor) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& pVertexArray) = 0;

		static Api GetApi() { return s_Api; }

	private:
		static Api s_Api;
	};
}
