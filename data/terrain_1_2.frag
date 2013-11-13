#version 140

out vec4 fragColor;

// Task_1_2 - ToDo Begin

in float color;

void main()
{
	// Apply a procedural texture here.

	// Tip: start with getting the height from the vertex stage 
	// and think of a function that yields iso lines.
	// Tip: checkout step, clamp, and mod

	float lineStep = 0.1;
	float lineWidth = 0.01;

	fragColor = mix(
		//white - darkblue - black
		vec4(vec2(0.5+abs(0.5-pow(color,3))-color),1.0-color,1.0),
		vec4(vec3(color),1.0),
		step(lineWidth, mod(color, lineStep))
	);

	// Task_1_2 - ToDo End 
}