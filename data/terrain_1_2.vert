#version 140

// Task_1_2 - ToDo Begin

uniform mat4 transform;
uniform sampler2D height;

in vec3 a_vertex;

out float color;

void main()
{
	// Here you need to gather texture coordinates.
	// Tip: note that your terrain might/should be in range [0,1] which 
	// accidentally could be interpreted as texture coordinates ...
	
	// use texture2D function to access a sampler2D object
	
	// ...

	color = texture2D(height, a_vertex.xz).r;

	gl_Position = transform * vec4(a_vertex+vec3(0.0,color,0.0), 1.0);
	
	// Task_1_2 - ToDo End
}
