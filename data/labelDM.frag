#version 140

uniform sampler2D label;

out vec4 fragColor;

in vec2 v_uv;

float aastep(float threshold, float value) 
{
	// Task_3_2 - ToDo Begin

	// ToDo: implement this function by using 
	// the GLSL functions dFdx and dFdy ;) and smoothstep
	
	// The result should provide a fragment width dependent
	// smoothing, that is independend of the actual viewports
	// dimensions...

	float afwidth = 0.7 * length (vec2(dFdx(value),dFdy(value)));
	if(value < threshold-afwidth) discard;

	return smoothstep(threshold-afwidth,threshold+afwidth,value);
	
	// Task_3_2 - ToDo End
}

void main()
{
// 	float texw = 1000;
// 	float texh = 250;
// 	float oneu = 1.0/texw;
// 	float onev = 1.0/texh;

//   vec2 st = v_uv;
// // Scale texcoords to range ([0,texw], [0,texh])
//   vec2 uv = st * vec2(texw, texh);

//   // Compute texel-local (u,v) coordinates for the four closest texels
//   vec2 uv00 = floor(uv - vec2(0.5)); // Lower left corner of lower left texel
//   vec2 uvlerp = uv - uv00 - vec2(0.5); // Texel-local lerp blends [0,1]

//   // Perform explicit texture interpolation of distance value.
//   // This is required for the split RG encoding of the 8.8 fixed-point value,
//   // and as a bonus it works around the bad texture interpolation precision
//   // in at least some ATI hardware.

//   // Center st00 on lower left texel and rescale to [0,1] for texture lookup
//   vec2 st00 = (uv00 + vec2(0.5)) * vec2(oneu, onev);

//   // Compute interpolated value from four closest 8-bit RGBA texels
//   vec4 D00 = texture(label, st00);
//   vec4 D10 = texture(label, st00 + vec2(oneu, 0.0));
//   vec4 D01 = texture(label, st00 + vec2(0.0, onev));
//   vec4 D11 = texture(label, st00 + vec2(oneu, onev));

//   // Retrieve the B channel to get the original grayscale image
//   vec4 G = vec4(D00.b, D01.b, D10.b, D11.b);
  
//   // Interpolate along v
//   G.xy = mix(G.xz, G.yw, uvlerp.y);
  
//   // Interpolate along u
//   float g = mix(G.x, G.y, uvlerp.x);

//   float c = aastep(0.4, g);
//   // Final fragment color
//   fragColor = vec4(vec3(c, c, c), 1.0); 















	// Task_3_2 - ToDo Begin

	float a = texture(label, v_uv).r;

	fragColor = vec4( vec3(0.0), aastep(0.5,a));
	
	// Task_3_2 - ToDo End
	
}
