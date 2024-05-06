uniform sampler2D diffuse;
varying vec2 texCoordVar;
uniform vec4 color;

void main() {
    vec4 colorPre = texture2D(diffuse, texCoordVar);
	vec4 colorPost;
	colorPost.rgba = vec4(color.r * colorPre.r, color.g * colorPre.g, color.b * colorPre.b, color.a * colorPre.a);
    gl_FragColor = colorPost;
}