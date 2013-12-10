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

	if(value < threshold) discard;

	return smoothstep(threshold,0.5,value);
	//return step(0.5,value);
	// return dFdx(value)*150+0.5;
	
	// Task_3_2 - ToDo End
}

void main()
{
	// Task_3_2 - ToDo Begin

	float a = texture(label, v_uv).r;

	//fragColor = vec4(vec3(0.0), aastep(0.45, a));
	fragColor = vec4( vec3(0.0), aastep(0.493,a));
	
	// Task_3_2 - ToDo End
}
