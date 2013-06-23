#ifndef MEDIAWORKBENCH_DISTANCE_FIELD_FONT_H
#define MEDIAWORKBENCH_DISTANCE_FIELD_FONT_H

#include "libMediaWorkbench_global.h"

#include <string>
#include <vector>
#include <DataStructure/Vector.h>

class QFont;


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

        cellar::Vec2i charPosition(int unicode);
        cellar::Vec2i charDimensions(int unicode);

    private:
        static const int NB_CHARACTERS;

        std::string _family;
        float _weight;
        int _quality;
        std::vector<cellar::Vec2i> _positions;
        std::vector<cellar::Vec2i> _dimensions;
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

    inline cellar::Vec2i DistanceFieldFont::charPosition(int unicode)
    {
        return _positions[unicode];
    }

    inline cellar::Vec2i DistanceFieldFont::charDimensions(int unicode)
    {
        return _dimensions[unicode];
    }
}

#endif //MEDIAWORKBENCH_DISTANCE_FIELD_FONT_H
