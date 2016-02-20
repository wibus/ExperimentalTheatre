#ifndef PROPROOM3D_STAGESET_H
#define PROPROOM3D_STAGESET_H

#include <string>
#include <vector>
#include <memory>

#include "StageZone.h"


namespace prop3
{
    class DebugLine;
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
        const std::vector<DebugLine>& debugLines() const;
        void addDebugLine(const DebugLine& line);
        void clearDebugLines();


        // Timestamps
        bool updateTimeStamp();

        bool stageSetChanged() const;


    private:
        std::shared_ptr<Material> _ambientMaterial;
        std::shared_ptr<Backdrop> _backdrop;
        TimeStamp _lastTimeStamp;
        bool _stageSetChanged;

        std::vector<DebugLine> _debugLines;
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

    inline const std::vector<DebugLine>& StageSet::debugLines() const
    {
        return _debugLines;
    }
}

#endif // PROPROOM3D_STAGESET_H
