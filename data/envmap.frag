#version 140

uniform sampler2D envmap;
uniform samplerCube cubemap;

uniform int mapping;

out vec4 fragColor;

in vec3 v_eye;

// Task_2_1 - ToDo Begin
// Implement the four requested projection mappings.

 const float c_pi  = 3.14159;

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
		float u = 0.5*atan(eye.x, eye.z)/c_pi;
		float v = 2.0*asin(-eye.y)/c_pi;
		// use texture function with the envmap sampler
		vec4 texColor = texture2D(envmap, vec2(u,v));
		vec4 floorColor = vec4(0.5,0.5,0.7,1.0);

		color = mix(floorColor, texColor, smoothstep(0.0,0.08, eye.y));
	}	
	else if(2 == mapping) 	// paraboloid
	{
		float m = 2.0 + 2.0*eye.y;
		float u = 0.5 + eye.x/m;
		float v = 0.5 + eye.z/m;
		// use texture function with the envmap sampler
		vec4 texColor = texture2D(envmap, vec2(u,v));
		vec4 floorColor = vec4(0.0,0.0,0.0,1.0);

		color = mix(floorColor, texColor, smoothstep(0.0,0.08, eye.y));
	}
	else if(3 == mapping) 	// sphere
	{
		float m = 2.0 *sqrt(eye.x*eye.x + eye.y*eye.y + (1.0-eye.z)*(1.0-eye.z));
		float u = 0.5 + eye.x/m;
		float v = 0.5 - eye.y/m;
		// use texture function with the envmap sampler

		color = texture2D(envmap, vec2(u,v));
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
