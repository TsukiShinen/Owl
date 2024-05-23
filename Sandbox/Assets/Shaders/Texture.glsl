#type vertex
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in float in_TexIndex;
layout(location = 4) in float in_TilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

void main() 
{
	v_Color = in_Color;
	v_TexCoord = in_TexCoord;
	v_TexIndex = in_TexIndex;
	v_TilingFactor = in_TilingFactor;
	gl_Position = u_ViewProjection * vec4(in_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 out_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Texture[32];

void main() 
{
	out_Color = texture(u_Texture[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
}