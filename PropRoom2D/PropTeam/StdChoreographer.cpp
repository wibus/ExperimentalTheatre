#include "StdChoreographer.h"

#include "Shape/Circle.h"
#include "Shape/Polygon.h"

#include <cassert>
using namespace std;


namespace prop2
{
    StdChoreographer::StdChoreographer() :
        _dt(real(0)),
        _gravity(real(98.0)), // 9.8m/s2 * 10px/m
        _circles(),
        _polygons()
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
            return;
        }

        size_t nbCircles = _circles.size();
        size_t nbPolygons = _polygons.size();


        // Resolve collisions of dynamic shapes
        // Order is important;
        // A shape should never trigger a resolveXY() with a preceding shape :
        //  * Circles precede Polygons
        //  * Lower indices precede higher indices

        for(size_t master=0; master < nbCircles; ++master)
        {
            shared_ptr<Circle>& masterCircle = _circles[master];
            if(masterCircle->bodyType() == BodyType::DYNAMIC)
            {
                for(size_t slave = 0; slave < nbCircles; ++slave)
                    if(slave > master || _circles[slave]->bodyType() != BodyType::DYNAMIC)
                    {
                        resolveCircleCircle(masterCircle, _circles[slave]);
                        updateVelocity(masterCircle.get());
                        updateVelocity(_circles[slave].get());
                    }

                for(size_t slave = 0; slave < nbPolygons; ++slave)
                {
                    resolveCirclePolygon(masterCircle, _polygons[slave]);
                    updateVelocity(masterCircle.get());
                    updateVelocity(_polygons[slave].get());
                }
            }
        }

        for(size_t master=0; master < nbPolygons; ++master)
        {
            shared_ptr<Polygon>& masterPolygon = _polygons[master];
            if(masterPolygon->bodyType() == BodyType::DYNAMIC)
            {
                for(size_t slave = 0; slave < nbCircles; ++slave)
                    if(_circles[slave]->bodyType() != BodyType::DYNAMIC)
                    {
                        resolveCirclePolygon(_circles[slave], masterPolygon);
                        updateVelocity(_circles[slave].get());
                        updateVelocity(masterPolygon.get());
                    }

                for(size_t slave = 0; slave < nbPolygons; ++slave)
                    if(slave > master || _polygons[slave]->bodyType() != BodyType::DYNAMIC)
                    {
                        resolvePolygonPolygon(masterPolygon, _polygons[slave]);
                        updateVelocity(masterPolygon.get());
                        updateVelocity(_polygons[slave].get());
                    }
            }
        }


        // Update all shapes with added forces
        for(size_t i=0; i<_circles.size(); ++i)
            updatePosition(_circles[i].get());

        for(size_t i=0; i<_polygons.size(); ++i)
            updatePosition(_polygons[i].get());
    }

    void StdChoreographer::updateVelocity(AbstractShape* shape)
    {
        if(shape->bodyType() == BodyType::DYNAMIC)
        {
            shape->addLinearVelocity(shape->linearAcceleration() * _dt);
            shape->setLinearAcceleration(Vec2r(real(0), real(0)));

            shape->addAngularVelocity(shape->angularAcceleration() * _dt);
            shape->setAngularAcceleration(real(0));
        }
    }

    void StdChoreographer::updatePosition(AbstractShape* shape)
    {
        if(shape->bodyType() == BodyType::DYNAMIC)
        {
            // Gravity
            shape->addLinearAcceleration(Vec2r(real(0), -_gravity));
            updateVelocity(shape);
        }
        if(shape->bodyType() == BodyType::DYNAMIC ||
           shape->bodyType() == BodyType::CINEMATIC)
        {
            shape->moveBy(shape->linearVelocity() * _dt);
            shape->rotate(shape->angularVelocity() * _dt);
        }
    }

    void StdChoreographer::resolveCircleCircle(
        std::shared_ptr<Circle>& circle1,
        std::shared_ptr<Circle>& circle2)
    {
        Vec2r centersDist = circle2->centroid() - circle1->centroid();

        // Test for collision
        real overlappindDist = circle1->radius() + circle2->radius() - (real)centersDist.length();
        if(overlappindDist < real(0.0) )
            return;

        // Collision normal points out circle1 (local y axis)
        Vec2r uNormal = centersDist.normalize();
        // Collision tangent points right to normal (local x axis)
        Vec2r uTangent = perpCCW(uNormal);

        real bounce = ((circle1->bounciness() *
                        circle2->bounciness()) + 1) / 2;

        if(circle1->bodyType() == BodyType::DYNAMIC &&
           circle2->bodyType() == BodyType::DYNAMIC)
        {
            Vec2r c1vn = proj(circle1->linearVelocity(), uNormal);
            real  c1mass = circle1->mass();

            Vec2r c2vn = proj(circle2->linearVelocity(), uNormal);
            real c2mass = circle2->mass();

            real massSum = c1mass + c2mass;
            real massDiff = c1mass - c2mass;

            Vec2r c1vtFinal = (2*c2mass*c2vn + c1vn*massDiff) / massSum;
            Vec2r c2vtFinal = (2*c1mass*c1vn - c2vn*massDiff) / massSum;

            // Add forces
            circle1->addLinearAcceleration(bounce * (c1vtFinal - c1vn) / _dt);
            circle2->addLinearAcceleration(bounce * (c2vtFinal - c2vn) / _dt);

            // Rotation
            Vec2r Vr = proj(circle2->linearVelocity() - circle1->linearVelocity(),
                            uTangent);
            Vec2r Wr = (circle1->angularVelocity() * circle1->radius() +
                        circle2->angularVelocity() * circle2->radius()) * (-uTangent);
            Vec2r impulse = (Vr + Wr) * sqrt(circle1->momentOfInertia() * circle2->momentOfInertia()) /
                    (circle1->radius() + circle2->radius());

            Vec2r at = circle1->centroid() + uNormal * circle1->radius();
            circle1->addForceAt(impulse, at);
            circle2->addForceAt(-impulse, at);

            // Move shapes
            real c1Move = -(c2mass / massSum * overlappindDist);
            real c2Move = c1Move + overlappindDist;
            circle1->moveBy(uNormal * c1Move);
            circle2->moveBy(uNormal * c2Move);
        }
        else if(circle1->bodyType() == BodyType::DYNAMIC)
        {
            // Normal
            Vec2r Vn = proj(circle1->linearVelocity(), uNormal);
            circle1->addLinearAcceleration(bounce * (-2.0f*Vn) / _dt);

            // Tangent
            real radius = circle1->radius();
            real  M  = circle1->mass();
            real  I  = circle1->momentOfInertia();
            Vec2r R  = uNormal * circle1->radius();
            real  Wi = circle1->angularVelocity();
            Vec2r Vi = proj(circle1->linearVelocity() - circle2->linearVelocity() +
                        uTangent * circle2->radius() * circle2->angularVelocity(),
                        uTangent);

            real Wf = (I*Wi - M*cross(R, Vi)) / (I + M*radius*radius);

            circle1->addForceAt(uTangent * (Wf - Wi) * I / (radius * _dt),
                                circle1->centroid() + R);

            // Move
            circle1->moveBy(-uNormal * overlappindDist);
        }
        else if(circle2->bodyType() == BodyType::DYNAMIC)
        {
            // Normal
            Vec2r Vn = proj(circle2->linearVelocity(), uNormal);
            circle2->addLinearAcceleration(bounce * (-2.0f*Vn) / _dt);

            // Tangent
            real  radius = circle2->radius();
            real  M  = circle2->mass();
            real  I  = circle2->momentOfInertia();
            Vec2r R  = -uNormal * circle2->radius();
            real  Wi = circle2->angularVelocity();
            Vec2r Vi = proj(circle2->linearVelocity() - circle1->linearVelocity() -
                            uTangent * circle1->radius() * circle1->angularVelocity(),
                            uTangent);

            real Wf = (I*Wi - M*cross(R, Vi)) / (I + M*radius*radius);

            circle2->addForceAt(-uTangent * (Wf - Wi) * I / (radius * _dt),
                                circle2->centroid() + R);

            // Move
            circle2->moveBy(uNormal * overlappindDist);
        }
        else
        {
            assert(true /* resolution called for two non-dynamic shapes : circle-circle*/ );
        }
    }

    void StdChoreographer::resolveCirclePolygon(
        std::shared_ptr<Circle>& circle,
        std::shared_ptr<Polygon>& polygon)
    {
        const vector<Segment2Dr>& pOultine = polygon->outline();
        if(pOultine.empty())
            return;

        Vec2r minimalVect;
        real minimalDist2(0);
        real radius2 = circle->radius() * circle->radius();
        size_t nbSides = pOultine.size();
        for(size_t i=0; i < nbSides; ++i)
        {
            const Segment2Dr& line = pOultine[i];
            Vec2r distVect = line.pointMinimalDirection(circle->centroid());
            real distLength2 = distVect.length2();
            if(distLength2 < radius2)
            {
                Vec2r distVect = line.pointMinimalDirection(circle->centroid());
                if(!minimalDist2 || distLength2 < minimalVect.length2())
                    minimalVect = distVect;
                    minimalDist2 = distLength2;
            }
        }

        // If there isn't any collision, exit
        if(!minimalDist2)
            return;

        if(circle->bodyType() == BodyType::DYNAMIC &&
           polygon->bodyType() == BodyType::DYNAMIC)
        {
            // TODO wibus 2012-10-28: resolve circle and polygon for circle-polygon
        }
        else if(circle->bodyType() == BodyType::DYNAMIC)
        {
            Vec2r n = minimalVect.normalized();
            Vec2r t = perpCW( n );

            real M = circle->mass();
            real R = circle->radius();
            real I = circle->momentOfInertia();
            real e = circle->bounciness() * polygon->bounciness();
            Vec2r vi = circle->linearVelocity();

            real j = -(1 + e)proj(vi, n) / ();

/*
            Vec2r vNi = proj(circle->linearVelocity(), n);
            Vec2r vNf = - vNi * circle->bounciness()*polygon->bounciness();
            circle->addLinearForce((vNf - vNi)*M / _dt);


            real VdotT = dot(circle->linearVelocity(), t);
            real WdotT = circle->angularVelocity();
            real j = (VdotT - WdotT*R) / (R*R/I + 1.0/M);
            Vec2r vTi = proj(circle->linearVelocity(), t);
            Vec2r wTi = circle->angularVelocity() * R * t;
            Vec2r vTf = vTi + wTi;
            circle->addForceAt(-(vTf)*M / _dt, circle->centroid() - n*R);
*/
            circle->moveBy(minimalVect.normalized() * (minimalVect.length() - circle->radius()));
        }
        else if(polygon->bodyType() == BodyType::DYNAMIC)
        {
            // TODO wibus 2012-10-28: resolve polygon for circle-polygon
        }
        else
        {
            assert(true /* resolution called for two non-dynamic shapes : circle-polygon*/ );
        }
    }

    void StdChoreographer::resolvePolygonPolygon(
        std::shared_ptr<Polygon>& polygon1,
        std::shared_ptr<Polygon>& polygon2)
    {
        // TODO wibus 2012-10-21 : Implement polygon-polygon
        // collision resolution
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
    }

    void StdChoreographer::unmanageImageHud(const std::shared_ptr<ImageHud>&)
    {
    }

}
