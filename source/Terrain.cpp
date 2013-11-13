
#include <vector>
#include <cassert>

#include "OpenGLFunctions.h"

#include "Terrain.h"


Terrain::Terrain(
	unsigned short size
,	OpenGLFunctions & gl)
: m_vertices(QOpenGLBuffer::VertexBuffer)
, m_indices(QOpenGLBuffer::IndexBuffer)
{
	m_vao.create();
	m_vao.bind();

	m_vertices.create();
	m_vertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

	m_indices.create();
	m_indices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    // Task_1_1 - ToDo Begin

    strip(size, m_vertices, m_indices, -1);

    // Configure your Vertex Attribute Pointer based on your vertex buffer (mainly number of coefficients ;)).

    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    gl.glEnableVertexAttribArray(0);

    // Task_1_1 - ToDo End

    m_vao.release();
}

void Terrain::draw(
	OpenGLFunctions & gl
,	const GLenum mode)
{
    // Task_1_1 - ToDo Begin

    // Setup the OpenGL state appropriate to the given and your personal drawing requirements.
    // You might take depth tests, primitive restart, and culling into account.

    // gl.glEnable(...
    // ...
    gl.glEnable(GL_COLOR_ARRAY);
    gl.glEnable(GL_VERTEX_ARRAY);
    gl.glEnable(GL_PRIMITIVE_RESTART);

    gl.glPrimitiveRestartIndex(-1);

    m_vao.bind();
    gl.glDrawElements(mode, m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.release();


    // Remember to "clean up" the state after rendering.
    gl.glDisable(GL_COLOR_ARRAY);
    gl.glDisable(GL_VERTEX_ARRAY);
    gl.glDisable(GL_PRIMITIVE_RESTART);

    // gl.glDisable(...
    // ...

    // Task_1_1 - ToDo End
}

void Terrain::strip(
	const unsigned short size
,	QOpenGLBuffer & vertices
,	QOpenGLBuffer & indices
,	const GLuint primitiveRestartIndex)
{
    // Task_1_1 - ToDo Begin

    // perhaps, check for pointless parameter input

    // tips: probably use some for loops to fill up a std or qt container with 
    // floats or other structs, fitting the purpose. further, remember that 
    // gradually summing up lots of small floats might lead to major inaccuracies ...

    // ...

    std::vector<float> *verticeVector = new std::vector<float>;
    verticeVector->reserve(size*size*3);
    for(ushort ix=0; ix<size; ++ix)
        for(ushort iy=0; iy<size; ++iy){
            verticeVector->push_back(float(ix)/float(size-1));
            verticeVector->push_back(0.f);
            verticeVector->push_back(float(iy)/float(size-1));
        }

	vertices.bind();
	vertices.allocate(verticeVector->data(), sizeof(float)*verticeVector->size());
	// ...


    std::vector<uint> *indexVector = new std::vector<uint>;
    indexVector->reserve((size-1)*(1+2*size));

    for(uint ix=0; ix<uint(size-1); ++ix){
        for(uint iy=0; iy<uint(size); ++iy){
            indexVector->push_back(ix+iy*size);
            indexVector->push_back(ix+iy*size+1);
        }
        indexVector->push_back(primitiveRestartIndex);
    }

	indices.bind();
	indices.allocate(indexVector->data(),sizeof(uint)*indexVector->size());

    //Task_1_1 - ToDo End
}
