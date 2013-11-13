#version 140

uniform sampler2D water;
uniform sampler2D height;

in vec3 texCoord;

out vec4 fragColor;

void main()
{


	fragColor = vec4(texture2D(water,texCoord.xz).rgb, mix(0.0,0.5,step(texture2D(height,texCoord.xz).r, texCoord.y)));

	
	// Task_1_4 - ToDo End
}	