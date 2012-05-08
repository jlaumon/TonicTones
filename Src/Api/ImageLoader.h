//      ImageLoader.h
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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <Color.h>
#include <HdrImage.h>
#include <Exports.h>

/**
 * \class ImageLoader
 * \brief The ImageLoader class is an interface which must be implemented by image loader plugins.
 */

class TT_API ImageLoader
{
public:
    virtual ~ImageLoader(){}
    
    /**
     * Sets the name of the image to be opened to \a fileName.
     */
    virtual void setFileName(const QString& fileName) = 0;
    
    /**
     * Opens the image file and returns true if it was successfully opened, otherwise returns false.
     */
    virtual bool openFile() = 0;
    
    /**
     * Returns the size of the image or QSize(0,0) if size cannot be read.
     */
    virtual QSize getSize() = 0;
    
    /**
     * Returns a pointer to image data or NULL if data cannot be read.
     */
    virtual Color* getData() = 0;
    
    /**
     * Returns the color space of the data returned by getData().
     */
    virtual HdrImage::ColorSpace getColorSpace() = 0;
    
    /**
     * Returns the name of the loader.
     */
    virtual QString name() const = 0;
};

typedef QSharedPointer<ImageLoader> ImageLoaderPtr;


//----------------------------------------------------------------------

/**
 * \class ImageLoaderFactory
 * \brief The ImageLoaderFactory class is an interface which must be implemented and exported by image loader plugins.
 * 
 * This class is used to create ImageLoader instances.
 */
class TT_API ImageLoaderFactory
{
public:
    virtual ~ImageLoaderFactory(){}
    
    /**
     * Returns the list of file extensions the loader can read.
     */
    virtual QStringList extensions() const = 0;
    
    /**
     * Returns a newly created loader.
     */
    virtual ImageLoaderPtr createLoader(const QString &fileName) const = 0;
};

Q_DECLARE_INTERFACE(ImageLoaderFactory, "TonicTones.ImageLoaderFactory/1.1")


#endif
