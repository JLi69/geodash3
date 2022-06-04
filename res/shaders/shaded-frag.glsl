#version 430

uniform vec4 u_Color;
out vec4 o_Color;

//Fragment location in 3D space
in vec4 fragPos;

in float o_Shading;

//Texture coordinate
in vec2 o_texCoord;

layout(binding = 0) uniform sampler2D samp;

void main()
{
	o_Color = u_Color * o_Shading;
	o_Color = texture(samp, o_texCoord) * o_Shading;
	o_Color.a = 1.0;

	//Outline the cube
	if(abs(fragPos.x) > 0.95 && abs(fragPos.y) > 0.95 ||
		abs(fragPos.y) > 0.95 && abs(fragPos.z) > 0.95 ||
		abs(fragPos.x) > 0.95 && abs(fragPos.z) > 0.95)
		o_Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
