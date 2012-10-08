//      TonicTones.h
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

#ifndef TONICTONES_H
#define TONICTONES_H

#include <QtGui>
#include <ui_TonicTones.h>
#include <ui_ScreenColorsDialog.h>
#include <HdrImage.h>
#include <ImageScrollArea.h>
#include <ImageLoaderManager.h>
#include <ToneMappingOperatorManager.h>

class TonicTones : public QMainWindow, private Ui::TonicTones
{
    Q_OBJECT
public:
    TonicTones(QWidget *parent = 0);

public slots:
    void open();
    void openScreenColorsDialog();
    void updateScreenColors();
    void updateImage();
    void updateZoom(double scaleFactor);
    void updateOperator(const QString& operatorName);
    void updateGamma(int value);
    void displayImage() const;
    void enableOperator(bool enabled);
    void enableGamma(bool enabled);
    
private:

    Ui::ScreenColorsDialog uiScreenColors;
    
    ImageScrollArea* m_scrollArea;
    QLabel* m_zoomLabel;
    QLabel* m_operatorLabel;
    QLabel* m_conversionTimeLabel;
    QLabel* m_displayTimeLabel;
    
    HdrImage* m_inputImage; 
    HdrImage* m_outputImage; 
	int* m_pixmapBuffer;
    float m_xyzToRgbMatrix[3][3];
    float m_gamma;
    bool m_gammaEnabled;
    bool m_operatorEnabled;
    ImageLoaderManager &m_loaderManager;
    ToneMappingOperatorManager &m_operatorManager;
};

#endif
