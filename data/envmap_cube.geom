#version 140

// Task_2_3 - ToDo Begin

uniform mat4 transforms[6];
in vec3 v_eye[3];
//...

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 g_eye;

void main()
{
	for (int layer = 0; layer < 6; layer++) {
        gl_Layer = layer;
        for (int i = 0; i < 3; i++) {
            gl_Position = transforms[layer]*gl_PositionIn[i];
            g_eye = v_eye[i];
            EmitVertex();
        }
        EndPrimitive();
    }
	// ToDo: for each cubemap face

	// use the gl_Layer to set the current cube map face to render to
	
	// retrieve the g_eye vector and pass to fragment stage
	
	// set  gl_Position, the input is available via 
	// gl_in[0].gl_Position to gl_in[2].gl_Position
	
	// finish up each vertex with EmitVertex();
	// and each primitive with EmitPrimitivie();
}

// Task_2_3 - ToDo End
