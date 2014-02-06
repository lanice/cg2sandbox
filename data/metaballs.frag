#version 150

const float INFINITY = 1e+4;

const int SIZE = 3;
const int MAX_LEVEL = 40;
const float THRESHOLD = 0.66;

struct Sphere
{
	vec3 position;
	float radius;
};

struct Material
{
	vec4 sr; // vec3 specular, float reflectance
	vec4 dr; // vec3 diffuse, float roughness
};

struct Ray
{
	vec3 origin;
	vec3 direction;
};


Sphere blobs[16];
Material materials[16];

void cache(
	sampler2D positions
,	sampler2D materials0
,	sampler2D materials1)
{
	for(int i = 0; i < SIZE; ++i)
	{
		ivec2 uv = ivec2(i % 4, i / 4);
	
		vec4 v = texelFetch(positions, uv, 0);
		
		Sphere blob;
		blob.position = v.xyz;
		blob.radius = v.w;

		blobs[i] = blob;
		
		Material mat;
		mat.sr = texelFetch(materials0, uv, 0);
		mat.dr = texelFetch(materials1, uv, 0);
		
		materials[i] = mat;
	}
}

// sphere intersection
bool intersect(
    const in Sphere blob
,   const in Ray ray // needs to be normalized!
,   out float t0
,   out float t1)
{
	// Task_5_1 - ToDo Begin
	// implement a ray sphere intersection

	// hint: the case of single intersection with sphere can be neglected, so
	// only t > 0.0 intersections are relevant (if you like you can also
	// account for t == 0.0 intersections, which should have a rare occurrence).

	// hint: use ray.origin, ray.direction, blob.position, blob.radius
	// hint: make it fast (if you like)! ;)

	// intersection of a sphere (x²+y²+z²=r²)
	//   (first we shifted the sphere to (0,0,0))
	// and a line (vecX = vecOrigin + t*vecDirection):
	// substitude x, y and z and resolve for t.
	// you will get the formula below 

	vec3 rayOffset = ray.origin - blob.position;
	float a = dot(ray.direction, ray.direction);
	float b = 2*dot(ray.direction, rayOffset);
	float c = dot(rayOffset, rayOffset) - pow(blob.radius,2);

	// no solutions
	if (pow(b,2) < 4*a*c)
		return false;

	float d = sqrt(pow(b,2)-4*a*c);
	float divi = 2*a;
	t0 = (-b-d)/divi;
	t1 = (-b+d)/divi;
	
	// sphere in wrong direction
	return (t0>0) && (t1>0);

	// Task_5_1 - ToDo End
}

bool rcast(in Ray ray, out vec3 normal, out Material material, out float t)
{	
    // Task_5_1 - ToDo Begin
	
	// return normal for the nearest intersected sphere, as well as
	// the intersection parameter t (that should allow to retrieve the 
	// itnersection point I = ray.origin + ray.direction * t).
	
	// the function should return true, if at least one sphere was hit, 
	// and false if no when no sphere was hit.
		
	// (Task_5_2 - ToDo return material of nearest intersected sphere)

	t =  INFINITY;

	for(int i = 0; i < SIZE; ++i)
	{
		float t0;
		float t1;

		if(intersect(blobs[i], ray, t0, t1) && (t0<t))
		{
			t = t0;
			normal = normalize(ray.origin+t0*ray.direction-blobs[i].position);
			material = materials[i];
		}
	}

	return t<INFINITY;
	
	// ToDo: End Task 5_1
}


// Task_5_3 - ToDo Begin

// ... your helper functions

// ... more ...

float energy(float t, Ray ray){
	float result = 0.0;
	for(int i = 0; i < SIZE; ++i){
		vec3 r = ray.origin+t*ray.direction - blobs[i].position;
		float q = dot(r,r);
		if(q == 0) return 2.0;
		result += blobs[i].radius/q;
	}
	return result;
}

void interp(in vec3 pos, out vec3 normal, out Material material){
	material.sr = vec4(0.0);
	material.dr = vec4(0.0);
	normal = vec3(0.0);

	float energyFactor = 0.0;

	for(int i = 0; i < SIZE; ++i){
		vec3 r = pos - blobs[i].position;
		float q = dot(r,r);
		float factor = blobs[i].radius/q;
		material.sr += factor*materials[i].sr;
		material.dr += factor*materials[i].dr;
		normal += factor*(normalize(pos-blobs[i].position));
		energyFactor += factor;
	}
	normal = normalize(normal);
	material.sr /= energyFactor;
	material.dr /= energyFactor;
}

bool trace(in Ray ray, out vec3 normal, out Material material, out float t)
{
	// Task_5_3 - ToDo Begin

	// find nearest and farthest intersection for all metaballs 
	// hint: use for loop, INFINITE, SIZE, intersect, min and max...

	// ...
	
	// implement raymarching within your tmin and tmax 
	// hint: e.g., use while loop, THRESHOLD, and implment yourself
	// an attribute interpolation function (e.g., interp(pos, normal, material, actives?))
	// as well as a summation function (e.g., sum(pos))
	
	// your shader should terminate!

	// return true if iso surface was hit, fals if not
	
	float tNear = INFINITY;
	float tFar =  0.0;
	float tNearSphere = INFINITY;
	float tFarSphere = 0.0;
	bool result = false;

	for(int i = 0; i < SIZE; ++i){
		float t0;
		float t1;

		// distance of sphere from origin in ray direction
		float tSphereDist = dot(normalize(ray.direction), blobs[i].position-ray.origin)/length(ray.direction);

		// bring in the radius
		float tMaybeNearer = tSphereDist-blobs[i].radius/length(ray.direction);
		float tMaybeFarther = tSphereDist+blobs[i].radius/length(ray.direction);
		tNearSphere = mix(tNearSphere, tMaybeNearer, step(tMaybeNearer, tNearSphere));
		tFarSphere = mix(tMaybeFarther, tFarSphere, step(tMaybeFarther, tFarSphere));

		if(intersect(blobs[i], ray, t0, t1)){
			tFar = mix(t1, tFar, step(tNear, t0));
			tNear = mix(t0, tNear, step(tNear, t0));
		}
	}

	//if a sphere was intersected, take intersecion points 
	// else take nearest and farthest z-component (relative to ray) of spheres
	tNear = mix(tNear, tNearSphere, step(INFINITY,tNear));
	tFar = mix(tFar, tFarSphere, step(tFar, 0.0));
	if(tFar == tNear)
		tFar += 0.0001;

	tNear = 0.0;


	float tDist;
	for (int i = 0; i < 2; ++i)
	{
		if (tNear == tFar) break;

		tDist = (tFar-tNear)/MAX_LEVEL;

		// step further until we are in a blob
		while(tNear < tFar && energy(tNear, ray) < 1.0) tNear += tDist;

		// range limitation
		tFar = tNear;

		// step into opposite direction (with much more granular steps) until we are out again
		while(energy(tNear, ray) > 1.0) tNear -= tDist/MAX_LEVEL;
	}

	// check if we hit a blob
	if (tNear < tFar) result = true;

	t = tNear;
	interp(ray.origin+t*ray.direction, normal, material);
	return result;
}

// Task_5_3 - ToDo End
