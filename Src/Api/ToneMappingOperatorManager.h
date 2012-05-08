//      ToneMappingOperatorManager.h
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


#ifndef TONEMAPPINGOPERATORMANAGER_H
#define TONEMAPPINGOPERATORMANAGER_H

#include <QtGui>
#include <Singleton.h>
#include <HdrImage.h>
#include <ToneMappingOperator.h>
#include <Exports.h>

class ToneMappingOperatorManager; // get an error without that... why??

class TT_API ToneMappingOperatorManager : public Singleton<ToneMappingOperatorManager>
{
    friend class Singleton<ToneMappingOperatorManager>;

public:
    ToneMappingOperatorPtr getActiveOperator();
    QStringList registerOperators(const QString &directory);
    bool empty();
    void setActiveOperator(const QString& operatorName);
    
private:
    QMap <QString, ToneMappingOperatorFactory*> operators;
    ToneMappingOperatorPtr activeOperator;
};

#endif
