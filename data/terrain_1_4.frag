#version 140

uniform sampler2D ground;
uniform sampler2D caustics;
uniform float time;

in vec2 texCoord;
in float color;
in float darkFactor;
out vec4 fragColor;

const int scaleFactor = 2;
const int scaleFactorCaustics = 15;
const float ambienteDiv = 0.5;

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
	vec4 coordResult = realTexCoord(texCoord,color);
	vec4 color1 = texture2D(ground, vec2(coordResult.x+coordResult.z, coordResult.y)).rgba;
	vec4 color2 = texture2D(ground, vec2(coordResult.x+coordResult.w, coordResult.y)).rgba;
	vec4 endColor = mix(color1, color2, smoothstep(0.1,0.25,mod(color,0.25)));

	vec4 terrainColor = vec4((darkFactor*(1.0-ambienteDiv)+ambienteDiv)*endColor);
	
	float timeOffset = mod(int(time * 600), 32)/32.0;
	float causticsX = timeOffset + mod(texCoord.x*scaleFactorCaustics/32.0, 1.0/32.0);
	float causticsY = mod(texCoord.y*scaleFactorCaustics,1.0);
	vec4 causticColor = texture2D(caustics, vec2(causticsX,causticsY));
	fragColor = mix(terrainColor, causticColor, 0.3*step(color, 0.2));
}