#version 140

uniform samplerCube source;
uniform samplerCube cubemap;

uniform float timef;

out vec4 fragColor;

in vec3 v_normal;
in vec3 v_eye;

// Task_2_3 - ToDo Begin
void main()
{
	vec3 n = normalize(v_normal);
	vec3 e = normalize(v_eye);
	
	vec3 r = reflect(-e,n);
	vec3 q = refract(-e,n,1.1);
	
	float frsl = smoothstep(0.5,1.5,length(e+r));

	vec4 refl = texture(source,r);
	vec4 refr = texture(source,q);
	
	fragColor =  refl;//mix(refl,refr,frsl);
}

// Task_2_3 - ToDo End