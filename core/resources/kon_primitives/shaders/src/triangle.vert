#version 450
#extension GL_EXT_buffer_reference : require

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec2 outUV;

struct Vertex {
	vec3 position;
	float uv_x;
	vec3 normal;
	float uv_y;
	vec4 color;
}; 

layout(buffer_reference, std430) readonly buffer VertexBuffer{ 
	Vertex vertices[];
};

//push constants block
layout( push_constant ) uniform constants {
	mat4 render_matrix;
	VertexBuffer vertexBuffer;
	int drawMode;
} PushConstants;

// drawMode
// 0 = regular draw
// 1 = uv coords
// 2 = normals

void main()  {
	//load vertex data from device adress
	Vertex v =  PushConstants.vertexBuffer.vertices[gl_VertexIndex];

	//output data
	gl_Position = PushConstants.render_matrix * vec4(v.position.xyz, 1.0f);

	if(PushConstants.drawMode == 0) {
		outColor = v.color;
	} else if(PushConstants.drawMode == 1) {
		outColor = vec4(v.uv_x, v.uv_y, 0.0f, 1.0f);
	} else {
		outColor = vec4(v.normal, 1.0f);
	}

	
	outUV.x = v.uv_x;
	outUV.y = v.uv_y;
}

