//      Color.h
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


#ifndef COLOR_H
#define COLOR_H

#include <QtGui>
#include <Exports.h>

class TT_API Color
{
public:
    Color();
    Color(float f1, float f2, float f3);
    
    Color& operator =(const QVector3D& vect);
    
    Color operator *(float f) const;
    
    QVector3D toVect() const;
    
    float& operator [](int i)
    {
        return val[i];
    }

    const float& operator [](int i) const
    {
        return val[i];
    }
    
    Color clamp() const;
    
    void set(float f1, float f2, float f3)
    {
        val[0] = f1;
        val[1] = f2;
        val[2] = f3;
    }
    
private:
    float val[3];
   
};

#endif
