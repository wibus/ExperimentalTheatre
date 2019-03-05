#include "StageSet.h"

#include <algorithm>

#include "Node/Visitor.h"
#include "Node/Prop/Material/Material.h"
#include "Node/Light/Backdrop/ProceduralSun.h"

using namespace std;


namespace prop3
{
    StageSet::StageSet() :
        StageZone("Stage set"),
        _ambientMaterial(material::AIR),
        _backdrop(new ProceduralSun())
    {
        registerTo(_ambientMaterial);
        registerTo(_backdrop);
    }

    StageSet::~StageSet()
    {
        unregisterFrom(_ambientMaterial);
        unregisterFrom(_backdrop);
    }

    void StageSet::accept(Visitor& visitor)
    {
        visitor.visit(*this);
    }

    std::vector<std::shared_ptr<Node>> StageSet::children() const
    {
        std::vector<std::shared_ptr<Node>> c = StageZone::children();
        c.push_back(_ambientMaterial);
        c.push_back(_backdrop);
        return c;
    }

    void StageSet::setAmbientMaterial(const std::shared_ptr<Material>& ambientMaterial)
    {
        swapChild(_ambientMaterial, ambientMaterial);
        _ambientMaterial = ambientMaterial;

        stampCurrentUpdate();
    }

    void StageSet::setBackdrop(const std::shared_ptr<Backdrop>& backdrop)
    {
        swapChild(_backdrop, backdrop);
        _backdrop = backdrop;

        stampCurrentUpdate();
    }

    void StageSet::makeTraveling(Visitor& visitor)
    {
        vector<Node*> nodeStack;

        // Level-order tree insertion
        nodeStack.push_back(this);
        for(size_t i=0; i < nodeStack.size(); ++i)
        {
            Node* node = nodeStack[i];

            if(node != nullptr)
            {
                for(const auto& n : node->children())
                    nodeStack.push_back(n.get());
            }
        }

        // Post-order tree visit
        while(!nodeStack.empty())
        {
            Node* node = nodeStack.back();

            if(node != nullptr)
            {
                node->accept(visitor);
            }

            nodeStack.pop_back();
        }
    }

    bool StageSet::stageSetChanged(const TimeStamp &reference)
    {
		if(reference < timeStamp())
			return true;
		else
			return false;
    }

    void StageSet::addDebugLine(const DebugLineStrip& line)
    {
        _debugLines.push_back(line);

        // We do not want to update its timestamp or otherwise
        // it would invalidate current raytraced frame.
    }

    void StageSet::clearDebugLines()
    {
        _debugLines.clear();

        // We do not want to update its timestamp or otherwise
        // it would invalidate current raytraced frame.
    }

    void StageSet::addDebugPoints(const DebugPointCloud& points)
    {
        _debugPoints.push_back(points);

        // We do not want to update its timestamp or otherwise
        // it would invalidate current raytraced frame.
    }

    void StageSet::clearDebugPoints()
    {
        _debugPoints.clear();

        // We do not want to update its timestamp or otherwise
        // it would invalidate current raytraced frame.
    }
}

