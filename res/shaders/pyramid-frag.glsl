#version 430

//Color that gets passed into the shader
uniform vec4 u_Color;

//Color to be outputted
out vec4 o_Color;

//location of the fragment in 3D space
in vec4 fragPos;

in float o_Shading;

//Texture stuff
in vec2 o_texCoord;
layout(binding = 1) uniform sampler2D samp2;

void main()
{
	o_Color = u_Color * o_Shading;
	o_Color = texture(samp2, o_texCoord) * o_Shading;

	//Shading on edge of pyramid
	if(fragPos.x + 0.5 + fragPos.z + 0.5 <= fragPos.y + 0.3 &&
		fragPos.x + 0.5 + fragPos.z + 0.5 >= fragPos.y - 0.3 ||

		-fragPos.x + 0.5 + fragPos.z + 0.5 <= fragPos.y + 0.3 &&
		-fragPos.x + 0.5 + fragPos.z + 0.5 >= fragPos.y - 0.3 ||
		
		fragPos.x + 0.5 - fragPos.z + 0.5 <= fragPos.y + 0.3 &&
		fragPos.x + 0.5 - fragPos.z + 0.5 >= fragPos.y - 0.3 ||

		-fragPos.x + 0.5 - fragPos.z + 0.5 <= fragPos.y + 0.3 &&
		-fragPos.x + 0.5 - fragPos.z + 0.5 >= fragPos.y - 0.3 ||

		fragPos.y < -0.8) 
		o_Color *= 0.5;
	
	o_Color.a = 1.0;	
}
