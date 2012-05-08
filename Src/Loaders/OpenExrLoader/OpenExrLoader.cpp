//      OpenExrLoader.cpp
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


#include <OpenExrLoader.h>
#include <ImfArray.h>
#include <ImfChromaticities.h>
#include <ImfStandardAttributes.h>
#include <ImfRgbaYca.h>

#define tr(s) QObject::tr(s)

Q_EXPORT_PLUGIN2(TT_OpenExrLoader, OpenExrLoaderFactory)

/**
 * \class OpenExrLoader
 * \brief The OpenExrLoader class can load OpenEXR images.
 * 
 * This class is an implementation of ImageLoader interface.
 */

OpenExrLoader::OpenExrLoader() 
	: file(NULL)
{
    
}

/**
 * Returns the name of the loader: OpenEXR Loader.
 */
QString OpenExrLoader::name() const
{
    return tr("OpenEXR Loader");
}

/**
 * Sets the name of the image to be opened to \a fileName.
 */
void OpenExrLoader::setFileName(const QString& f)
{
    fileName = f;
}

/**
 * Opens the image file and returns true if it was successfully opened, otherwise returns false.
 */
bool OpenExrLoader::openFile()
{
    delete file;
    try
    {
        file = new Imf::RgbaInputFile(fileName.toStdString().c_str());
        return true;
    }
    catch(const std::exception&)
    {
        return false;
    }
}

/**
 * Returns the size of the image.
 */
QSize OpenExrLoader::getSize()
{
    //try
    //{
    //    dw = file->header().dataWindow();
    //    width = dw.max.x - dw.min.x + 1;
    //    height = dw.max.y - dw.min.y + 1;
    //    return QSize(width,height);
    //}
    //catch(const std::exception&)
    //{
        return QSize(0,0);
    //}
}

/**
 * Returns a pointer to image data in Yxy color space.
 */
Color* OpenExrLoader::getData()
{
    //Imf::Array2D<Imf::Rgba> pixels;
    //    
    //pixels.resizeErase (height, width);
    //file->setFrameBuffer (&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
    //file->readPixels (dw.min.y, dw.max.y);

    //Imf::Chromaticities cr; // has default values

    //if (Imf::hasChromaticities(file->header()))
    //    cr = chromaticities(file->header());
    //    
    //Imath::M44f m = Imf::RGBtoXYZ(cr, 1);
    //
    //Color* data = new Color[width*height];
    //float w;
    //for (int i=0; i<height; ++i)
    //    for (int j=0; j<width; ++j)
    //    {
    //        Imath::V3f xyz = Imath::V3f (pixels[i][j].r, pixels[i][j].g, pixels[i][j].b) * m;

    //        if((w = xyz[0] + xyz[1] + xyz[2]) > 0.0)
    //            data[i*width + j] = Color(xyz[1],     // Y
    //                                      xyz[0]/w,   // x
    //                                      xyz[1]/w);  // y
    //        else
    //            data[i*width + j] = Color(0.0,0.0,0.0);

    //    }

    //return data;
	return NULL;
}

/**
 * Returns the color space of the data returned by getData(): HdrImage::Yxy.
 */
HdrImage::ColorSpace OpenExrLoader::getColorSpace()
{
    return HdrImage::Yxy;
}

//----------------------------------------------------------------------

/**
 * \class OpenExrLoaderFactory
 * \brief The OpenExrLoaderFactory class creates instances of OpenExrLoader.
 * 
 * This class is an implementation of ImageLoaderFactory.
 */


/**
 * Returns the list of file extensions the loader can read: exr.
 */
QStringList OpenExrLoaderFactory::extensions() const 
{
    return QStringList() << "exr";
}

/**
 * Returns a newly created loader.
 */
ImageLoaderPtr OpenExrLoaderFactory::createLoader(const QString& fileName) const
{
    ImageLoaderPtr loader(new OpenExrLoader);
    loader->setFileName(fileName);
    return loader;
}


