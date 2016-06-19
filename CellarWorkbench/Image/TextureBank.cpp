#include "TextureBank.h"

#include <CellarWorkbench/Misc/Log.h>

#include "Texture.h"
#include "ImageBank.h"
#include "Image.h"

using namespace std;
using namespace cellar;


namespace cellar
{
    const std::string TextureBank::LINEARIZED_FLAG = "<linearized>";

    TextureBank& getTextureBank()
    {
        return *TextureBank::getInstance();
    }


    TextureBank::TextureBank() :
        _textures()
    {
        std::shared_ptr<Texture> blank(new Texture(
            getImageBank().getImage("blank"), false));

        _textures[""] = blank;
        _textures["blank"] = blank;
        _textures["" + LINEARIZED_FLAG] = blank;
        _textures["blank" + LINEARIZED_FLAG] = blank;
    }

    bool TextureBank::isInBank(
            const std::string& texName,
            bool linearized) const
    {
        std::string bankName = texName;
        if(linearized) bankName += LINEARIZED_FLAG;

        return _textures.find(bankName) != _textures.end();
    }

    bool TextureBank::addTexture(
            const std::string& texName,
            bool linearized)
    {
        std::string imgName = texName;
        std::shared_ptr<Texture> tex(new Texture(
            getImageBank().getImage(imgName), linearized));

        std::string bankName = texName;
        if(linearized) bankName += LINEARIZED_FLAG;

        return _textures.insert(make_pair(bankName, tex)).second;
    }

    std::shared_ptr<Texture> TextureBank::getTexturePtr(
            const std::string& texName,
            bool linearized)
    {
        std::string bankName = texName;
        if(linearized) bankName += LINEARIZED_FLAG;

        auto it = _textures.find(bankName);

        if(it != _textures.end())
        {
            return it->second;
        }

        getLog().postMessage(new Message('I', false,
            "Implicitly adding : " + bankName, "TextureBank"));
        bool isAdded = addTexture(texName, linearized);

        if(isAdded)
        {
            return getTexturePtr(texName, linearized);
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "Failed to implicitly add : " + bankName, "TextureBank"));

            return getTexturePtr("blank", linearized);
        }
    }
}
