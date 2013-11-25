#version 140

// Task_1_3 - ToDo Begin

uniform mat4 transform;
uniform sampler2D height;
uniform float timef;

in vec3 a_vertex;

out vec2 texCoord;
out float color;
out float darkFactor;

const float softShadowFactor = 0.015;

void main()
{
	// Note: should be similar to 1_2 vertex shader.
	// In addition, you need to pass the texture coords 
	// to the fragment shader...

	// ...

	texCoord = a_vertex.xz;
	color = texture2D(height, texCoord).r;

	vec3 vectorX = normalize(vec3(softShadowFactor, color - texture2D(height,texCoord+vec2(softShadowFactor,0.0)).r, 0.0));
	vec3 vectorZ = normalize(vec3(0.0, color - texture2D(height,texCoord+vec2(0.0,softShadowFactor)).r, softShadowFactor));
	vec3 rotVec = vec3(cos(timef*40),1.0,sin(timef*40));
	darkFactor = dot(cross(vectorZ,vectorX), normalize(rotVec));


	gl_Position = transform * vec4(a_vertex+vec3(0.0,color,0.0), 1.0);
	
	// Task_1_3 - ToDo End
}
