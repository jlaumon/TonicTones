//      Color.cpp
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


#include <Color.h>

/**
 * \class Color
 * \brief The Color class is a simple class to handle a color as 3 float values.
 */


Color::Color(){}

Color::Color(float f1, float f2, float f3)
{
    val[0] = f1;
    val[1] = f2;
    val[2] = f3;
}

Color& Color::operator =(const QVector3D& vect)
{
    val[0] = vect.x();
    val[1] = vect.y();
    val[2] = vect.z();
    
    return *this;
}

Color Color::operator *(float f) const
{
    return Color (val[0] * f,
                  val[1] * f,
                  val[2] * f);
}

QVector3D Color::toVect() const
{
    return QVector3D(val[0],val[1],val[2]);
}

Color Color::clamp() const
{
    float min = 0.0;
    float max = 1.0;
    return Color(qBound(min,val[0],max),
                 qBound(min,val[1],max),
                 qBound(min,val[2],max));
}
