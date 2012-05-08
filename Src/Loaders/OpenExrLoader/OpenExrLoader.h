//      OpenExrLoader.h
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

#ifndef OPENEXRLOADER_H
#define OPENEXRLAODER_H

#include <QtGui>
#include <ImageLoader.h>
#include <ImfRgbaFile.h>

class OpenExrLoader : public ImageLoader
{
public:
    OpenExrLoader();
    
    virtual void setFileName(const QString& fileName);
    virtual bool openFile();
    virtual QSize getSize();
    virtual Color* getData();
    virtual HdrImage::ColorSpace getColorSpace();
    virtual QString name() const;
    
private:
	Imf::RgbaInputFile* file;
	QString fileName;
	int width;
	int height;
	Imath::Box2i dw;
};

class OpenExrLoaderFactory : public QObject, public ImageLoaderFactory
{
    Q_OBJECT
    Q_INTERFACES(ImageLoaderFactory)
    
public:    
    virtual QStringList extensions() const;
    virtual ImageLoaderPtr createLoader(const QString& fileName) const;
};

#endif
