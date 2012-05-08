//      Exceptions.cpp
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


#include <Exceptions.h>

/**
 * \class Exception
 * \brief The Exception class is a simple exception class accepting QString messages instead of const char*.
 */

Exception::Exception(const QString &m) throw()
: msg(m)
{
 
}

Exception::~Exception() throw()
{
    
}

const QString& Exception::qWhat() const throw()
{
    return msg;
}

const char* Exception::what() const throw()
{
    return msg.toStdString().c_str();
}    
