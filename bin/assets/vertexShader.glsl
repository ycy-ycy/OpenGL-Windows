in vec3 pos;
in vec3 col;
in vec2 texCoord;

out vec3 vCol;
out vec2 vTexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
	vCol = col;
	vTexCoord = texCoord;
	vTexCoord.y *= -1;
	gl_Position = projMat * viewMat * modelMat * vec4(pos, 1.0);
}