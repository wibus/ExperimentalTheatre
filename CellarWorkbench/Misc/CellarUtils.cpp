#include "CellarUtils.h"

#include <sstream>
using namespace std;

#include <QFile>
#include <QTextStream>


namespace cellar
{
    int modulateInt(int& value, int minVal, int maxVal)
    {
        int amplitude = maxVal - minVal;
        int offset = value - minVal;
        int rounds = 0 <= offset ? offset/amplitude : (offset+1)/amplitude - 1;

        value -= rounds * amplitude;
        return rounds;
    }

    string fileToString(const string& fileName, bool* ok)
    {
        QFile file(fileName.c_str());
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            if(ok != nullptr) *ok = false;
            return string();
        }

        QTextStream stream(&file);
        string text = stream.readAll().toStdString();
        file.close();

        if(ok != nullptr) *ok = true;
        return text;
    }
}
