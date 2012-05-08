//      ImageLoaderManager.h
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


#ifndef IMAGELOADERMANAGER_H
#define IMAGELOADERMANAGER_H

#include <QtGui>
#include <Singleton.h>
#include <HdrImage.h>
#include <ImageLoader.h>
#include <Exports.h>

class ImageLoaderManager; // get an error without that... why??

class TT_API ImageLoaderManager : public Singleton<ImageLoaderManager>
{
    friend class Singleton<ImageLoaderManager>;

public:
    ImageLoaderPtr getLoader(const QString &fileName);
    void registerLoaders(const QString &directory);
    bool empty();
    
private:
    QMap <QString, ImageLoaderFactory*> loaders;
};

#endif
