#type vertex
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;

void main() 
{
	v_Color = in_Color;
	gl_Position = u_ViewProjection * u_Transform * vec4(in_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 out_Color;

in vec4 v_Color;

void main() 
{
	out_Color = v_Color;
}