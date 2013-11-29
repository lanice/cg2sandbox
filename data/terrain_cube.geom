#version 140

// Task_2_3 - ToDo Begin

uniform mat4 transforms[6];
uniform mat4 viewProjectionInverted;
//...
in vec2 v_texCoord[3];
in float v_color[3];
in float v_darkFactor[3];
out vec2 texCoord;
out float color;
out float darkFactor;

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

void main()
{
	//for (int layer = 0; layer < 6; layer++){
        gl_Layer = 0;	
        for (int i = 0; i < 3; i++) {
        	texCoord = v_texCoord[i];
        	color = v_color[i];
        	darkFactor = v_darkFactor[i];
            gl_Position = (transforms[0])*gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 1;
        
        for (int i = 0; i < 3; i++) {
        	texCoord = v_texCoord[i];
        	color = v_color[i];
        	darkFactor = v_darkFactor[i];
            gl_Position = (transforms[1])*gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 2;
        
        for (int i = 0; i < 3; i++) {
        	texCoord = v_texCoord[i];
        	color = v_color[i];
        	darkFactor = v_darkFactor[i];
            gl_Position = (transforms[2])*gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 3;
        
        for (int i = 0; i < 3; i++) {
        	texCoord = v_texCoord[i];
        	color = v_color[i];
        	darkFactor = v_darkFactor[i];
            gl_Position = (transforms[3])*gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 4;
        
        for (int i = 0; i < 3; i++) {
        	texCoord = v_texCoord[i];
        	color = v_color[i];
        	darkFactor = v_darkFactor[i];
            gl_Position = (transforms[4])*gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();

        gl_Layer = 5;
        
        for (int i = 0; i < 3; i++) {
        	texCoord = v_texCoord[i];
        	color = v_color[i];
        	darkFactor = v_darkFactor[i];
            gl_Position = (transforms[5])*gl_in[i].gl_Position;
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
