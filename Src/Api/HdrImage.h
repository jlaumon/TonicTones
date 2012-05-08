//      HdrImage.h
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


#ifndef HDRIMAGE_H
#define HDRIMAGE_H

#include <QtGui>
#include <Color.h>
#include <Exports.h>

class TT_API HdrImage
{    
    
public:
    enum ColorSpace
    {
        NONE,
        RGB,
        Yxy
    };

    HdrImage();
    HdrImage(const HdrImage& im);
    HdrImage(const QString& fileName);
    ~HdrImage();
    QSize size() const;
    ColorSpace colorSpace() const;
    bool isNull() const;
    HdrImage* toRgb(const float mXyzToRgb[3][3]) const;
    void load(const QString &fileName);
    Color* operator[] (int i);
    const Color* operator[] (int i) const;
    bool hasY() const;
    int YIndex() const;
    
private:
    void resize(int width, int height);
    HdrImage* fromYxyToRgb(const float m[3][3]) const;

    int width;
    int height;
    Color* data;
    bool null;
    ColorSpace space;
    float pMin;
    float pMax;
    
    static int YIndices[];
};

#endif
