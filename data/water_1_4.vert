#version 140

uniform mat4 transform;
uniform sampler2D water;

in vec3 a_vertex;

out vec3 texCoord;

void main()
{

	texCoord = a_vertex + vec3(0.0,0.2,0.0);
	gl_Position = transform * vec4(texCoord,1.0);

}
