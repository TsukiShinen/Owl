#type vertex
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main() 
{
	v_TexCoord = in_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(in_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 out_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform float u_TilingFactor;
uniform vec4 u_Color;

void main() 
{
	out_Color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
}