#include "CellarUtils.h"

#include <fstream>
#include <sstream>
using namespace std;

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
        ifstream file;
        file.open(fileName.c_str(), ifstream::in);
        if(!file.is_open())
        {
            if(ok != nullptr) *ok = false;
            return string();
        }

        stringstream text;
        text << file.rdbuf();
        file.close();

        if(ok != nullptr) *ok = true;
        return text.str();
    }
}
