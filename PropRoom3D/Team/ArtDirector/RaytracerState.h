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

            void setHiddenSurfaceRemoved(bool removed);

            void incSampleCount();

            void addSampleCount(unsigned int count);

            void resetSampleCount();

            void setDivergence(double divergence);

            void setDraftLevel(int draftLevel);

            void setDraftParams(const DraftParams& draftParams);


        private:
            double renderTime() const;


            int _workerCount;
            bool _interrupted;
            bool _hiddenSurfacesRemoved;

            std::chrono::steady_clock::time_point _startTime;
            unsigned int _sampleCount;
            double _divergence;

            DraftParams _draftParams;
            int _draftLevel;
        };


        RaytracerState(ProtectedState& state);

        int workerCount() const;

        bool interrupted() const;


        void setSampleCountThreshold(unsigned int sampleCountThreshold);

        unsigned int sampleCount() const;

        bool runningOutOfSamples() const;


        void setRenderTimeThreshold(double renderTimeThreshold);

        double renderTime() const;

        bool runningOutOfTime() const;


        void setDivergenceThreshold(double divergenceThreshold);

        double divergence() const;

        double divergenceThreshold() const;

        bool converged() const;


        void setSurfaceVisibilityThreshold(double threshold);

        double surfaceVisibilityThreshold() const;

        bool debuggingHiddenSurfaceRemoval() const;

        bool hiddenSurfacesRemoved() const;


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


        void setFilmRawFilePath(const std::string& filePath);

        std::string filmRawFilePath() const;


        static const std::string COLOROUTPUT_ALBEDO;
        static const std::string COLOROUTPUT_WEIGHT;
        static const std::string COLOROUTPUT_DIVERGENCE;
        static const std::string COLOROUTPUT_VARIANCE;
        static const std::string COLOROUTPUT_PRIORITY;
        static const std::string COLOROUTPUT_REFERENCE;
        static const std::string COLOROUTPUT_COMPATIBILITY;

        static const std::string UNSPECIFIED_RAW_FILE;


    private:
        ProtectedState& _protectedState;
        bool _isUpdateEachTileEnabled;
        std::string _colorOutputType;
        std::string _filmRawFilePath;

        unsigned int _sampleCountThreshold;
        double _renderTimeThreshold;
        double _divergenceThreshold;
        double _surfaceVisibilityThreshold;
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
        return sampleCount() >= _sampleCountThreshold;
    }

    inline double RaytracerState::renderTime() const
    {
        if(isDrafting() || sampleCount() == 0)
            return 0.0;

        return _protectedState.renderTime();
    }

    inline bool RaytracerState::runningOutOfTime() const
    {
        return renderTime() >= _renderTimeThreshold;
    }

    inline double RaytracerState::divergence() const
    {
        return _protectedState._divergence;
    }

    inline double RaytracerState::divergenceThreshold() const
    {
        return _divergenceThreshold;
    }

    inline bool RaytracerState::converged() const
    {
        return _divergenceThreshold >= divergence();
    }

    inline double RaytracerState::surfaceVisibilityThreshold() const
    {
        return _surfaceVisibilityThreshold;
    }

    inline bool RaytracerState::debuggingHiddenSurfaceRemoval() const
    {
        return _surfaceVisibilityThreshold < 0.0;
    }

    inline bool RaytracerState::hiddenSurfacesRemoved() const
    {
        return _protectedState._hiddenSurfacesRemoved;
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

    inline std::string RaytracerState::filmRawFilePath() const
    {
        return _filmRawFilePath;
    }
}

#endif // PROPROOM3D_RAYTRACERSTATE_H
