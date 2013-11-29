#version 140

// Task_2_3 - ToDo Begin

uniform mat4 transforms[6];
uniform mat4 viewProjectionInverted;
//...

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

out vec3 g_eye;

void main()
{
	//for (int layer = 0; layer < 6; layer++){
        gl_Layer = 0;	
        for (int i = 0; i < 3; i++) {
            gl_Position = gl_in[i].gl_Position;
			g_eye = (inverse(transforms[0])*vec4(gl_in[i].gl_Position)).xyz;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 1;
        
        for (int i = 0; i < 3; i++) {
            gl_Position = gl_in[i].gl_Position;
			g_eye = (inverse(transforms[1])*vec4(gl_in[i].gl_Position)).xyz;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 2;
        
        for (int i = 0; i < 3; i++) {
            gl_Position = gl_in[i].gl_Position;
			g_eye = (inverse(transforms[2])*vec4(gl_in[i].gl_Position)).xyz;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 3;
        
        for (int i = 0; i < 3; i++) {
            gl_Position = gl_in[i].gl_Position;
			g_eye = (inverse(transforms[3])*vec4(gl_in[i].gl_Position)).xyz;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 4;
        
        for (int i = 0; i < 3; i++) {
            gl_Position = gl_in[i].gl_Position;
			g_eye = (inverse(transforms[4])*vec4(gl_in[i].gl_Position)).xyz;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 5;
        
        for (int i = 0; i < 3; i++) {
            gl_Position = gl_in[i].gl_Position;
			g_eye = (inverse(transforms[5])*vec4(gl_in[i].gl_Position)).xyz;
            EmitVertex();
        }
        EndPrimitive();
    //}
	// ToDo: for each cubemap face

	// use the gl_Layer to set the current cube map face to render to
	
	// retrieve the g_eye vector and pass to fragment stage
	
	// set  gl_Position, the input is available via 
	// gl_in[0].gl_Position to gl_in[2].gl_Position
	
	// finish up each vertex with EmitVertex();
	// and each primitive with EmitPrimitivie();
}

// Task_2_3 - ToDo End
