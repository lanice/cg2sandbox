
#pragma once

#include <QMatrix4x4>
#include <QMap>
#include <QVector3D>

#include "AbstractPainter.h"

class QOpenGLShader;
class QOpenGLShaderProgram;

class Camera;
class ScreenAlignedQuad;
class PatchedTerrain;


typedef struct Quad{
	Quad* content[4];
	float scale;
	QVector3D position;
	unsigned char LOD[4];

	void clearQuad(){
		if(content[0] == nullptr)
			return;
		for (int i=0; i<4; i++){
			 content[i]->clearQuad();
			 delete content[i];
			 content[i] = nullptr;
		}
	}
}Quad;

class Painter : public AbstractPainter
{
public:
    Painter();
    virtual ~Painter();

    virtual bool initialize();
    
    virtual void paint(float timef);
    virtual void resize(
        int width
    ,   int height);

    virtual void update();
    virtual void update(const QList<QOpenGLShaderProgram *> & programs);

    void keyPressEvent(QKeyEvent * event);

protected:

    void bindEnvMaps(GLenum target);
    void unbindEnvMaps(GLenum target);

    void paint_4_1(float timef);

protected:
    QOpenGLShaderProgram * createBasicShaderProgram(
        const QString & vertexShaderFileName
    ,   const QString & fragmentShaderFileName);

    QOpenGLShaderProgram * createBasicShaderProgram(
        const QString & vertexShaderFileName
    ,   const QString & geometryShaderFileName
    ,   const QString & fragmentShaderFileName);

	int calcLOD(float length, QVector3D from, QVector3D to);
	void paintQuad(Quad *root);
	void correctLOD(Quad *root);
    void patchify();
    void patchify(
        float extend
    ,   float x
    ,   float z
    ,   int level
	,   Quad* root);
   
    float height(
        const float x
    ,   const float z) const;

    bool cull(
        const QVector4D & v0
    ,   const QVector4D & v1
    ,   const QVector4D & v2);

    // ...

protected:
    Camera * m_camera;

    ScreenAlignedQuad * m_quad;

    QList<QMatrix4x4> m_transforms;

    QMap<int, QOpenGLShaderProgram *> m_programs;
    QList<QOpenGLShader *> m_shaders;

    PatchedTerrain * m_terrain;

    std::vector<unsigned short> m_heights;

    float m_yScale;
    float m_yOffset;

    GLuint m_height;
    GLuint m_normals;
    GLuint m_diffuse;
    GLuint m_detail;
    GLuint m_detailn;

    bool m_drawLineStrips;
    bool m_debug;

    float m_precission;
    int m_level;
    QVector3D m_cachedEye;
};
