#version 430

//Perspective and model view matrix
uniform mat4 u_PerspectiveMat,
			 u_ModelViewMat;

//Input of the position
layout(location = 0) in vec4 pos;

//location in 3D space
out vec4 fragPos;

//Shading
out float o_Shading;

void main()
{
	gl_Position = u_PerspectiveMat * u_ModelViewMat * pos;

	fragPos = pos;

	if(pos.y == 1.0)
		o_Shading = 0.6;
	else if(pos.y == -1.0)
		o_Shading = 1.0;
}
