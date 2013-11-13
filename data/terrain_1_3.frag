#version 140

uniform sampler2D ground;

in vec2 texCoord;
in float color;
in float darkFactor;
out vec4 fragColor;

const int scaleFactor = 2;
const float ambienteDiv = 0.5;
// Task_1_3 - ToDo Begin

// Note: you can specify consts instead of uniforms or local variables...
//const float t = 1.0; 

vec4 realTexCoord(vec2 oldCoord, float height){

	float offset1 = 0.75 - min(height-mod(height,0.25),0.75);
	float offset2 = max(offset1-0.25, 0.0);

	float newX = mod(oldCoord.x*scaleFactor,0.25);
	float newY = mod(oldCoord.y*4*scaleFactor,1.0);

	vec4 coordsAndOffset = vec4(newX, newY, offset1, offset2);
	return coordsAndOffset;
}

void main()
{
	// Implement height based texturing using a texture atlas
	// with known structure ;)
	
	// Tip: start with getting the height from the vertex stage 
	// and think of a function that yields you the two texture "indices".
	// These "indices" can be used as offset when accessing the texture...
	// Tip: again, checkout step, clamp, mod, and smoothstep!

	// ...
	
	// Note: its ok if you habe minimal seams..., you need not to apply
	// mipmaps not to use safe margins to avoid those seems. 
	
	// ...
	
	// Note: feel free to scale the textures as it pleases you.
	
	// ...
	
	//float i = ... ;
	//fragColor = mix(texture2D(ground, uv0), texture2D(ground, uv1), i);
	vec4 coordResult = realTexCoord(texCoord,color);
	vec4 color1 = texture2D(ground, vec2(coordResult.x+coordResult.z, coordResult.y)).rgba;
	vec4 color2 = texture2D(ground, vec2(coordResult.x+coordResult.w, coordResult.y)).rgba;
	vec4 endColor = mix(color1, color2, smoothstep(0.1,0.25,mod(color,0.25)));

	fragColor = vec4((darkFactor*(1.0-ambienteDiv)+ambienteDiv)*endColor);
	
	// Task_1_3 - ToDo End
}