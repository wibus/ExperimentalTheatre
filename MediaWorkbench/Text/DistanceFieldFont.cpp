#include "DistanceFieldFont.h"

#include <QFont>
#include <QFontMetrics>
#include <QPixmap>
#include <QPainter>

using namespace cellar;


namespace media
{
    const int DistanceFieldFont::NB_CHARACTERS = 256;


    DistanceFieldFont::DistanceFieldFont() :
        _positions(NB_CHARACTERS),
        _dimensions(NB_CHARACTERS)
    {

    }

    DistanceFieldFont::DistanceFieldFont(
        const std::string& family,
        float weight,
        int quality) :
        _weight(weight),
        _quality(quality)
    {
        setFamily(family);
    }

    bool DistanceFieldFont::setFamily(const std::string& family)
    {
        _family = family;

        QFont font(family.c_str(), _quality);
        QFontMetrics metrics(font);

        const int CHAR_HEIGHT = metrics.height() * NB_CHARACTERS;
        QPixmap pixmap(
            metrics.maxWidth(),
            CHAR_HEIGHT);

        QPainter painter(&pixmap);
        painter.setFont(font);
        painter.setRenderHint(QPainter::NonCosmeticDefaultPen);

        for(int i=0; i < NB_CHARACTERS; ++i)
        {
            QChar character(i);
            painter.drawText(0, i*CHAR_HEIGHT, QString(character));
            _positions[i] = Vec2i(0, i*CHAR_HEIGHT);
            _dimensions[i] = Vec2i(metrics.width(character), CHAR_HEIGHT);
        }
    }
}
