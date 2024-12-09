#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

vec3 palette(in float t)
{
	vec3 a = vec3(.5, .5, .5);
	vec3 b = vec3(.5, .5, .5);
	vec3 c = vec3(.8, .8, .5);
	vec3 d = vec3(0., .2, .5);
	return a + b * cos(6.28318 * (c * t + d));
}

uniform float u_time;
uniform vec2 u_resolution;

void main(void)
{
	vec2 m = vec2(sin(u_time / 4.), -cos(u_time / 5.73));
	float v = cos(u_time * 3.) * 16.;

	vec2 coord = vec2(-1.4 + m.x / 9., 0.04 * m.y);
	float zoom = 10. + m.y * 5.;

	vec2 uv = (gl_FragCoord.xy * 2. - u_resolution.xy) / u_resolution.y;
	uv = uv / zoom + coord;

	float it = 0.;
	vec2 xy = vec2(0., 0.);
	float max_iteration = 80. + v;
	for (float iteration = 0.; iteration < 80. + 16.; iteration += 1.)
	{
		if (dot(xy, xy) > 4. || iteration >= max_iteration) break;
		xy = vec2(xy.x * xy.x - xy.y * xy.y, 2. * xy.x * xy.y) + uv;
		it = iteration;
	}
	float d = log(it) / log(max_iteration);
	vec3 color = palette(1. - d);
	color = pow(color, vec3(1.5));

	if (it > max_iteration - 1.) color = vec3(0.);

	gl_FragColor = vec4(color, 1.);
}
 
