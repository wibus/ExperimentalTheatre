#ifndef CELLARWORKBENCH_TEXTUREBANK_H
#define CELLARWORKBENCH_TEXTUREBANK_H

#include <map>
#include <string>
#include <memory>
#include <stdexcept>

#include <CellarWorkbench/DesignPattern/Singleton.h>


#include "../libCellarWorkbench_global.h"


namespace cellar
{
    class Texture;

    class CELLAR_EXPORT TextureBank : public cellar::Singleton<TextureBank>
    {
    private:
        friend class cellar::Singleton<TextureBank>;
        TextureBank();

    public:
        bool isInBank(
                const std::string& texName,
                bool linearized) const;

        bool addTexture(
                const std::string& texName,
                bool linearized);

        std::shared_ptr<Texture> getTexturePtr(
                const std::string& texName,
                bool linearized);


    private:
        static const std::string LINEARIZED_FLAG;

        std::map< std::string, std::shared_ptr< Texture > > _textures;
    };

    CELLAR_EXPORT TextureBank& getTextureBank();
}

#endif // CELLARWORKBENCH_TEXTUREBANK_H
