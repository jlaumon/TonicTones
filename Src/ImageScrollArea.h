//      ImagScrollArea.h
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


#ifndef IMAGESCROLLAREA_H
#define IMAGESCROLLAREA_H

#include <QtGui>

class ImageScrollArea : public QScrollArea
{
    Q_OBJECT
    
public:
    ImageScrollArea(QWidget *parent=0);
    QLabel* image();
    void scaleImage(double factor, bool relative=true);

signals:
    void scaleChanged(double scaleFactor);
    
protected:
    virtual void wheelEvent(QWheelEvent *event);
    
private:
    void adjustScrollBar(QScrollBar *scrollBar, double factor, bool relative);
    
    QLabel *imageLabel; ///< QLabel in which the image is displayed
    double scaleFactor; ///< Image scale factor (1.0 means normal size)
};

#endif
