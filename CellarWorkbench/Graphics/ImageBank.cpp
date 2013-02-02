#include "ImageBank.h"
#include "Image.h"
#include "Misc/Log.h"

using namespace std;


namespace cellar
{

    ImageBank& getImageBank()
    {
        return *ImageBank::getInstance();
    }


    ImageBank::ImageBank() :
        _images()
    {
        unsigned char* whitePixel = new unsigned char[4];
        whitePixel[0] = 255; whitePixel[1] = 255;
        whitePixel[2] = 255; whitePixel[3] = 255;
        Image blank = Image(whitePixel, 1, 1);

        addImage("", blank);
        addImage("blank", blank);
    }

    bool ImageBank::addImage(
            const std::string& imgName,
            const std::string& fileName)
    {
        Image img;
        if(img.load( imgName ))
        {
            return addImage(imgName, img);
        }

        getLog().postMessage(new Message('E', false,
            imgName + " image not found at : " + fileName, "ImageBank"));

        return false;
    }

    bool ImageBank::addImage(
            const std::string& imgName,
            const Image& image)
    {
        bool isAdded = _images.insert(
            make_pair(imgName, make_shared<Image>(image)) ).second;

        if(isAdded)
        {
            getLog().postMessage(new Message('I', false,
                "'" + imgName + "' successfully added to image bank", "ImageBank"));
        }
        else
        {
            getLog().postMessage(new Message('W', false,
                "An image named '" + imgName + "' is already in the bank", "ImageBank"));
        }

       return isAdded;
    }

    bool ImageBank::deleteImage(
            const std::string& imgName)
    {        
        int nbRemoved = _images.erase(imgName);

        if(0 < nbRemoved)
        {
            getLog().postMessage(new Message('I', false,
                "Image deleted : " + imgName, "ImageBank"));
        }
        else
        {
            getLog().postMessage(new Message('W', false,
                "Image not found so not deleted : " + imgName, "ImageBank"));
        }

        return nbRemoved;
    }

    Image& ImageBank::getImage(
            const std::string& imgName)
    {
        return *getImagePtr(imgName);
    }

    std::shared_ptr<Image> ImageBank::getImagePtr(
            const std::string& imgName)
    {
        auto it = _images.find(imgName);

        if(it != _images.end())
        {
            return it->second;
        }

        getLog().postMessage(new Message('I', false,
            "Implicitly adding : " + imgName, "ImageBank"));
        bool isAdded = addImage(imgName, imgName);

        if(isAdded)
        {
            return getImagePtr(imgName);
        }
        else
        {
            getLog().postMessage(new Message('E', false,
                "Failed to implicitly add : " + imgName, "ImageBank"));
            addImage(imgName, getImage("blank"));

            return getImagePtr("blank");
        }
    }
}
