//      Singleton.h
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

#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * \class Singleton
 * \brief A singleton design pattern implementation.
 * 
 * A singleton has a unique instance accessible through the method instance().
 */

template <typename T>
class Singleton
{
protected:
  Singleton() {}
  ~Singleton() {}

public:

    /**
     * Returns a pointer to the unique instance of the class or create one if there is none.
     */
    static T *instance()
    {
        if (NULL == inst)
        {
            inst = new T;
        }
        return (static_cast<T*> (inst));
    }

    /**
     * Destroy the unique instance of the class.
     */
    static void destroy()
    {
        if (NULL != inst)
        {
            delete inst;
            inst = NULL;
        }
    }

private:
    static T *inst;
    
    Singleton(const Singleton&);
    Singleton& operator =(const Singleton&);
};

template <typename T>
T *Singleton<T>::inst = NULL;

#endif
