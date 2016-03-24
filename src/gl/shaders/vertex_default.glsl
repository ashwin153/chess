#ersion 330 core
in vec4 vertex_position;
uniform mat4 model, view;
uniform vec3 light_position;

void main() {
	gl_Position = view * model * vertex_position;
	
}
