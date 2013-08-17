//      ReinhardGlobalOperator.cpp
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


#include <ReinhardGlobalOperator.h>
#include <Exceptions.h>
#include <math.h>
#include <Profiler.h>

#define OPERATOR_NAME "Reinhard Global Operator"

#ifndef QT_NO_DEBUG
	Q_EXPORT_PLUGIN2(TT_ReinhardGlobalOperator, ReinhardGlobalOperatorFactory)
#else
	Q_EXPORT_PLUGIN2(TT_ReinhardGlobalOperatord, ReinhardGlobalOperatorFactory)
#endif

/**
 * \class ReinhardGlobalOperator
 * \brief The ReinhardGlobalOperator class is an implementation of Erik 
 * Reinhard's global tone mapping operator.
 * 
 * This class implements ToneMappingOperator interface.
 */
 

ReinhardGlobalOperator::ReinhardGlobalOperator() :
    inputImage(NULL),
    outputImage(NULL),
    avLum(0.0),
    keyValue(0.18f),
    burnOut(false),
    lumWhite2(1.0f)
{
    
}

void ReinhardGlobalOperator::setupUi(QWidget* parent)
{
    ui.setupUi(parent);
    
    connect(ui.keyValueSlider, SIGNAL(sliderReleased()), this, SLOT(toneMap()));
    connect(ui.keyValueSlider, SIGNAL(valueChanged(int)), this, SLOT(updateKeyValue(int)));
    connect(ui.burnOutGroupBox, SIGNAL(toggled(bool)), this, SLOT(updateBurnOut(bool)));
    connect(ui.whiteValueSpinBox, SIGNAL(editingFinished()), this, SLOT(updateLumWhite()));
    connect(ui.whiteValueOkButton, SIGNAL(clicked()), this, SLOT(toneMap()));
}

/**
 * Returns the operator name: Reinhard Global Operator.
 */
QString ReinhardGlobalOperator::name() const
{
    return tr(OPERATOR_NAME);
}

const HdrImage* ReinhardGlobalOperator::getToneMappedImage() const
{
    return outputImage;
}

/**
 * Sets the image to be tone mapped.
 * 
 * This function also computes average luminance and then calls toneMap().
 */
void ReinhardGlobalOperator::setImage(const HdrImage* image)
{
	if (!image->hasY())
        throw Exception(tr("Image passed to %1 does not contains Y data. Cannot turn water into wine.").arg(name()));
    
    QTime t;
    t.start();
    
	{
		PROFILE_FUNC();

		inputImage = image;
		QSize size = inputImage->size();
		delete outputImage;
		outputImage = new HdrImage(*inputImage);
		const double delta = 0.0001;
    
		float lumMax = 0.0;
		int Y = inputImage->YIndex();
		avLum = 0.0;
		for(int i=0; i<size.height(); ++i)
			for(int j=0; j<size.width(); ++j)
			{
				double lum = qMax(double((*inputImage)[i][j][Y]),0.0);
				avLum += log(delta + lum);
				lumMax = qMax(lumMax,float(lum));
			}
		avLum = exp(avLum/(double)(size.width()*size.height()));
    
		ui.whiteValueSpinBox->setMaximum(qMin(lumMax,float(1e19)));
		ui.whiteValueSpinBox->setValue(qMin(lumMax,float(1e19)));
		ui.keyValueSlider->setValue(18); // = 0.18
		ui.burnOutGroupBox->setChecked(false);
    
		msg = tr("Operator Init: %1 ms").arg(t.elapsed());
	}
    toneMap();
}

/**
 * Tone maps the image.
 * 
 * This function emit imageUpdated() when tone mapping is finished.
 */
void ReinhardGlobalOperator::toneMap()
{
	if (inputImage)
    {
		QTime t;
        t.start();
        
		{
			PROFILE_FUNC();

			QSize size = inputImage->size();
			int width = size.width();
			int height = size.height();
			int Y = inputImage->YIndex();

			if(burnOut)
				for(int i=0; i<height; ++i)
					for(int j=0; j<width; ++j)
					{
						float lum = (*inputImage)[i][j][Y];
						lum = keyValue*lum/avLum;
						(*outputImage)[i][j][Y] = lum*(1.0+(lum/lumWhite2))/(1.0+lum);

					}
			else
				for(int i=0; i<height; ++i)
					for(int j=0; j<width; ++j)
					{
						float lum = (*inputImage)[i][j][Y];
						lum = keyValue*lum/avLum;
						(*outputImage)[i][j][Y] = lum/(1.0+lum);

					}
		}
        emit message(msg + tr("  Tone Mapping: %1 ms").arg(t.elapsed()));
                
        emit imageUpdated();
    }
}

/**
 * Reads the key value from the user interface.
 * 
 * This function is typically called when the key value slider is moved.
 */
void ReinhardGlobalOperator::updateKeyValue(int value)
{
    keyValue = float(value)/100.0;
    ui.keyValue->setText(QString("%1").arg(keyValue, 0, 'f', 2));
}

/**
 * Enable or disable burn out.
 * 
 * This function is typically called when the burn out checkbox is clicked. 
 * It also calls updateLumWhite().
 */
void ReinhardGlobalOperator::updateBurnOut(bool enabled)
{
    burnOut = enabled;
    updateLumWhite();
}

/**
 * Reads the white value from the user interface.
 * 
 * This function is typically called when the spin box value is modified. 
 * It also calls toneMap().
 */
void ReinhardGlobalOperator::updateLumWhite()
{
    float value = ui.whiteValueSpinBox->value();
    lumWhite2 = value*value;
    toneMap();
}

//----------------------------------------------------------------------

/**
 * \class ReinhardGlobalOperatorFactory
 * \brief The ReinhardGlobalOperatorFactory class creates instances of ReinhardGlobalOperator.
 * 
 * This class is an implementation of ToneMappingOperatorFactory.
 */

/**
 * Returns a newly created ReinhardGlobalOperator.
 */
ToneMappingOperatorPtr ReinhardGlobalOperatorFactory::createOperator() const
{
    return ToneMappingOperatorPtr(new ReinhardGlobalOperator);
}

/**
 * Returns the operator name: Reinhard Global Operator.
 */
QString ReinhardGlobalOperatorFactory::operatorName() const
{
    return tr(OPERATOR_NAME);
}
