#version 140

uniform sampler2D label;

out vec4 fragColor;

in vec2 v_uv;

float aastep(float threshold, float value) 
{
	// Task_3_2 - ToDo Begin

	// ToDo: implement this function by using 
	// the GLSL functions dFdx and dFdy ;) and smoothstep
	
	// The result should provide a fragment width dependent
	// smoothing, that is independend of the actual viewports
	// dimensions...

	float afwidth = 0.7 * length (vec2(dFdx(value),dFdy(value)));
	if(value < threshold-afwidth) discard;

	return smoothstep(threshold-afwidth,threshold+afwidth,value);
	
	// Task_3_2 - ToDo End
}

void main()
{ 
	// Task_3_2 - ToDo Begin

	float a = texture(label, v_uv).r;

	fragColor = vec4( vec3(0.0), aastep(0.497,a));
	
	// Task_3_2 - ToDo End
}
