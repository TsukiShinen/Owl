#include "opch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGlRendererApi.h"

namespace Owl
{
	RendererApi* RenderCommand::s_RendererApi = new OpenGlRendererApi;
}
