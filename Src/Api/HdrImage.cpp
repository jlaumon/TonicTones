//      HdrImage.cpp
//      
//      Copyright 2010 Jérémy Laumon <jeremy.laumon@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.


#include <HdrImage.h>
#include <ImageLoaderManager.h>
#include <Exceptions.h>
#include <Profiler.h>

#define tr(arg) QObject::tr(arg)

/**
 * \class HdrImage
 * \brief The HdrImage class is a representation of an HDR image.
 * 
 * The image data is stored as an array of Color and is accessible through the operator [].
 * 
 * This class can handle several color spaces and provides transformations between them.
 * At the moment, only RGB and Yxy color spaces are available.
 */

/**
 * Position of Y element for each color space.
 */
int HdrImage::YIndices[] =
{
    -1, // NONE
    -1, // RGB
     0  // Yxy
};

/**
 * Constructs an empty image.
 */
HdrImage::HdrImage() :
    width(0),
    height(0),
    data(NULL),
    null(true),
    space(HdrImage::NONE)
{
    
}

/**
 * Constructs a copy of \a im.
 */
HdrImage::HdrImage(const HdrImage& im)
{
    width = im.width;
    height = im.height;
    null = im.null;
    space = im.space;
    
    data = new Color[width*height];
    for(int i=0; i<width*height; ++i)
        data[i] = im.data[i];
}

/**
 * Constructs an image and tries to load the image from the file with the given \a fileName.
 */
HdrImage::HdrImage(const QString& fileName) :
    width(0),
    height(0),
    data(NULL),
    null(true),
    space(HdrImage::NONE)
{
    load(fileName);
}

HdrImage::~HdrImage()
{
    delete[] data;
}

/**
 * Returns the size of the image.
 */
QSize HdrImage::size() const
{
    return QSize(width, height);
}

/**
 * Returns the color space of the image.
 */
HdrImage::ColorSpace HdrImage::colorSpace() const
{
    return space;
}

/**
 * Returns a RGB copy of the image or throw an Exception if the transformation from the current color space is not implemented.
 */
HdrImage* HdrImage::toRgb(const float toRgbMatrix[3][3]) const
{
    HdrImage* image = NULL;
    switch (space)
    {
        case HdrImage::Yxy :
            image = fromYxyToRgb(toRgbMatrix);
            break;
        case HdrImage::RGB:
            image = new HdrImage(*this);
            break;
        default:
            throw Exception(tr("Transformation from the current color space to RGB not implemented yet."));
            break;
    }
    return image;
}

/**
 * Returns a RGB copy of a Yxy image.
 */
HdrImage* HdrImage::fromYxyToRgb(const float matrix[3][3]) const
{
	PROFILE_FUNC();

    HdrImage* image = new HdrImage;
    image->resize(width, height);
    image->space = HdrImage::RGB;
    float xyz[3];
    #define X xyz[0]
    #define Y xyz[1]
    #define Z xyz[2]
    int ii, jj, maxI = width*height;
    for(int i=0; i<maxI; ++i)
    {
        Y = data[i][0];
        if((Y > 0.0) && (data[i][1] > 0.0) && (data[i][2] > 0.0))
        {
            X = (data[i][1] * Y) / data[i][2];
            Z = (X / data[i][1]) - X - Y;
        }
        else
            X = Z = 0.0;
        
        float rgb[3] = {0,0,0};
        for (ii = 0; ii < 3; ii++)
            for (jj = 0; jj < 3; jj++)
                rgb[ii] += matrix[ii][jj] * xyz[jj];
        image->data[i].set(rgb[0],rgb[1],rgb[2]);
    }
    #undef X
    #undef Y
    #undef Z
    return image;
}

/**
 * Deletes current image data and creates new ones of given width and height.
 */
void HdrImage::resize(int w, int h) 
{
    delete[] data;
    
    width = w;
    height = h;
    data = new Color[width * height];
    space = HdrImage::NONE;
    null = false;
}

/**
 * Loads an image from the file with the given fileName or throw an Exception if the file cannot be loaded.
 */
void HdrImage::load(const QString& fileName) 
{
    ImageLoaderPtr loader = ImageLoaderManager::instance()->getLoader(fileName);
    if (!loader->openFile())
        throw Exception(tr("Error while loading %1: %2 was unable to load the file.").arg(QDir(fileName).dirName()).arg(loader->name()));
    
    QSize size = loader->getSize();
    width = size.width();
    height = size.height();
    
    space = loader->getColorSpace();
    
    delete[] data;
    data = loader->getData();
    
    if (width < 1 || height < 1 || data == NULL)
    {
        width = 0;
        height = 0;
        delete[] data;
        null = true;
        space = HdrImage::NONE;
        throw Exception(tr("Error while loading %1: %2 was unable to load the file.").arg(QDir(fileName).dirName()).arg(loader->name()));
    }
    
    null = false;
    qDebug("%s loaded.", fileName.toStdString().c_str());
}

/**
 * Returns true if it is a null image, otherwise returns false.
 */
bool HdrImage::isNull() const
{
    return null;
}  

/**
 * Returns a pointer to the row \a i of data.
 */
Color* HdrImage::operator [](int i)
{
    return &data[i*width];
}

/**
 * Returns a constant pointer to the row \a i of data.
 */
const Color* HdrImage::operator [](int i) const
{
    return &data[i*width];
}

/**
 * Returns true if the image color space contains luminance data, otherwise returns false.
 */
bool HdrImage::hasY() const
{
    return YIndices[space]>=0;
}

/**
 * Returns the luminance index of the image color space.
 */
int HdrImage::YIndex() const
{
    return YIndices[space];
}

