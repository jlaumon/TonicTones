//      ReinhardGlobalOperator.h
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

#ifndef REINHARDGLOBALOPERATOR_H
#define REINHARDGLOBALOPERATOR_H

#include <QtGui>
#include <ToneMappingOperator.h>
#include <ui_ReinhardGlobalOperator.h>

class ReinhardGlobalOperator : public ToneMappingOperator
{
    Q_OBJECT
public:
    ReinhardGlobalOperator();

    virtual void setupUi(QWidget* widget);
    virtual const HdrImage* getToneMappedImage() const;
    virtual void setImage(const HdrImage *image);
    virtual QString name() const;
    
public slots:
    void updateKeyValue(int value);
    void updateBurnOut(bool enabled);
    void updateLumWhite();
    void toneMap();

private:

    Ui::ReinhardGlobalOperator ui;
    const HdrImage* inputImage;
    HdrImage* outputImage;
    double avLum;
    float keyValue;
    bool burnOut;
    float lumWhite2;
    QString msg;
    
};

class ReinhardGlobalOperatorFactory : public QObject, public ToneMappingOperatorFactory
{
    Q_OBJECT
    Q_INTERFACES(ToneMappingOperatorFactory)
    
public:
    virtual ToneMappingOperatorPtr createOperator() const;
    virtual QString operatorName() const;
};

#endif
