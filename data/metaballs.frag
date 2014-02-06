#version 150

const float INFINITY = 1e+4;

const int SIZE = 16;
const int MAX_LEVEL = 10;
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
		result += pow(blobs[i].radius/(length(ray.origin+t*ray.direction - blobs[i].position)),5);
	}
	return result;
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
	
	float tNear = 100000.0;
	float tFar =  0.0;
	float tNearSphere = 100000.0;
	float tFarSphere = 0.0;
	bool result = false;
	float minRadius = INFINITY;

	for(int i = 0; i < SIZE; ++i){
		minRadius = mix(blobs[i].radius, minRadius, step(minRadius, blobs[i].radius));
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
	tNear = mix(tNear, tNearSphere, step(100000.0,tNear));
	tFar = mix(tFar, tFarSphere, step(tFar, 0.0));

	int level = 0;
	float nearEnergy = energy(tNear, ray);
	float farEnergy = energy(tFar, ray);
	float tPreNear = tNear;
	float tPreFar = tFar;

	while(level<MAX_LEVEL){
		if(nearEnergy>THRESHOLD && nearEnergy<THRESHOLD+1.0)
			return true;
		if(nearEnergy > 1.0){
			float dummy	= tFar;
			tFar = tNear;
			tNear = min(tNear-(dummy-tNear)/2, minRadius);
		}else{
			if(farEnergy < 1.0){
				tFar -= 2*minRadius;
			}else{
				tNear = (tNear+tFar)/2;
			}
		}
		//if tNear was in and out at least once
		if(sign(tPreNear) != sign(tNear)){
			tFar = max(tNear, tPreNear);
			tNear = min(tNear, tPreNear);
		}

		tPreNear = tNear;
		nearEnergy = energy(tNear, ray);
		farEnergy = energy(tFar, ray);
		++level;
	}

	t = energy(tNear,ray)*100000;
	return result;
}

// Task_5_3 - ToDo End
