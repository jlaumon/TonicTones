//      ReinhardLocalOperator.h
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

#ifndef REINHARDLOCALOPERATOR_H
#define REINHARDLOCALOPERATOR_H

#include <QtGui>
#include <ToneMappingOperator.h>
#include <fftw3.h>
#include <ui_ReinhardLocalOperator.h>

class ReinhardLocalOperator : public ToneMappingOperator
{
    Q_OBJECT
public:
    ReinhardLocalOperator();
    virtual ~ReinhardLocalOperator();

    virtual void setupUi(QWidget* widget);
    virtual const HdrImage* getToneMappedImage() const;
    virtual void setImage(const HdrImage *image);
    virtual QString name() const;
    
public slots:
    void updateKeyValue(int value);
    void updateSharpening(int value);
    void toneMap();

private:
    fftw_complex* convolveFft(fftw_complex* f1, fftw_complex* f2);

    Ui::ReinhardLocalOperator ui;
    const HdrImage* inputImage;
    HdrImage* outputImage;
    int width;
    int height;
    double avLum;
    double keyValue;
    double sharpening;
    QString msg;
    fftw_complex* responses[9];
};

class ReinhardLocalOperatorFactory : public QObject, public ToneMappingOperatorFactory
{
    Q_OBJECT
    Q_INTERFACES(ToneMappingOperatorFactory)
    
public:
    virtual ToneMappingOperatorPtr createOperator() const;
    virtual QString operatorName() const;
};

#endif
