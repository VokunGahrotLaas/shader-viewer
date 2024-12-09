#ifdef GL_ES
precision mediump float;
#endif

#define TWO_PI 6.28318530718

uniform vec2 u_resolution;
uniform float u_time;


void main(){
	vec2 uv = gl_FragCoord.xy / u_resolution.xy;
	gl_FragColor = vec4(uv,0.5+0.5*sin(u_time),1.0);
}
