
#pragma once

#include <QtGlobal>
#include <QVector3D>

class QVector2D;
class QSize;
class QMatrix4x4;

struct Boundary;

class NavigationMath
{
public:
    static bool validDepth(float depth);

    static void retrieveNearAndFarPlane(
        const QVector3D & camera
    ,   const QVector3D & center
    ,   const Boundary & boundary
    ,   qreal & zNear
    ,   qreal & zFar);

	static const QVector3D rayPlaneIntersection(
		bool & valid
	,	const QVector3D & rnear	///< ray start
	,	const QVector3D & rfar  ///< ray end - used for line direction retrieval
	,	const QVector3D & location = QVector3D(0.f, 0.f, 0.f) ///< location of the plane
	,	const QVector3D & normal = QVector3D(0.f, 1.f, 0.f)); ///< normal of the plane

    //static const QVector3D rayPlaneIntersectionExt(
    //    const QVector3D & rnear ///< ray start
    //,   const QVector3D & rfar  ///< ray end - used for line direction retrieval
    //,   const QVector3D & location = QVector3D(0.f, 0.f, 0.f) ///< location of the plane
    //,   const QVector3D & normal = QVector3D(0.f, 1.f, 0.f)   ///< normal of the plane
    //,   qreal maxDistance = 8.0);

	/** Yields the intersection point of a ray defined by a 2d point to the 
        root with an axis aligned square of side length a
    */
    static const QVector2D raySquareIntersection(
		const QVector2D & point ///< ray points from point to root
	,	qreal length = 1.0);

	static const bool insideSquare(
		const QVector2D & point ///< ray points from point to root
	,	qreal length = 1.0);

	static const QVector2D rayCircleIntersection(
		bool & valid
	,	const QVector2D & origin
	,	const QVector2D & ray
	,	qreal radius = 1.0);

	static const QVector2D xz(const QVector3D & xyz);

	static const qreal angle(
		const QVector3D & a
	,	const QVector3D & b);

	static const bool boundaryVisible(
		const QMatrix4x4 & modelViewProjection
	,	const Boundary & boundary);

	static const bool pointVisible(
		const QMatrix4x4 & modelViewProjection
	,	const QVector3D & point);

    /** Calculates a camera eye position that is either equal or further away
        from the given camera center, ensured to have the given point in view.
    */
	static const QVector3D cameraWithPointInView(
		const QVector3D & eye
	,	const QVector3D & center
	,	const QVector3D & up
	,	qreal verticalFieldOfView
	,	qreal aspect
	,	const QVector3D & point);

	static const qreal distanceToClosestPoint(
		const QVector3D & camera
	,	const QVector3D & center
	,	const QVector3D & point);
};
