#version 430

out vec4 o_Color;
in vec4 fragPos;
uniform float u_percentage; //percent of level completed

void main()
{
	o_Color = vec4(0.0, 0.75, 0.0, 1.0);

	if(abs(fragPos.x) > 0.995 ||
		abs(fragPos.y) > 0.8)
		o_Color = vec4(0.0, 0.0, 0.0, 1.0);

	if((fragPos.x + 1.0) / 2.0 > u_percentage)
		o_Color = vec4(0.0, 0.0, 0.0, 1.0);
}
