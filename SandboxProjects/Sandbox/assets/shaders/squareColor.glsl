#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ProjectionViewMatrix;
uniform mat4 u_ModelMatrix;

out vec3 v_Position;


void main(){
	v_Position = a_Position;
	gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;


void main(){
	color = vec4(0.8, 0.2, 0.3, 1.0);
}