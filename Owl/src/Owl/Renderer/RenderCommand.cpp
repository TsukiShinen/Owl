#include "opch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGlRendererApi.h"

namespace Owl
{
	Scope<RendererApi> RenderCommand::s_RendererApi = CreateScope<OpenGlRendererApi>();
}
