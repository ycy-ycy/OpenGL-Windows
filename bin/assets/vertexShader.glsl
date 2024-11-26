attribute vec3 pos;
attribute vec3 col;
attribute vec2 texCoord;

varying vec3 vCol;
varying vec2 vTexCoord;

void main() {
	vCol = col;
	vTexCoord = texCoord;
	vTexCoord.y *= -1;
	gl_Position = vec4(pos, 1.0);
}