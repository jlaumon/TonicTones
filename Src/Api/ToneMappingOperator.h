//      ToneMappingOperator.h
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


#ifndef TONEMAPPINGOPERATOR_H
#define TONEMAPPINGOPERATOR_H

#include <QtGui>
#include <HdrImage.h>


/**
 * \class ToneMappingOperator
 * \brief The ToneMappingOperator class is an interface which must be implemented by tone mapping operator plugins.
 */

class ToneMappingOperator : public QObject
{
    Q_OBJECT
public:
    virtual ~ToneMappingOperator(){}
    
    /**
     * Sets up the user interface inside \a widget.
     */
    virtual void setupUi(QWidget* widget) = 0;
    
    /**
     * Returns a constant pointer to the tone mapped image.
     */
    virtual const HdrImage* getToneMappedImage() const = 0;
    
    /**
     * Sets the image to be tone mapped.
     * 
     * This function should start the tone mapping process.
     */
    virtual void setImage(const HdrImage *image) = 0;
    
    /**
     * Returns the operator name.
     */
    virtual QString name() const = 0;
signals:

    /**
     * This signal is emitted when the tone mapping algorithm has finished processing the image.
     */
    void imageUpdated() const;
    
    /**
     * This signal is emitted when the operator want to display a message.
     */
    void message(const QString& msg);
};

typedef QSharedPointer<ToneMappingOperator> ToneMappingOperatorPtr;


//----------------------------------------------------------------------

/**
 * \class ToneMappingOperatorFactory
 * \brief The ToneMappingOperatorFactory class is an interface which must 
 * be implemented and exported by tone mapping operator plugins.
 * 
 * This class is used to create ToneMappingOperator instances.
 */

class ToneMappingOperatorFactory
{
public:
    virtual ~ToneMappingOperatorFactory(){}
    
    /**
     * Returns a newly created operator.
     */
    virtual ToneMappingOperatorPtr createOperator() const = 0;
    
    /**
     * Returns the name of the operator.
     */
    virtual QString operatorName() const = 0;
};

Q_DECLARE_INTERFACE(ToneMappingOperatorFactory, "TonicTones.ToneMappingOperatorFactory/1.0")


#endif
