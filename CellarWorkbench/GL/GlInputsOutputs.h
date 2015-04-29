#ifndef CELLARWORKBENCH_GLINPUTSOUTPUTS_H
#define CELLARWORKBENCH_GLINPUTSOUTPUTS_H

#include <map>
#include <string>


#include "../libCellarWorkbench_global.h"


namespace cellar
{
    class CELLAR_EXPORT GlInputsOutputs
    {
    public:
        GlInputsOutputs();

        // Accesors
        const std::map<unsigned int, std::string>& inputs() const;
        const std::map<unsigned int, std::string>& outputs() const;

        // Modifiers
        bool setInput(unsigned int index, const std::string& name);
        bool setOutput(unsigned int index, const std::string& name);

        typedef std::map<unsigned int, std::string>::const_iterator IOit;

    private:
        std::map<unsigned int, std::string> _inputs;
        std::map<unsigned int, std::string> _outputs;
    };



    // IMPLEMENTATION //
    // Accesors
    inline const std::map<unsigned int, std::string>& GlInputsOutputs::inputs() const
    {
        return _inputs;
    }

    inline const std::map<unsigned int, std::string>& GlInputsOutputs::outputs() const
    {
        return _outputs;
    }


    // Modifiers
    inline bool GlInputsOutputs::setInput(unsigned int index, const std::string& name)
    {
        return _inputs.insert(make_pair(index, name)).second;
    }

    inline bool GlInputsOutputs::setOutput(unsigned int index, const std::string& name)
    {
        return _outputs.insert(make_pair(index, name)).second;
    }

}

#endif // CELLARWORKBENCH_GLINPUTSOUTPUTS_H
