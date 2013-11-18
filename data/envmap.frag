#version 140

uniform sampler2D envmap;
uniform samplerCube cubemap;

uniform int mapping;

out vec4 fragColor;

in vec3 v_eye;

// Task_2_1 - ToDo Begin
// Implement the four requested projection mappings.

 const float c_pi  = 3.1415926535897932384626433832795;

vec4 env(in vec3 eye)
{
	vec4 color;
	
	if(0 == mapping) 		// cube
	{
		// use texture function with the cubemap sampler
		color = texture(cubemap,eye);
	}
	else if(1 == mapping) 	// polar
	{	
		float x = 0.5*atan(eye.x, eye.z)/c_pi;
		float y = 2.0*asin(-eye.y)/c_pi;
		// use texture function with the envmap sampler
		vec4 texColor = texture2D(envmap, vec2(x,y));
		vec4 floorColor = vec4(0.4,0.4,0.4,1.0);

		color = mix(floorColor, texColor, smoothstep(-0.3,0.3, eye.y));
	}	
	else if(2 == mapping) 	// paraboloid
	{
		// use texture function with the envmap sampler
		color = vec4(0.0, 0.0, 1.0, 1.0); // ToDo
	}
	else if(3 == mapping) 	// sphere
	{
		// use texture function with the envmap sampler
		color = vec4(1.0, 1.0, 0.0, 1.0); // ToDo
	}
	return color;
}

// Task_2_1 - ToDo End


void main()
{
	vec3 eye = normalize(v_eye);
	vec4 color = env(eye);
	
	fragColor = color;
}
