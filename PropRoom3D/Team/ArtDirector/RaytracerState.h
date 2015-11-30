#ifndef PROPROOM3D_RAYTRACERSTATE_H
#define PROPROOM3D_RAYTRACERSTATE_H

#include <chrono>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class PROP3D_EXPORT RaytracerState
    {
    public:
        class DraftParams
        {
        public:
            DraftParams();
            DraftParams(
                int levelCount,
                int sizeRatio,
                int frameCountPerLevel,
                bool fastDraftEnabled);

            int levelCount;
            int sizeRatio;
            int frameCountPerLevel;
            bool fastDraftEnabled;
        };

        class ProtectedState
        {
            friend class RaytracerState;

        public:
            ProtectedState();

            // Setters
            void startTimeChrono();

            void setWorkerCount(int workerCount);

            void setInterrupted(bool interrupted);

            void incSampleCount();

            void addSampleCount(unsigned int count);

            void resetSampleCount();

            void setDivergenceThreshold(double divergenceThreshold);

            void setDivergence(double divergence);

            void setDraftLevel(int draftLevel);

            void setDraftParams(const DraftParams& draftParams);


        private:
            double renderTime() const;

            std::chrono::steady_clock::time_point _startTime;

            int _workerCount;
            bool _interrupted;
            unsigned int _sampleCount;

            double _divergenceThreshold;
            double _divergence;

            DraftParams _draftParams;
            int _draftLevel;
        };


        RaytracerState(ProtectedState& state);

        int workerCount() const;

        bool interrupted() const;

        unsigned int sampleCount() const;

        double renderTime() const;


        double divergence() const;

        double divergenceThreshold() const;

        bool converged() const;


        int draftLevel() const;

        int draftLevelCount() const;

        int draftLevelSizeRatio() const;

        int draftFrameCountPerLevel() const;

        bool fastDraftEnabled() const;


        bool isDrafter() const;

        bool isDrafting() const;

        bool isRendering() const;


    private:
        ProtectedState& _protectedState;
    };



    // IMPLEMENTATION //
    inline int RaytracerState::workerCount() const
    {
        return _protectedState._workerCount;
    }

    inline bool RaytracerState::interrupted() const
    {
        return _protectedState._interrupted;
    }

    inline unsigned int RaytracerState::sampleCount() const
    {
        return _protectedState._sampleCount;
    }

    inline double RaytracerState::renderTime() const
    {
        if(isDrafting() || sampleCount() == 0)
            return 0.0;

        return _protectedState.renderTime();
    }

    inline double RaytracerState::divergence() const
    {
        return _protectedState._divergence;
    }

    inline double RaytracerState::divergenceThreshold() const
    {
        return _protectedState._divergenceThreshold;
    }

    inline bool RaytracerState::converged() const
    {
        return _protectedState._divergenceThreshold >=
               _protectedState._divergence;
    }

    inline int RaytracerState::draftLevel() const
    {
        return _protectedState._draftLevel;
    }

    inline int RaytracerState::draftLevelCount() const
    {
        return _protectedState._draftParams.levelCount;
    }

    inline int RaytracerState::draftLevelSizeRatio() const
    {
        return _protectedState._draftParams.sizeRatio;
    }

    inline int RaytracerState::draftFrameCountPerLevel() const
    {
        return _protectedState._draftParams.frameCountPerLevel;
    }

    inline bool RaytracerState::fastDraftEnabled() const
    {
        return isDrafter() && _protectedState._draftParams.fastDraftEnabled;
    }

    inline bool RaytracerState::isDrafter() const
    {
        return _protectedState._draftParams.levelCount != 0 &&
               _protectedState._draftParams.frameCountPerLevel != 0;
    }

    inline bool RaytracerState::isDrafting() const
    {
        return isDrafter() && (_protectedState._draftLevel <
                               _protectedState._draftParams.levelCount);
    }

    inline bool RaytracerState::isRendering() const
    {
        return isDrafting() || (!converged());
    }
}

#endif // PROPROOM3D_RAYTRACERSTATE_H
