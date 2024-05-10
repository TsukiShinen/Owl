#type vertex
#version 330 core

layout(location = 0) in vec3 in_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(in_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 out_Color;

uniform vec4 u_Color;

void main() 
{
	out_Color = u_Color;
}