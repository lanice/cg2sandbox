#version 140

in vec2 a_vertex;
out vec3 v_eye;
uniform mat4 viewProjectionInverted;

void main()
{
	gl_Position = vec4(a_vertex, 1.0, 1.0);
	v_eye = (viewProjectionInverted * vec4(a_vertex, 1.0, 1.0)).xyz;
}