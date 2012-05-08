//      ImageScrollArea.cpp
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


#include <ImageScrollArea.h>

/**
 * \class ImageScrollArea
 * \brief The ImageScrollArea class is a widget displaying an image and providing shortcuts to scale up/down the image.
 * 
 *  - Wheel : scroll vertically 
 *  - Shift + Wheel : scroll horizontally
 *  - Ctrl + Wheel : zoom
 */

/**
 * Constructs an image scroll area with the given \a parent.
 */
ImageScrollArea::ImageScrollArea(QWidget *parent):
    QScrollArea(parent),
    imageLabel(new QLabel),
    scaleFactor(1.0)
{
    setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    
    imageLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    
    setWidget(imageLabel);
    scaleImage(1.0);
}

/**
 * Returs a pointer to the QLabel displaying the image.
 */
QLabel* ImageScrollArea::image()
{
    return imageLabel;
}

/**
 * Handles wheel events to navigate and zoom inside the scroll area.
 */
void ImageScrollArea::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    
    if (event->modifiers() & Qt::ShiftModifier)
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 3*numSteps*horizontalScrollBar()->singleStep());
    else if (event->modifiers() & Qt::ControlModifier)
        scaleImage(1.0+(double(numSteps)/10.0));
    else 
        verticalScrollBar()->setValue(verticalScrollBar()->value() - 3*numSteps*verticalScrollBar()->singleStep());
    
    event->accept();
}

/**
 * Scales the image.
 * 
 * If \a relative is true, image current scale is multiplied by \a factor. 
 * Otherwise, image current scale is replaced by \a factor.
 */
void ImageScrollArea::scaleImage(double factor, bool relative)
{
    if(imageLabel->pixmap())
    {
        if (relative)
            scaleFactor *= factor;
        else
            scaleFactor = factor;
        imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

        adjustScrollBar(horizontalScrollBar(), factor, relative);
        adjustScrollBar(verticalScrollBar(), factor, relative);
        
        emit scaleChanged(scaleFactor);
    }
}


/**
 * Adjusts scroll bars position when image scale is modified.
 */
void ImageScrollArea::adjustScrollBar(QScrollBar *scrollBar, double factor, bool relative)
{
    if (relative)
        scrollBar->setValue(int(factor * scrollBar->value()
                                + ((factor - 1) * scrollBar->pageStep()/2)));
    else
        scrollBar->setValue((scrollBar->maximum() - scrollBar->minimum())/2);
        
}

/**
 * \fn void ImageScrollArea::scaleChanged()
 * 
 * This signal is emitted when image scale is modified.
 */
