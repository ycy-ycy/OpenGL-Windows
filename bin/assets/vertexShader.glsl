attribute vec3 pos;
attribute vec3 col;
varying vec3 vCol;

void main() {
	vCol = col;
	gl_Position = vec4(pos, 1.0f);
}