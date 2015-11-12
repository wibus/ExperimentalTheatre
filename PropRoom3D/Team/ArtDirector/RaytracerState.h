#ifndef PROPROOM3D_RAYTRACERSTATE_H
#define PROPROOM3D_RAYTRACERSTATE_H

#include <chrono>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class PROP3D_EXPORT RaytracerState
    {
    public:
        class ProtectedState
        {
            friend class RaytracerState;

        public:
            ProtectedState();

            // Setters
            void startTimeChrono();

            void setWorkerCount(int workerCount);

            void setInterrupted(bool interrupted);

            void setSampleCount(unsigned int sampleCount);

            void setDivergence(double divergence);

            void setDraftLevel(int draftLevel);


        private:
            double renderTime() const;

            std::chrono::steady_clock::time_point _startTime;

            int _workerCount;
            bool _interrupted;
            unsigned int _sampleCount;
            double _divergence;
            int _draftLevel;
        };


        RaytracerState(ProtectedState& state);

        int workerCount() const;

        bool interrupted() const;

        unsigned int sampleCount() const;

        double renderTime() const;


        double divergence() const;

        double divergenceThreshold() const;


        int draftLevel() const;

        int draftLevelCount() const;

        int draftLevelSizeRatio() const;

        int draftFrameCountPerLevel() const;

        bool fastDraftEnabled() const;


        bool isDrafter() const;

        bool isDrafting() const;

        bool isRendering() const;


        void setDivergenceThreshold(double divergenceThreshold);

        void setDraftParams(
                int levelCount,
                int levelSizeRatio,
                int frameCountPerLevel,
                bool enableFastDraft);


    private:
        ProtectedState& _protectedState;

        double _divergenceThreshold;

        int _draftLevelCount;
        int _draftLevelSizeRatio;
        int _draftFrameCountPerLevel;
        bool _fastDraftEnabled;
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
        return _divergenceThreshold;
    }

    inline int RaytracerState::draftLevel() const
    {
        return _protectedState._draftLevel;
    }

    inline int RaytracerState::draftLevelCount() const
    {
        return _draftLevelCount;
    }

    inline int RaytracerState::draftLevelSizeRatio() const
    {
        return _draftLevelSizeRatio;
    }

    inline int RaytracerState::draftFrameCountPerLevel() const
    {
        return _draftFrameCountPerLevel;
    }

    inline bool RaytracerState::fastDraftEnabled() const
    {
        return isDrafter() && _fastDraftEnabled;
    }

    inline bool RaytracerState::isDrafter() const
    {
        return _draftLevelCount != 0 && _draftFrameCountPerLevel != 0;
    }

    inline bool RaytracerState::isDrafting() const
    {
        return isDrafter() && (_protectedState._draftLevel < _draftLevelCount);
    }

    inline bool RaytracerState::isRendering() const
    {
        return isDrafting() || (_divergenceThreshold <= _protectedState._divergence);
    }
}

#endif // PROPROOM3D_RAYTRACERSTATE_H
