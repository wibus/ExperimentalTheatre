#include "StdChoreographer.h"

#include "Shape/Circle.h"
#include "Shape/Polygon.h"
#include "Hardware/Hardware.h"

#include <cassert>
using namespace std;
using namespace cellar;


namespace prop2
{
    StdChoreographer::StdChoreographer() :
        _dt(real(0)),
        _gravity(real(0), real(0)),
        _circles(),
        _polygons(),
        _maxHandledDeltaTime(real(0.1)),
        _correctionPercentage(real(0.65)),
        _correctionSlop(real(0.1))
    {
    }

    StdChoreographer::~StdChoreographer()
    {
    }

    void StdChoreographer::setup()
    {
    }

    void StdChoreographer::reset()
    {
        _circles.clear();
        _polygons.clear();
    }

    void StdChoreographer::update(real dt)
    {
        _dt = dt;

        if(_dt == real(0))
        {
            // No time elapsed,
            // no need to update
            return;
        }

        if(_dt > _maxHandledDeltaTime)
        {
            // To much time elpased,
            // would make the simulation unstable
            return;
        }

        size_t nbCircles = _circles.size();
        size_t nbPolygons = _polygons.size();

        // Update positions of all shapes
        for(size_t i=0; i<_circles.size(); ++i)
        {
            updateShape(_circles[i]);
        }
        for(size_t i=0; i<_polygons.size(); ++i)
        {
            updateShape(_polygons[i]);
        }


        // Resolve collisions of dynamic shapes
        // Order is important;
        // A shape should never trigger a resolveXY() with a preceding shape :
        //  * Circles precede Polygons
        //  * Lower indices precede higher indices
        std::vector< std::shared_ptr<StdCollisionReport> > collisionReports;
        std::shared_ptr<StdCollisionReport> report;

        for(size_t master=0; master < nbCircles; ++master)
        {
            shared_ptr<Circle>& masterCircle = _circles[master];

            if(masterCircle->bodyType() == EBodyType::DYNAMIC)
            {
                for(size_t slave = 0; slave < nbCircles; ++slave)
                {
                    const std::shared_ptr<Circle>& slaveCircle = _circles[slave];
                    if(slaveCircle->bodyType() != EBodyType::GRAPHIC &&
                       (slaveCircle->bodyType() == EBodyType::KINEMATIC ||
                        master < slave))
                    {
                        report = detectCircleCircle(masterCircle, slaveCircle);
                        if(report->areColliding)
                            collisionReports.push_back(report);
                    }
                }

                for(size_t slave = 0; slave < nbPolygons; ++slave)
                {
                    const std::shared_ptr<Polygon>& slavePolygon = _polygons[slave];
                    if(slavePolygon->bodyType() != EBodyType::GRAPHIC)
                    {
                        report = detectCirclePolygon(masterCircle, slavePolygon);
                        if(report->areColliding)
                            collisionReports.push_back(report);
                    }
                }
            }
        }

        for(size_t master=0; master < nbPolygons; ++master)
        {
            shared_ptr<Polygon>& masterPolygon = _polygons[master];

            if(masterPolygon->bodyType() == EBodyType::DYNAMIC)
            {
                for(size_t slave = 0; slave < nbCircles; ++slave)
                {
                    const std::shared_ptr<Circle>& slaveCircle = _circles[slave];
                    if(slaveCircle->bodyType() == EBodyType::KINEMATIC)
                    {
                        report = detectCirclePolygon(slaveCircle, masterPolygon);
                        if(report->areColliding)
                            collisionReports.push_back(report);
                    }
                }

                for(size_t slave = 0; slave < nbPolygons; ++slave)
                {
                    const std::shared_ptr<Polygon>& slavePolygon = _polygons[slave];
                    if(slavePolygon->bodyType() != EBodyType::GRAPHIC &&
                       (slavePolygon->bodyType() == EBodyType::KINEMATIC ||
                        master < slave))
                    {
                        report = detectPolygonPolygon(masterPolygon, slavePolygon);
                        if(report->areColliding)
                            collisionReports.push_back(report);
                    }
                }
            }
        }


        // Resolve found collisions
        int nbReports = static_cast<int>(collisionReports.size());
        for(int i=0; i < nbReports; ++i)
        {
            moveApart(collisionReports[i]);
            resolveCollision(collisionReports[i]);
        }
    }

    void StdChoreographer::updateShape(
        const std::shared_ptr<AbstractShape>& shape)
    {
        if(shape->bodyType() == EBodyType::DYNAMIC)
        {
            // Apply the gravitationnal acceleration to dynamic shapes
            shape->addLinearAcceleration(_gravity);
        }

        // Update the velocities of the shape
        shape->addLinearVelocity(shape->linearAcceleration()   * _dt);
        shape->addAngularVelocity(shape->angularAcceleration() * _dt);

        // Reset the accelerations
        shape->setLinearAcceleration(Vec2r());
        shape->setAngularAcceleration(real(0.0));

        // Apply friction
        Vec3r linearFriction = shape->linearFrictionCoefficients();
        if(linearFriction)
        {
            Vec2r linearVelocityDir = shape->linearVelocity();
            if(linearVelocityDir)
            {
                real  linearVelocityLen2 = linearVelocityDir.length2();
                real  linearVelocityLen = sqrt(linearVelocityLen2);
                linearVelocityDir /= linearVelocityLen;

                real linearDecelerationMagnitude =
                        linearFriction[0] +
                        linearFriction[1] * linearVelocityLen +
                        linearFriction[2] * linearVelocityLen2;
                linearDecelerationMagnitude *= _dt;

                if(linearDecelerationMagnitude <  linearVelocityLen)
                {
                    shape->addLinearVelocity(
                        -linearVelocityDir * linearDecelerationMagnitude);
                }
                else
                {
                    shape->setLinearVelocity(Vec2r(real(0), real(0)));
                }
            }
        }
        Vec3r angularFriction = shape->angularFrictionCoefficients();
        if(angularFriction)
        {
            real angularVelocity = shape->angularVelocity();
            if(angularVelocity)
            {
                real angularVelocity2 = angularVelocity * angularVelocity;
                real angularVelocityDir = sign(angularVelocity);
                angularVelocity = absolute(angularVelocity);

                real angularDecelerationMagnitude =
                        angularFriction[0] +
                        angularFriction[1] * angularVelocity +
                        angularFriction[2] * angularVelocity2;
                angularDecelerationMagnitude *= _dt;

                if(angularDecelerationMagnitude < angularVelocity)
                {
                    shape->addAngularVelocity(
                        -angularVelocityDir * angularDecelerationMagnitude);
                }
                else
                {
                    shape->setAngularVelocity(real(0));
                }
            }
        }

        // Move the shape
        shape->moveBy(shape->linearVelocity()  * _dt);
        shape->rotate(shape->angularVelocity() * _dt);
    }

    void StdChoreographer::moveApart(
        const std::shared_ptr<StdCollisionReport>& report)
    {
        AbstractShape* shape1 = report->shape1.get();
        AbstractShape* shape2 = report->shape2.get();

        Vec2r n = report->contactNormal;
        real pen = report->penetrationDepth;

        real iM1 = shape1->inverseMass();
        real iM2 = shape2->inverseMass();

        real correction = _correctionPercentage *
                          maxVal(pen - _correctionSlop, real(0.0)) /
                          (iM1 + iM2);
        shape1->moveBy( n * (iM1 * correction));
        shape2->moveBy(-n * (iM2 * correction));
    }

    void StdChoreographer::resolveCollision(
        const std::shared_ptr<StdCollisionReport>& report)
    {
        AbstractShape* shape1 = report->shape1.get();
        AbstractShape* shape2 = report->shape2.get();

        // Get tangent space coordinate system
        Vec2r n = report->contactNormal;
        Vec2r t = perpCW(n);

        // Get distance of shapes' centroid from contact point
        Vec2r p = report->contactPoint;
        Vec2r r1 = p - shape1->centroid();
        Vec2r r2 = p - shape2->centroid();

        // Get shapes' inertia
        real iM1 = shape1->inverseMass();
        real iM2 = shape2->inverseMass();
        real iI1 = shape1->inverseMomentOfInertia();
        real iI2 = shape2->inverseMomentOfInertia();

        // Get shapes' velocities
        Vec2r v1 = shape1->linearVelocity();
        Vec2r v2 = shape2->linearVelocity();
        real w1 = shape1->angularVelocity();
        real w2 = shape2->angularVelocity();

        // Get bounce coefficient
        real e = minVal(shape1->hardware()->bounciness(),
                        shape2->hardware()->bounciness());


        // Compute normal impulse //
        real vn1 = dot(v1, n);
        real vn2 = dot(v2, n);
        real rn1 = cross(r1, n);
        real rn2 = cross(r2, n);

        real jn = (1 + e);
        jn *= ((vn2 + w2*rn2) - (vn1 + w1*rn1));
        jn /= (iM1 + iM2 + rn1*rn1*iI1 + rn2*rn2*iI2);


        // A negative impulse means that the shapes are already moving apart
        // No need to apply any impulse in that case
        if(jn < real(0.0)) return;


        // Compute tangent impulse //
        real vt1 = dot(v1, t);
        real vt2 = dot(v2, t);
        real rt1 = cross(r1, t);
        real rt2 = cross(r2, t);

        real jt = real(1.0);
        jt *= ((vt2 + w2*rt2) - (vt1 + w1*rt1));
        jt /= (iM1 + iM2 + rt1*rt1*iI1 + rt2*rt2*iI2);


        // Compute the final tangent impulse assuming the fact that it is
        // limited by the shapes' static coefficients of friction and normal force
        real us = shape1->hardware()->staticFrictionCoefficient() *
                  shape2->hardware()->staticFrictionCoefficient();
        real absoluteJt = absolute(jt);
        if(jn*us < absoluteJt)
        {
            real ud = shape1->hardware()->dynamicFrictionCoefficient() *
                      shape2->hardware()->dynamicFrictionCoefficient();
            jt *= (jn / absoluteJt) * ud;
        }


        // Apply the final impulse
        Vec2r j = t*jt + n*jn;
        shape1->applyImpulseAt( j, p);
        shape2->applyImpulseAt(-j, p);
    }

    std::shared_ptr<StdCollisionReport> StdChoreographer::detectCircleCircle(
        const std::shared_ptr<Circle>& circle1,
        const std::shared_ptr<Circle>& circle2)
    {
        std::shared_ptr<StdCollisionReport> report(new StdCollisionReport());

        real r1 = circle1->radius();
        real r2 = circle2->radius();

        real minDistLen = r1 + r2;
        real minDistLen2 = minDistLen*minDistLen;

        Vec2r dist = circle1->centroid() - circle2->centroid();
        real distLen2 = dist.length2();

        // Return now if the shapes do not collide
        if(minDistLen2 < distLen2)
            return report;

        real distLen = sqrt(distLen2);
        real radiusRatio2 = r2 / (r1+r2);

        report->areColliding = true;
        report->shape1 = circle1;
        report->shape2 = circle2;
        report->penetrationDepth = minDistLen - distLen;
        report->contactNormal = dist / distLen;
        report->contactPoint =
            circle2->centroid() + report->contactNormal * distLen * radiusRatio2;

        return report;
    }

    std::shared_ptr<StdCollisionReport> StdChoreographer::detectCirclePolygon(
        const std::shared_ptr<Circle>& circle,
        const std::shared_ptr<Polygon>& polygon)
    {
        std::shared_ptr<StdCollisionReport> report(new StdCollisionReport());

        Vec2r c = circle->centroid();
        real r = circle->radius();

        Vec2r minDist;
        real minDistLen2 = r*r;

        int nbv = polygon->nbVertices();
        const std::vector<Segment2Dr>& outline = polygon->outline();
        for(int i=0; i < nbv; ++i)
        {
            Vec2r dist = outline[i].pointMinimalDirection(c);
            real distLen2 = dist.length2();
            if(distLen2 < minDistLen2)
            {
                minDist = dist;
                minDistLen2 = distLen2;
            }
        }

        // Return now if the shapes do not collide
        if(minDistLen2 == r*r)
            return report;

        real minDistLen = sqrt(minDistLen2);

        report->areColliding = true;
        report->shape1 = circle;
        report->shape2 = polygon;
        report->penetrationDepth = r - minDistLen;
        report->contactNormal = minDist / minDistLen;
        report->contactPoint = circle->centroid() -
                               report->contactNormal *
                                    (r - report->penetrationDepth / real(2.0));

        return report;
    }

    std::shared_ptr<StdCollisionReport> StdChoreographer::detectPolygonPolygon(
        const std::shared_ptr<Polygon>& polygon1,
        const std::shared_ptr<Polygon>& polygon2)
    {
        std::shared_ptr<StdCollisionReport> report(new StdCollisionReport());

        const std::vector<Segment2Dr>& outline1 = polygon1->outline();
        const std::vector<Segment2Dr>& outline2 = polygon2->outline();

        int nbv1 = polygon1->nbVertices();
        int nbv2 = polygon2->nbVertices();

        Vec2r weightedNormalsAverage;
        Vec2r weightedCollisionPointsAverage;
        real weightsAccumulator = real(0.0);

        for(int i=0; i < nbv1; ++i)
        {
            Vec2r pt = outline1[i].begin();
            if(!polygon2->contains(pt))
                continue;

            Vec2r minDist = outline2[0].pointMinimalDirection(pt);
            real minDistLen2 = minDist.length2();
            for(int j=1; j < nbv2; ++j)
            {
                Vec2r dist = outline2[j].pointMinimalDirection(pt);
                real distLen2 = dist.length2();
                if(minDistLen2 < distLen2)
                    continue;

                minDist = dist;
                minDistLen2 = distLen2;
            }

            real pointContribution = sqrt(minDistLen2);
            weightsAccumulator += pointContribution;
            weightedNormalsAverage -= pointContribution * minDist;
            weightedCollisionPointsAverage += pointContribution * pt;
        }

        for(int i=0; i < nbv2; ++i)
        {
            Vec2r pt = outline2[i].begin();
            if(!polygon1->contains(pt))
                continue;

            Vec2r minDist = outline1[0].pointMinimalDirection(pt);
            real minDistLen2 = minDist.length2();
            for(int j=1; j < nbv1; ++j)
            {
                Vec2r dist = outline1[j].pointMinimalDirection(pt);
                real distLen2 = dist.length2();
                if(minDistLen2 < distLen2)
                    continue;

                minDist = dist;
                minDistLen2 = distLen2;
            }

            real pointContribution = sqrt(minDistLen2);
            weightsAccumulator += pointContribution;
            weightedNormalsAverage += pointContribution * minDist;
            weightedCollisionPointsAverage += pointContribution * pt;
        }

        // Return now if the shapes do not collide
        if(weightsAccumulator == real(0.0))
            return report;

        Vec2r col = weightedCollisionPointsAverage / weightsAccumulator;
        Vec2r pen = weightedNormalsAverage / weightsAccumulator;
        real penLen = pen.length();

        report->areColliding = true;
        report->shape1 = polygon1;
        report->shape2 = polygon2;
        report->penetrationDepth = penLen;
        report->contactNormal = pen / penLen;
        report->contactPoint = col;

        return report;
    }

    void StdChoreographer::manageCircle(const std::shared_ptr<Circle>& circle)
    {
        _circles.push_back(circle);
    }

    void StdChoreographer::managePolygon(const std::shared_ptr<Polygon>& polygon)
    {
        _polygons.push_back(polygon);
    }

    void StdChoreographer::manageTextHud(const std::shared_ptr<TextHud>&)
    {
        // Nothing to do
    }

    void StdChoreographer::manageImageHud(const std::shared_ptr<ImageHud>&)
    {
        // Nothing to do
    }

    void StdChoreographer::unmanageCircle(const std::shared_ptr<Circle>& circle)
    {
        for(auto it = _circles.begin(); it != _circles.end(); ++it)
        {
            if((*it)->id() == circle->id())
            {
                _circles.erase(it);
                return;
            }
        }
    }

    void StdChoreographer::unmanagePolygon(const std::shared_ptr<Polygon>& polygon)
    {
        for(auto it = _polygons.begin(); it != _polygons.end(); ++it)
        {
            if((*it)->id() == polygon->id())
            {
                _polygons.erase(it);
                return;
            }
        }
    }

    void StdChoreographer::unmanageTextHud(const std::shared_ptr<TextHud>&)
    {
        // Nothing to do
    }

    void StdChoreographer::unmanageImageHud(const std::shared_ptr<ImageHud>&)
    {
        // Nothing to do
    }

}
