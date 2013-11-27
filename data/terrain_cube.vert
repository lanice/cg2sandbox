#version 140

in vec3 a_vertex;

void main()
{
	gl_Position = vec4(a_vertex,1.0);
}
