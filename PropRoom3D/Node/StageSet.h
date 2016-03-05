#ifndef PROPROOM3D_STAGESET_H
#define PROPROOM3D_STAGESET_H

#include <string>
#include <vector>
#include <memory>

#include "StageZone.h"


namespace prop3
{
    class DebugLineStrip;
    class DebugPointCloud;
    class Backdrop;
    class Material;
    class Visitor;


    class PROP3D_EXPORT StageSet : public StageZone
    {
    public :
        StageSet();
        virtual ~StageSet();


        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual std::vector<std::shared_ptr<Node>> children() const;


        // Ambient Material
        std::shared_ptr<Material> ambientMaterial() const;

        virtual void setAmbientMaterial(const std::shared_ptr<Material>& ambientMaterial);


        // Backdrop
        std::shared_ptr<Backdrop> backdrop() const;

        virtual void setBackdrop(const std::shared_ptr<Backdrop>& backdrop);


        // Tree travelling
        virtual void makeTraveling(Visitor& visitor);


        // Debug props
        const std::vector<DebugLineStrip>& debugLines() const;
        void addDebugLine(const DebugLineStrip& line);
        void clearDebugLines();

        const std::vector<DebugPointCloud>& debugPoints() const;
        void addDebugPoints(const DebugPointCloud& points);
        void clearDebugPoints();


        // Timestamps
        bool updateTimeStamp();

        bool stageSetChanged() const;


    private:
        std::shared_ptr<Material> _ambientMaterial;
        std::shared_ptr<Backdrop> _backdrop;
        TimeStamp _lastTimeStamp;
        bool _stageSetChanged;

        std::vector<DebugLineStrip> _debugLines;
        std::vector<DebugPointCloud> _debugPoints;
    };



    // IMPLEMENTATION //
    inline std::shared_ptr<Material> StageSet::ambientMaterial() const
    {
        return _ambientMaterial;
    }

    inline std::shared_ptr<Backdrop> StageSet::backdrop() const
    {
        return _backdrop;
    }

    inline bool StageSet::stageSetChanged() const
    {
        return _stageSetChanged;
    }

    inline const std::vector<DebugLineStrip>& StageSet::debugLines() const
    {
        return _debugLines;
    }

    inline const std::vector<DebugPointCloud>& StageSet::debugPoints() const
    {
        return _debugPoints;
    }
}

#endif // PROPROOM3D_STAGESET_H
