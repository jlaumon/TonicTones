//      LdrFormatsLoader.cpp
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


#include <LdrFormatsLoader.h>
#include <Profiler.h>

#define tr(s) QObject::tr(s)

Q_EXPORT_PLUGIN2(TT_LdrFormatsLoader, LdrFormatsLoaderFactory)

/**
 * \class LdrFormatsLoader
 * \brief The LdrFormatsLoader class can load almost all "normal" image formats.
 * 
 * This class is an implementation of ImageLoader interface.
 */

/**
 * Returns the name of the loader: LDR Formats Loader.
 */
QString LdrFormatsLoader::name() const
{
    return tr("LDR Formats Loader");
}

/**
 * Sets the name of the image to be opened to \a fileName.
 */
void LdrFormatsLoader::setFileName(const QString& f)
{
    fileName = f;
}

/**
 * Opens the image file and returns true if it was successfully opened, otherwise returns false.
 */
bool LdrFormatsLoader::openFile()
{
	PROFILE_FUNC();

	return image.load(fileName);
}

/**
 * Returns the size of the image.
 */
QSize LdrFormatsLoader::getSize()
{
    return image.size();
}

/**
 * Returns a pointer to image data in Yxy color space.
 */
Color* LdrFormatsLoader::getData()
{
	PROFILE_FUNC();

    QSize size = image.size();
    int width = size.width();
    int height = size.height();
    
    float rgbToXyzMatrix  [3][3] = {{0.5141364f, 0.3238786f,  0.16036376f},
                                    {0.265068f,  0.67023428f, 0.06409157f},
                                    {0.0241188f, 0.1228178f,  0.84442666f}};
    
    Color* data = new Color[width*height];
    float w;
    int ii, jj;
    for (int i=0; i<height; ++i)
        for (int j=0; j<width; ++j)
        {
            QRgb pixel = image.pixel(j,i);
            float rgb[3] = {float(qRed(pixel))/255.0,
                            float(qGreen(pixel))/255.0,
                            float(qBlue(pixel))/255.0};
            float xyz[3] = {0.0, 0.0, 0.0};
            
            for (ii = 0; ii < 3; ++ii)
                for (jj = 0; jj < 3; ++jj)
                    xyz[ii] += rgbToXyzMatrix[ii][jj] * rgb[jj];
            
            if((w = xyz[0] + xyz[1] + xyz[2]) > 0.0)
                data[i*width + j] = Color(xyz[1],     // Y
                                          xyz[0]/w,   // x
                                          xyz[1]/w);  // y
            else
                data[i*width + j] = Color(0.0,0.0,0.0);

        }

    return data;
}

/**
 * Returns the color space of the data returned by getData(): HdrImage::Yxy.
 */
HdrImage::ColorSpace LdrFormatsLoader::getColorSpace()
{
    return HdrImage::Yxy;
}


//----------------------------------------------------------------------


/**
 * \class LdrFormatsLoaderFactory
 * \brief The LdrFormatsLoaderFactory class creates instances of LdrFormatsLoader.
 * 
 * This class is an implementation of ImageLoaderFactory.
 */


/**
 * Returns the list of file extensions the loader can read: bmp, gif, jpg, png, tiff.
 */
QStringList LdrFormatsLoaderFactory::extensions() const 
{
    return QStringList() << "bmp" << "gif" << "jpg" << "jpeg" << "png" << "tiff";
}

/**
 * Returns a newly created loader.
 */
ImageLoaderPtr LdrFormatsLoaderFactory::createLoader(const QString& fileName) const
{
    ImageLoaderPtr loader(new LdrFormatsLoader);
    loader->setFileName(fileName);
    return loader;
}


