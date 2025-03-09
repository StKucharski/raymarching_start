#version 330 core

uniform float u_time; 
uniform vec2 u_resolution;
out vec4 FragColor;

const int MAX_STEPS = 500;
const float MAX_DIST = 100.0;
const float EPSILON = 0.001;

float sphereSDF(vec3 p, float r){
	return length(p) - r;
}

float sdf(vec3 p){
	return min(sphereSDF(p - vec3(1.0, 1.0, 1.0), 1.0), sphereSDF(p - vec3(0.0,0.0,0.0), 1.0));
}

float raymarch(vec3 ro, vec3 rd) {
	float t = 0.0;
	for(int i = 0; i < MAX_STEPS; i++) {
		vec3 p = ro + rd * t;
		float d = sdf(p);
		if(d < EPSILON) return t;
		t += d;
		if(t > MAX_DIST) return -1.0;
	}
	return -1.0;
}

vec3 get_ray_direction(vec2 uv, vec3 ro, vec3 target, float fov) {
	vec3 forward = normalize(target - ro);
	vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), forward));
	vec3 up = cross(forward, right);
	float focalLength = 1.0 / tan(fov * 0.5);
	vec3 rd = normalize(uv.x * right + uv.y * up + focalLength * forward);
	return rd;
}

vec3  normal (vec3 p){
	return p * ( sdf(p) - sdf(p - EPSILON) ) / EPSILON;
}

float anglebetween(vec3 a ,vec3 b){
	return acos(dot(a,b) / (length(a) * length(b)));
}

void main() {
	vec2 uv = (gl_FragCoord.xy / u_resolution.xy) * 2.0 - 1.0;
	uv.x *= u_resolution.x / u_resolution.y;

	vec3 ro = vec3(0.0, 0.0, -5.0);    // Camera Position
	vec3 target = vec3(1.0, 1.0, 1.0); // Sphere Position
	float fov = 1.0;

	vec3 rd = get_ray_direction(uv, ro, target, fov);
	float t = raymarch(ro, rd);

	if(t > 0.0) {
		vec3 col = vec3(1.0, 0.4, 0.2);
		col *= anglebetween(ro, normal(ro + t*rd))/3.15;
		FragColor = vec4(col, 1.0);
	} else {
		FragColor = vec4(0.0, 2.0, 0.0, 1.0);
	}
}

