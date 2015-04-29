#include "StdChoreographer.h"

#include <cassert>
using namespace std;


#include "../Shape/Circle.h"
#include "../Shape/Polygon.h"
#include "../Shape/Segment2D.h"
#include "../Hardware/Hardware.h"


namespace prop2
{
    StdChoreographer::StdChoreographer() :
        _dt(0),
        _gravity(0, 0),
        _circles(),
        _polygons(),
        _maxHandledDeltaTime(0.1),
        _correctionPercentage(0.65),
        _correctionSlop(0.1)
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

    void StdChoreographer::update(double dt)
    {
        _dt = dt;

        if(_dt == 0)
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
        shape->setLinearAcceleration(glm::dvec2());
        shape->setAngularAcceleration(0.0);

        // Apply friction
        glm::dvec3 linearFriction = shape->linearFrictionCoefficients();
        if(linearFriction != glm::dvec3(0))
        {
            glm::dvec2 linearVelocityDir = shape->linearVelocity();
            if(linearVelocityDir != glm::dvec2(0))
            {
                double  linearVelocityLen2 = glm::dot(linearVelocityDir,
                                                      linearVelocityDir);
                double  linearVelocityLen = glm::sqrt(linearVelocityLen2);

                linearVelocityDir /= linearVelocityLen;

                double linearDecelerationMagnitude =
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
                    shape->setLinearVelocity(glm::dvec2(0, 0));
                }
            }
        }

        glm::dvec3 angularFriction = shape->angularFrictionCoefficients();
        if(angularFriction != glm::dvec3(0))
        {
            double angularVelocity = shape->angularVelocity();
            if(angularVelocity)
            {
                double angularVelocity2 = angularVelocity * angularVelocity;
                double angularVelocityDir = glm::sign(angularVelocity);
                angularVelocity = glm::abs(angularVelocity);

                double angularDecelerationMagnitude =
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
                    shape->setAngularVelocity(0);
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

        glm::dvec2 n = report->contactNormal;
        double pen = report->penetrationDepth;

        double iM1 = shape1->inverseMass();
        double iM2 = shape2->inverseMass();

        double correction = _correctionPercentage *
                          glm::max(pen - _correctionSlop, 0.0) /
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
        glm::dvec2 n = report->contactNormal;
        glm::dvec2 t = Segment2D::perpCW(n);

        // Get distance of shapes' centroid from contact point
        glm::dvec2 p = report->contactPoint;
        glm::dvec2 r1 = p - shape1->centroid();
        glm::dvec2 r2 = p - shape2->centroid();

        // Get shapes' inertia
        double iM1 = shape1->inverseMass();
        double iM2 = shape2->inverseMass();
        double iI1 = shape1->inverseMomentOfInertia();
        double iI2 = shape2->inverseMomentOfInertia();

        // Get shapes' velocities
        glm::dvec2 v1 = shape1->linearVelocity();
        glm::dvec2 v2 = shape2->linearVelocity();
        double w1 = shape1->angularVelocity();
        double w2 = shape2->angularVelocity();

        // Get bounce coefficient
        double e = glm::min(shape1->hardware()->bounciness(),
                        shape2->hardware()->bounciness());


        // Compute normal impulse //
        double vn1 = glm::dot(v1, n);
        double vn2 = glm::dot(v2, n);
        double rn1 = Segment2D::cross(r1, n);
        double rn2 = Segment2D::cross(r2, n);

        double jn = (1 + e);
        jn *= ((vn2 + w2*rn2) - (vn1 + w1*rn1));
        jn /= (iM1 + iM2 + rn1*rn1*iI1 + rn2*rn2*iI2);


        // A negative impulse means that the shapes are already moving apart
        // No need to apply any impulse in that case
        if(jn < 0.0) return;


        // Compute tangent impulse //
        double vt1 = glm::dot(v1, t);
        double vt2 = glm::dot(v2, t);
        double rt1 = Segment2D::cross(r1, t);
        double rt2 = Segment2D::cross(r2, t);

        double jt = 1.0;
        jt *= ((vt2 + w2*rt2) - (vt1 + w1*rt1));
        jt /= (iM1 + iM2 + rt1*rt1*iI1 + rt2*rt2*iI2);


        // Compute the final tangent impulse assuming the fact that it is
        // limited by the shapes' static coefficients of friction and normal force
        double us = shape1->hardware()->staticFrictionCoefficient() *
                  shape2->hardware()->staticFrictionCoefficient();
        double absoluteJt = glm::abs(jt);
        if(jn*us < absoluteJt)
        {
            double ud = shape1->hardware()->dynamicFrictionCoefficient() *
                      shape2->hardware()->dynamicFrictionCoefficient();
            jt *= (jn / absoluteJt) * ud;
        }


        // Apply the final impulse
        glm::dvec2 j = t*jt + n*jn;
        shape1->applyImpulseAt( j, p);
        shape2->applyImpulseAt(-j, p);
    }

    std::shared_ptr<StdCollisionReport> StdChoreographer::detectCircleCircle(
        const std::shared_ptr<Circle>& circle1,
        const std::shared_ptr<Circle>& circle2)
    {
        std::shared_ptr<StdCollisionReport> report(new StdCollisionReport());

        double r1 = circle1->radius();
        double r2 = circle2->radius();

        double minDistLen = r1 + r2;
        double minDistLen2 = minDistLen*minDistLen;

        glm::dvec2 dist = circle1->centroid() - circle2->centroid();
        double distLen2 = glm::dot(dist, dist);

        // Return now if the shapes do not collide
        if(minDistLen2 < distLen2)
            return report;

        double distLen = sqrt(distLen2);
        double radiusRatio2 = r2 / (r1+r2);

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

        glm::dvec2 c = circle->centroid();
        double r = circle->radius();

        glm::dvec2 minDist;
        double minDistLen2 = r*r;

        int nbv = polygon->nbVertices();
        const std::vector<Segment2D>& outline = polygon->outline();
        for(int i=0; i < nbv; ++i)
        {
            glm::dvec2 dist = outline[i].pointMinimalDirection(c);
            double distLen2 = glm::dot(dist, dist);
            if(distLen2 < minDistLen2)
            {
                minDist = dist;
                minDistLen2 = distLen2;
            }
        }

        // Return now if the shapes do not collide
        if(minDistLen2 == r*r)
            return report;

        double minDistLen = sqrt(minDistLen2);

        report->areColliding = true;
        report->shape1 = circle;
        report->shape2 = polygon;
        report->penetrationDepth = r - minDistLen;
        report->contactNormal = minDist / minDistLen;
        report->contactPoint = circle->centroid() -
                               report->contactNormal *
                                    (r - report->penetrationDepth / 2.0);

        return report;
    }

    std::shared_ptr<StdCollisionReport> StdChoreographer::detectPolygonPolygon(
        const std::shared_ptr<Polygon>& polygon1,
        const std::shared_ptr<Polygon>& polygon2)
    {
        std::shared_ptr<StdCollisionReport> report(new StdCollisionReport());

        const std::vector<Segment2D>& outline1 = polygon1->outline();
        const std::vector<Segment2D>& outline2 = polygon2->outline();

        int nbv1 = polygon1->nbVertices();
        int nbv2 = polygon2->nbVertices();

        glm::dvec2 weightedNormalsAverage;
        glm::dvec2 weightedCollisionPointsAverage;
        double weightsAccumulator = 0.0;

        for(int i=0; i < nbv1; ++i)
        {
            glm::dvec2 pt = outline1[i].begin();
            if(!polygon2->contains(pt))
                continue;

            glm::dvec2 minDist = outline2[0].pointMinimalDirection(pt);
            double minDistLen2 = glm::dot(minDist, minDist);
            for(int j=1; j < nbv2; ++j)
            {
                glm::dvec2 dist = outline2[j].pointMinimalDirection(pt);
                double distLen2 = glm::dot(dist, dist);
                if(minDistLen2 < distLen2)
                    continue;

                minDist = dist;
                minDistLen2 = distLen2;
            }

            double pointContribution = sqrt(minDistLen2);
            weightsAccumulator += pointContribution;
            weightedNormalsAverage -= pointContribution * minDist;
            weightedCollisionPointsAverage += pointContribution * pt;
        }

        for(int i=0; i < nbv2; ++i)
        {
            glm::dvec2 pt = outline2[i].begin();
            if(!polygon1->contains(pt))
                continue;

            glm::dvec2 minDist = outline1[0].pointMinimalDirection(pt);
            double minDistLen2 = glm::dot(minDist, minDist);
            for(int j=1; j < nbv1; ++j)
            {
                glm::dvec2 dist = outline1[j].pointMinimalDirection(pt);
                double distLen2 = glm::dot(dist, dist);
                if(minDistLen2 < distLen2)
                    continue;

                minDist = dist;
                minDistLen2 = distLen2;
            }

            double pointContribution = sqrt(minDistLen2);
            weightsAccumulator += pointContribution;
            weightedNormalsAverage += pointContribution * minDist;
            weightedCollisionPointsAverage += pointContribution * pt;
        }

        // Return now if the shapes do not collide
        if(weightsAccumulator == 0.0)
            return report;

        glm::dvec2 col = weightedCollisionPointsAverage / weightsAccumulator;
        glm::dvec2 pen = weightedNormalsAverage / weightsAccumulator;
        double penLen = glm::length(pen);

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
