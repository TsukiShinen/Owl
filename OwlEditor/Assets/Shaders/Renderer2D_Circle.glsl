#type vertex
#version 450 core

layout(location = 0) in vec3 in_WorldPosition;
layout(location = 1) in vec3 in_LocalPosition;
layout(location = 2) in vec4 in_Color;
layout(location = 3) in float in_Thickness;
layout(location = 4) in float in_Fade;
layout(location = 5) in int in_EntityId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_EntityId;

void main()
{
	Output.LocalPosition =in_LocalPosition;
	Output.Color = in_Color;
	Output.Thickness = in_Thickness;
	Output.Fade = in_Fade;

	v_EntityId = in_EntityId;

	gl_Position = u_ViewProjection * vec4(in_WorldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 out_Color;
layout(location = 1) out int out_EntityId;

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_EntityId;

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.LocalPosition);
    float circle = smoothstep(0.0, Input.Fade, distance);
    circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    out_Color = Input.Color;
	out_Color.a *= circle;

	out_EntityId = v_EntityId;
}