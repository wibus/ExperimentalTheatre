#ifndef MEDIAWORKBENCH_DISTANCE_FIELD_FONT_H
#define MEDIAWORKBENCH_DISTANCE_FIELD_FONT_H

#include <string>
#include <vector>

class QFont;

#include <GLM/glm.hpp>

#include "../libMediaWorkbench_global.h"


namespace media
{
    class MEDIA_EXPORT DistanceFieldFont
    {
    public:
        DistanceFieldFont();
        DistanceFieldFont(
            const std::string& fontFamily,
            float weight,
            int quality=12);

        bool setFamily(const std::string& family);
        std::string family() const;

        bool setWeight(float weight);
        float weight() const;

        int quality() const;

        glm::ivec2 charPosition(int unicode);
        glm::ivec2 charDimensions(int unicode);

    private:
        static const int NB_CHARACTERS;

        std::string _family;
        float _weight;
        int _quality;
        std::vector<glm::ivec2> _positions;
        std::vector<glm::ivec2> _dimensions;
    };



    // IMPLEMENTATION //
    inline std::string DistanceFieldFont::family() const
    {
        return _family;
    }

    inline bool DistanceFieldFont::setWeight(float weight)
    {
        _weight = weight;
    }

    inline float DistanceFieldFont::weight() const
    {
        return _weight;
    }

    inline int DistanceFieldFont::quality() const
    {
        return _quality;
    }

    inline glm::ivec2 DistanceFieldFont::charPosition(int unicode)
    {
        return _positions[unicode];
    }

    inline glm::ivec2 DistanceFieldFont::charDimensions(int unicode)
    {
        return _dimensions[unicode];
    }
}

#endif //MEDIAWORKBENCH_DISTANCE_FIELD_FONT_H
