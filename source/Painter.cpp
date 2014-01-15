
#include <cassert>

#include <QKeyEvent>
#include <QVector4D>
#include <QRectF>
#include <QPolygonF>
#include <qmath.h>

#include "Terrain.h"
#include "FileAssociatedShader.h"
#include "FileAssociatedTexture.h"
#include "Camera.h"
#include "Canvas.h"
#include "PatchedTerrain.h"

#include "ScreenAlignedQuad.h"

#include "Painter.h"

namespace
{
}

static const float pScale = 1.4f;

Painter::Painter()
: m_camera(nullptr)
, m_quad(nullptr)
, m_terrain(nullptr)
, m_height (-1)
, m_normals(-1)
, m_diffuse(-1)
, m_detail (-1)
, m_detailn(-1)
, m_drawLineStrips(false)
, m_precission(1.f)
, m_level(0)
, m_debug(false)
{
    setMode(PaintMode0);
}

Painter::~Painter()
{
    qDeleteAll(m_programs);
    qDeleteAll(m_shaders);

    FileAssociatedTexture::clean(*this);
}

bool Painter::initialize()
{

    initializeOpenGLFunctions();

    // Note: As always, feel free to change/modify parameters .. as long as its possible to 
    // see the terrain and navigation basically works, everything is ok. 

    glClearColor(1.f, 1.f, 1.f, 0.f);

    m_quad = new ScreenAlignedQuad(*this);

    QOpenGLShaderProgram * program;

    program = createBasicShaderProgram("data/terrain_4_1.vert", "data/terrain_4_1.frag");
    m_programs[PaintMode1] = program;
    program->bindAttributeLocation("a_vertex", 0);
    program->link();

    // load resources - goto https://moodle.hpi3d.de/course/view.php?id=58 and download either
    // the Tree Canyon Terrain or the complete Terrain Pack. Feel free to modify/exchange the data and extends/scales/offsets.

    m_height = FileAssociatedTexture::getOrCreate2Dus16("data/tree_canyon_h.raw", 2048, 2048, *this, &m_heights);
    m_yOffset = -1.f;
    m_yScale  =  3.f;

    m_normals = FileAssociatedTexture::getOrCreate2D("data/tree_canyon_n.png", *this);
    m_diffuse = FileAssociatedTexture::getOrCreate2D("data/tree_canyon_c.png", *this);
    m_detail  = FileAssociatedTexture::getOrCreate2D("data/moss_detail.png",   *this, GL_REPEAT, GL_REPEAT);
    m_detailn = FileAssociatedTexture::getOrCreate2D("data/moss_detail_n.png", *this, GL_REPEAT, GL_REPEAT);
    //m_detailc = FileAssociatedTexture::getOrCreate2D("data/moss-detail-c.png", *this, GL_REPEAT, GL_REPEAT);

    // Task_4_1 - ToDo Begin

    // Pick your maximum tree traversal depth/level (or recursion depth)
    // This should limit the minimum extend of a single patch in respect to the terrain 
    // and your approach (4x4 or 2x2 subdivisions)
    m_level = 3;
    // Pick a starting LOD for your patches (the enumeration still remains on 0, 1, and 2, but the
    // with each starting level, the resulting tiles are subdivided further, resulting in more detailed tiles.
    m_terrain = new PatchedTerrain(2, *this);

    // Task_4_1 - ToDo End

    glClearColor(0.6f, 0.8f, 0.94f, 1.f);

    return true;
}


void Painter::keyPressEvent(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_L:
        m_drawLineStrips = !m_drawLineStrips;
        update();
        break;
    case Qt::Key_C:
        m_debug = !m_debug;
        update();
        break;
    case Qt::Key_P:
        m_precission *= (event->modifiers() && Qt::Key_Shift ? 1.1f : 1.f / 1.1f);
        patchify();
        break;
    default:
        break;
    }
}


QOpenGLShaderProgram * Painter::createBasicShaderProgram(
    const QString & vertexShaderFileName
    , const QString & fragmentShaderFileName)
{
    QOpenGLShaderProgram * program = new QOpenGLShaderProgram();

    m_shaders << FileAssociatedShader::getOrCreate(
        QOpenGLShader::Vertex, vertexShaderFileName, *program);
    m_shaders << FileAssociatedShader::getOrCreate(
        QOpenGLShader::Fragment, fragmentShaderFileName, *program);
    program->bindAttributeLocation("a_vertex", 0);
    program->link();

    return program;
}

QOpenGLShaderProgram * Painter::createBasicShaderProgram(
    const QString & vertexShaderFileName
    , const QString & geometryShaderFileName
    , const QString & fragmentShaderFileName)
{
    QOpenGLShaderProgram * program = new QOpenGLShaderProgram();

    m_shaders << FileAssociatedShader::getOrCreate(
        QOpenGLShader::Vertex, vertexShaderFileName, *program);
    m_shaders << FileAssociatedShader::getOrCreate(
        QOpenGLShader::Geometry, geometryShaderFileName, *program);
    m_shaders << FileAssociatedShader::getOrCreate(
        QOpenGLShader::Fragment, fragmentShaderFileName, *program);
    program->bindAttributeLocation("a_vertex", 0);
    program->link();

    return program;
}

void Painter::resize(
    int width
    , int height)
{
    glViewport(0, 0, width, height);
    update();
}

void Painter::update()
{
    update(m_programs.values());
}

void Painter::update(const QList<QOpenGLShaderProgram *> & programs)
{
    foreach(const int i, m_programs.keys())
    {
        QOpenGLShaderProgram * program(m_programs[i]);

        if (programs.contains(program) && program->isLinked())
        {
            program->bind();

            switch (i)
            {
            case PaintMode0:
            case PaintMode9:
            case PaintMode8:
            case PaintMode7:
            case PaintMode6:
            case PaintMode5:
            case PaintMode4:
            case PaintMode3:
            case PaintMode2:
            case PaintMode1:

                if (!m_debug || camera()->eye() != m_cachedEye)
                {
                    m_cachedEye = camera()->eye();
                    patchify();
                }

                program->setUniformValue("mvp", camera()->viewProjection());
                program->setUniformValue("view", camera()->view());

                program->setUniformValue("yScale",  m_yScale);
                program->setUniformValue("yOffset", m_yOffset);

                program->setUniformValue("height",  0);
                program->setUniformValue("normals", 1);
                program->setUniformValue("diffuse", 2);
                program->setUniformValue("detail",  3);
                program->setUniformValue("detailn", 4);
                break;

            //case OtherProgram: // feel free to use more than one program per mode...
            //    break;
            }

            program->release();
        }
    }
}

void Painter::paint(float timef)
{
    switch (mode())
    {
    case PaintMode0:
    case PaintMode1:
        paint_4_1(timef); break;
    case PaintMode2:
    case PaintMode3:
    case PaintMode4:
        //paint_1_4(timef); break;
        //case PaintMode5:
        //    paint_1_5(timef); break;
        // ...
    default:
        break;
    }
}

// returns the height of the terrain at x, z with respect to the vertical 
// scale and offset used by the vertex shader and the terrains extend of 8.0

float Painter::height(
    const float x
,   const float z) const
{
    float y = static_cast<float>(m_heights[2048 * qBound<int>(0, 2048 * (z * 0.125 + 0.5f), 2047) + qBound<int>(0, 2048 * (x * 0.125 + 0.5), 2047)]);
    y /= 65536.f;

    return y * m_yScale + m_yOffset;
}

// Note: Feel free to remove your old code and start on minor cleanups and refactorings....

void Painter::patchify()
{
    m_terrain->drawReset();

    // Task_4_1 - ToDo Begin

    // Here you should initiate the "patchification" 

    // You can modify the signature of patchify as it pleases you.
    // This function is called whenever the camera changes.

	Quad *quadTreeRoot = new Quad;

	quadTreeRoot = new Quad;
	for(int i=0; i<4; i++)
		quadTreeRoot->content[i] = nullptr;

    patchify(8.f, 0.f, 0.f, 0, quadTreeRoot);
	correctAllLOD(quadTreeRoot);
	paintQuad(quadTreeRoot);
	
	quadTreeRoot->clearQuad();
	delete quadTreeRoot;

    // Task_4_1 - ToDo End
}

bool Painter::cull(
    const QVector4D & point1,
    const QVector4D & point2,
    const QVector4D & point3,
    const QVector4D & point4
    )
{
    // Task_4_1 - ToDo Begin
    
    // This function should return true, if the tile specified by vertices v0, v1, and v2 
    // is within the cameras view frustum.

    // Hint: you might try to use NDCs and transfer the incomming verticies appropriatelly.
    // With that in mind, it should be simpler to cull...

    // If you like, make use of QVector3D, QVector4D (toVector3DAffine), QPolygonF (boundingRect), and QRectF (intersects)

    // Task_4_1 - ToDo End

    QVector4D screenPos1 = camera()->viewProjection() * point1;
    float x1 = screenPos1.x()/screenPos1.w();
    float y1 = screenPos1.y()/screenPos1.w();
    float z1 = screenPos1.z()/screenPos1.w();
    QVector4D screenPos2 = camera()->viewProjection() * point2;
    float x2 = screenPos2.x()/screenPos2.w();
    float y2 = screenPos2.y()/screenPos2.w();
    float z2 = screenPos2.z()/screenPos2.w();
    QVector4D screenPos3 = camera()->viewProjection() * point3;
    float x3 = screenPos3.x()/screenPos3.w();
    float y3 = screenPos3.y()/screenPos3.w();
    float z3 = screenPos3.z()/screenPos3.w();
    QVector4D screenPos4 = camera()->viewProjection() * point4;
    float x4 = screenPos4.x()/screenPos4.w();
    float y4 = screenPos4.y()/screenPos4.w();
    float z4 = screenPos4.z()/screenPos4.w();

    return (
        (x1 > 1.f && x2 > 1.f && x3 > 1.f && x4 > 1.f) ||
        (x1 < -1.f && x2 < -1.f && x3 < -1.f && x4 < -1.f) ||
        (y1 > 1.f && y2 > 1.f && y3 > 1.f && y4 > 1.f) ||
        (y1 < -1.f && y2 < -1.f && y3 < -1.f && y4 < -1.f) ||
        (z1 > 1.f && z2 > 1.f && z3 > 1.f && z4 > 1.f) ||
        (z1 < 0.f && z2 < 0.f && z3 < 0.f && z4 < 0.f)
        );


    // return (cullPoint(screenPos1) && cullPoint(screenPos2) && cullPoint(screenPos3) && cullPoint(screenPos4));
}

void Painter::paintQuad(Quad *root){
	// if leaf draw
	if(root->content[0] == nullptr){
        if (!cull(QVector4D(root->position.x() - root->scale/2.f, height(root->position.x(), root->position.y()), root->position.z() - root->scale/2.f, 1.f),
                  QVector4D(root->position.x() - root->scale/2.f, height(root->position.x(), root->position.y()), root->position.z() + root->scale/2.f, 1.f),
                  QVector4D(root->position.x() + root->scale/2.f, height(root->position.x(), root->position.y()), root->position.z() - root->scale/2.f, 1.f),
                  QVector4D(root->position.x() + root->scale/2.f, height(root->position.x(), root->position.y()), root->position.z() + root->scale/2.f, 1.f))
            )
		    m_terrain->drawPatch(root->position, root->scale, root->LOD[0], root->LOD[1], root->LOD[2], root->LOD[3]);
    }else{
        // if not leaf draw all subPatches
        for(int i=0;i<4;i++)
            paintQuad(root->content[i]);
    }
}

// compares LODs of neighboured patches 
// (left and right is bottom and top when horizontal line comparison)
void Painter::correctLOD(Quad *right, Quad *left, bool vertical){
	// index of bottom-right patch
	int BRPatch;
	// index of LOD-triangle (0-bottom, 1-right, 2-top, 3-left)
	int rightTriIndex;

	if(vertical){
		// compare right LOD of left path with left LOD of right patch
		BRPatch = 0;
		rightTriIndex = 1;
	}else{
		// compare top LOD of bottom path with bottom LOD of top patch
		// rotatet 90° clockwise => BR is now at index 1
		BRPatch = 1;
		// right tile is, where bottom was (0)
		rightTriIndex = 0;
	}
	// if left is not devided further
	if(left->content[0] == nullptr){
		// if right is not devided further
		if(right->content[0] == nullptr){
			// if scale of neighboured patches is different
			if(left->scale > right->scale){
				// index 0 is never in use 
				// (when patch with LOD 2 is divided it has to be 1 to keep resolution)
				// compare left and right LOD
				right->LOD[rightTriIndex+2] = 1;
				left->LOD[rightTriIndex] = 2;
			}else{ 
				if (left->scale < right->scale){
					right->LOD[rightTriIndex+2] = 2;
					left->LOD[rightTriIndex] = 1;
				}
			}
		}else{
			// compare LODs of left subPatches of right with LODs of left
			correctLOD(right->content[BRPatch + 1], left, vertical);
			correctLOD(right->content[BRPatch + 2], left, vertical);
		}	
	// if left is devided further
	}else{
		// if right is not devided further
		if(right->content[0] == nullptr){
			// compare LODs of right subPatches of left with LODs of right
			correctLOD(right, left->content[BRPatch], vertical);
			correctLOD(right, left->content[(BRPatch + 3)%4], vertical);
		}else{
			// compare LODs of right subPatches of left with LODs of left subPatches of right
			correctLOD(right->content[BRPatch + 1], left->content[BRPatch], vertical);
			correctLOD(right->content[BRPatch + 2], left->content[(BRPatch + 3)%4], vertical);
		}
	}
}

void Painter::correctAllLOD(Quad *root){
	// if leaf (patch) => nothing to correct (no eges in it)
	if (root->content[0] == nullptr)
		return;

	// correct LODs on edges in all subPatches
	for(int i=0; i<4; i++)
		correctAllLOD(root->content[i]);
	
	// vertical edge correction
	// bottom-right <-> bottom-left
	correctLOD(root->content[0], root->content[1], true);
	// top-right <-> top-left
	correctLOD(root->content[3], root->content[2], true);

	// horizontal edge correction
	// bottom-right <-> top-right
	correctLOD(root->content[0], root->content[3], false);
	// bottom-left <-> top-left
	correctLOD(root->content[1], root->content[2], false);
}

float Painter::distToPatch(QVector3D &p1, QVector3D &p2){
	QVector3D direction = p2 - p1;
	return(
		qMax(
			QVector3D::crossProduct(
				m_cachedEye - p1,
				direction
			).length() / direction.length(),
			qMin((p1 - m_cachedEye).length(),(p2 - m_cachedEye).length())
		)
	);
}

// calculate LOD (returns -1 when patch has do be devided)
int Painter::calcLOD(float length, QVector3D &from, QVector3D &to){
	float dist =  distToPatch(from, to);
	float epsilon = pScale * length / dist;
	
	if (epsilon>1.f)
		return -1;

	return qCeil(epsilon*2);
}

void Painter::patchify(
    const float extend
,   const float x
,   const float z
,   const int level
,   Quad *root)
{
    // Task_4_1 - ToDo Begin

    // Use an ad-hoc or "static" approach where you decide to either 
    // subdivide the terrain patch and continue with the resulting
    // children (2x2 or 4x4), or initiate a draw call of patch.
    // For the draw call the LODs for all four tiles are required.
    // For skipping a tile (e.g., because of culling), use LOD = 3.

    // This functions signature suggest a recursive approach.
    // Feel free to implement this in any way with as few or as much traversals
    // /passes/recursions you need.

    // Checkt out the paper "Seamless Patches for GPU-Based Terrain Rendering"
	
	// calculate vertices of patch
	float newX = x - 4.f;
	float newZ = z - 4.f;
	QVector3D br(newX,0.f,newZ);
	QVector3D bl(newX+extend,0.f,newZ);
	QVector3D tr(newX,0.f,newZ+extend);
	QVector3D tl(newX+extend,0.f,newZ+extend);
	
	// calculate LOD for each tile of the patch
	int lodB = calcLOD(extend,bl,br);
	int lodR = calcLOD(extend,tr,br);
	int lodT = calcLOD(extend,tl,tr);
	int lodL = calcLOD(extend,tl,bl);

	// if level is low enough for a devide
	// and one LOD is < 0 (patch has to be devided)
    if (level < m_level &&
		(  lodB < 0
		|| lodR < 0
		|| lodT < 0
		|| lodL < 0))
    {
		// generate 4 new patches
		for(int i=0; i<4; i++){
			Quad *q = new Quad;
			for(int j=0; j<4; j++)
				q->content[j] = nullptr;
			root->content[i] = q;
		}

		// BR, BL, TL, TR
		patchify(extend/2.f, x, z, level+1, root->content[0]);
		patchify(extend/2.f, x+extend/2.f, z, level+1, root->content[1]);
		patchify(extend/2.f, x+extend/2.f, z+extend/2.f, level+1, root->content[2]);
		patchify(extend/2.f, x, z+extend/2.f, level+1, root->content[3]);
    }else{
		// give patch its contents
		root->position = QVector3D(x-(8.f-extend)/2.f, 0.0, z-(8.f-extend)/2.f);
		root->scale = extend;
		root->LOD[0] = lodB;
		root->LOD[1] = lodR;
		root->LOD[2] = lodT;
		root->LOD[3] = lodL;
    }

    // Task_4_1 - ToDo End
}

void Painter::paint_4_1(float timef)
{
    QOpenGLShaderProgram * program(m_programs[PaintMode1]);

    if (program->isLinked())
    {
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_height);

        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_normals);

        glActiveTexture(GL_TEXTURE2);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_diffuse);

        glActiveTexture(GL_TEXTURE3);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_detail);

        glActiveTexture(GL_TEXTURE4);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_detailn);

        m_terrain->draw(*this, *program, m_drawLineStrips);

        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
