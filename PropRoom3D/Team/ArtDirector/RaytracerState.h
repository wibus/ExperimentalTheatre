#ifndef PROPROOM3D_RAYTRACERSTATE_H
#define PROPROOM3D_RAYTRACERSTATE_H

#include <chrono>
#include <string>

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

            void setSampleCountThreshold(unsigned int sampleCountThreshold);

            void setRenderTimeThreshold(double renderTimeThreshold);

            void setDivergence(double divergence);

            void setDraftLevel(int draftLevel);

            void setDraftParams(const DraftParams& draftParams);


        private:
            double renderTime() const;


            int _workerCount;
            bool _interrupted;

            std::chrono::steady_clock::time_point _startTime;
            double _renderTimeThreshold;

            unsigned int _sampleCount;
            unsigned int _sampleCountThreshold;

            double _divergence;
            double _divergenceThreshold;

            DraftParams _draftParams;
            int _draftLevel;
        };


        RaytracerState(ProtectedState& state);

        int workerCount() const;

        bool interrupted() const;


        unsigned int sampleCount() const;

        bool runningOutOfSamples() const;


        double renderTime() const;

        bool runningOutOfTime() const;


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


        void setUpdateEachTile(bool enabled);
        bool isUpdateEachTileEnabled() const;

        void setColorOutputType(const std::string& colorOutput);
        std::string colorOutputType() const;

        static const std::string COLOROUTPUT_ALBEDO;
        static const std::string COLOROUTPUT_WEIGHT;
        static const std::string COLOROUTPUT_DIVERGENCE;
        static const std::string COLOROUTPUT_VARIANCE;
        static const std::string COLOROUTPUT_PRIORITY;

    private:
        ProtectedState& _protectedState;
        bool _isUpdateEachTileEnabled;
        std::string _colorOutputType;
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

    inline bool RaytracerState::runningOutOfSamples() const
    {
        return sampleCount() >= _protectedState._sampleCountThreshold;
    }

    inline double RaytracerState::renderTime() const
    {
        if(isDrafting() || sampleCount() == 0)
            return 0.0;

        return _protectedState.renderTime();
    }

    inline bool RaytracerState::runningOutOfTime() const
    {
        return renderTime() >= _protectedState._renderTimeThreshold;
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
        return isDrafting() || (!converged() && !runningOutOfTime() && !runningOutOfSamples());
    }

    inline bool RaytracerState::isUpdateEachTileEnabled() const
    {
        return _isUpdateEachTileEnabled;
    }

    inline std::string RaytracerState::colorOutputType() const
    {
        return _colorOutputType;
    }
}

#endif // PROPROOM3D_RAYTRACERSTATE_H
