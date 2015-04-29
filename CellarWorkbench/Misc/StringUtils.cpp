#include "StringUtils.h"

#include <QFile>
#include <QTextStream>

using namespace std;


namespace cellar
{
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
