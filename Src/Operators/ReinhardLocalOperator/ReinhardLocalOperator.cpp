//      ReinhardLocalOperator.cpp
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


#include <ReinhardLocalOperator.h>
#include <Exceptions.h>
#include <math.h>
#include <Profiler.h>

#define OPERATOR_NAME "Reinhard Local Operator"

#ifndef QT_NO_DEBUG
	Q_EXPORT_PLUGIN2(TT_ReinhardLocalOperator, ReinhardLocalOperatorFactory)
#else
	Q_EXPORT_PLUGIN2(TT_ReinhardLocalOperatord, ReinhardLocalOperatorFactory)
#endif

/**
 * \class ReinhardLocalOperator
 * \brief The ReinhardLocalOperator class is an implementation of Erik 
 * Reinhard's local tone mapping operator.
 * 
 * This class implements ToneMappingOperator interface. 
 * 
 * FFTW library is used to calculate convolutions efficiently.
 */

ReinhardLocalOperator::ReinhardLocalOperator() :
    inputImage(NULL),
    outputImage(NULL),
    width(0),
    height(0),
    keyValue(0.18),
    sharpening(8.0)
{
    for(int k=0; k<9; ++k)
    {
        responses[k] = NULL;
    }
}

ReinhardLocalOperator::~ReinhardLocalOperator()
{
    for(int k=0; k<9; ++k)
    {
		if (responses[k])
			fftw_free(responses[k]);
    }
}

/**
 * Returns the operator name: Reinhard Local Operator.
 */
QString ReinhardLocalOperator::name() const
{
    return tr(OPERATOR_NAME);
}

void ReinhardLocalOperator::setupUi(QWidget* parent)
{
    ui.setupUi(parent);
    
    connect(ui.keyValueSlider, SIGNAL(sliderReleased()), this, SLOT(toneMap()));
    connect(ui.keyValueSlider, SIGNAL(valueChanged(int)), this, SLOT(updateKeyValue(int)));
    
    connect(ui.sharpeningSlider, SIGNAL(sliderReleased()), this, SLOT(toneMap()));
    connect(ui.sharpeningSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSharpening(int)));
}

const HdrImage* ReinhardLocalOperator::getToneMappedImage() const
{
    return outputImage;
}

/**
 * Sets the image to be tone mapped.
 * 
 * This function also initializes the operator by computing :
 * -# the average luminance
 * -# the 9 required gaussian filters
 * -# the 9 convolutions of the image by the gaussian filters (called responses)
 * 
 * and then calls toneMap().
 * 
 * For improved efficiency, the convolutions are computed in the Fourier domain. 
 * 
 */
void ReinhardLocalOperator::setImage(const HdrImage* _inputImage)
{
    if (!_inputImage->hasY())
        throw Exception(tr("Image passed to %1 does not contains Y data. Cannot turn water into wine.").arg(name()));
    
    QTime t;
    t.start();
    
	{
		PROFILE_FUNC();

		inputImage = _inputImage;
		QSize size = inputImage->size();
		width = size.width();
		height = size.height();
		int Y = inputImage->YIndex();
		int length =  width*height;
		double delta = 0.0001;
		fftw_plan plan;
		fftw_complex* image = NULL;
		fftw_complex* filters[9] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    
		delete outputImage;
		outputImage = new HdrImage(*inputImage);
    
		// calculate average luminance
		avLum = 0.0;
		for(int i=0; i<height; ++i)
			for(int j=0; j<width; ++j)
			{
				double lum = qMax(double((*inputImage)[i][j][Y]),0.0);
				avLum += log(delta + lum);
			}
		avLum = exp(avLum/(double)(length));
    
		// delete data from potential previous image
		for(int k=0; k<9; ++k)
		{
			if (responses[k])
				fftw_free(responses[k]);
		}
    
		// allocate memory for image fft and filters ffts
		image = new fftw_complex[length];
		for(int k=0; k<9; ++k)
		{
			filters[k] = new fftw_complex[length];
			for(int i=0; i<length; ++i)
			{
				filters[k][i][0] = 0.0; // re
				filters[k][i][1] = 0.0; // im
			}
		}

		// generate filters data
		for(int k=0; k<9; ++k)
		{
			double s = pow(1.6, k);
			int rs = int(s+0.5);
			int maxY = qMin(height/2,rs);
			int minY = -maxY;
			int maxX = qMin(width/2,rs);
			int minX = -maxX;
			double c = (1./8.) * s * s;
			double a = 1./(M_PI * c);
			double sum = 0.0;
			double gauss = 0.0;

			for(int y=minY; y<maxY; ++y)
				for(int x=minX; x<maxX; ++x)
				{
					gauss = a * exp(-(x*x + y*y)/c);
					filters[k][(y + height/2)*width + x + width/2][0] = gauss;
					sum += gauss;
				}
        
			// normalization
			for(int y=minY; y<maxY; ++y)
				for(int x=minX; x<maxX; ++x)
				{
					filters[k][(y + height/2)*width + x + width/2][0] /= sum;
				}
		}
    
		// compute filters ffts
		for(int k=0; k<9; ++k)
		{
			plan = fftw_plan_dft_2d(height, width, filters[k], filters[k], FFTW_FORWARD, FFTW_ESTIMATE);
			fftw_execute(plan);
        
			double scale = 1./sqrt((float)length);
			for(int i=0; i<length; ++i)
			{
				filters[k][i][0] *= scale;
				filters[k][i][1] *= scale;
			}
		}
    
		// copy image data
		for(int i=0; i<height; ++i)
		{
			for(int j=0; j<width; ++j)
			{
				image[i*width + j][0] = (*inputImage)[i][j][Y];
				image[i*width + j][1] = 0.0;
			}
		}
        
		// compute image fft
		plan = fftw_plan_dft_2d(height, width, image, image, FFTW_FORWARD, FFTW_ESTIMATE);
		fftw_execute(plan);
		double scale = 1./sqrt((float)length);
		for(int i=0; i<length; ++i)
		{
			image[i][0] *= scale;
			image[i][1] *= scale;
		}
    
		// convolve image and filters
		for(int k=0; k<9; ++k)
		{
			responses[k] = convolveFft(image, filters[k]);
		}
    
		// compute responses inverse ffts
		for(int k=0; k<9; ++k)
		{
			plan = fftw_plan_dft_2d(height, width, responses[k], responses[k], FFTW_BACKWARD, FFTW_ESTIMATE);
			fftw_execute(plan);
        
			for(int i=0; i<height/2; ++i)
			{
				for(int j=0; j<width/2; ++j)
				{
					double tmp = responses[k][i*width + j][0];
					responses[k][i*width + j][0] = responses[k][(i+height/2)*width + j+width/2][0];
					responses[k][(i+height/2)*width + j+width/2][0] = tmp;
                
					tmp = responses[k][(i+height/2)*width + j][0];
					responses[k][(i+height/2)*width + j][0] = responses[k][i*width + j+width/2][0];
					responses[k][i*width + j+width/2][0] = tmp;
				}
			}
		}
    
		fftw_destroy_plan(plan);
		delete[] image;
		for(int k=0; k<9; ++k)
			delete[] filters[k];
    
		ui.keyValueSlider->setValue(18); // = 0.18
		ui.sharpeningSlider->setValue(8); 
	}
    
    msg = tr("Operator Init: %1 ms").arg(t.elapsed());
    
    toneMap();
}

/**
 * Computes the convolution of \a f1 by \a f2 in the Fourier domain.
 * A convolution in the Fourier domain is actually a complex multiplication.
 */
fftw_complex* ReinhardLocalOperator::convolveFft(fftw_complex* f1, fftw_complex* f2)
{
    int length = width*height;
    fftw_complex* res = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex)*length));
    for (int i=0; i<length; ++i)
    {
        res[i][0] = f1[i][0] * f2[i][0] - f1[i][1] * f2[i][1];
        res[i][1] = f1[i][0] * f2[i][1] + f1[i][1] * f2[i][0];
    }
    return res;
}

/**
 * Tone maps the image.
 * 
 * This function emit imageUpdated() when tone mapping is finished.
 */
void ReinhardLocalOperator::toneMap()
{
    if(inputImage)
    {
        QTime t;
        t.start();
        
        int Y = inputImage->YIndex();
        const float threshold = 0.05;
        
        int k;
        double s, v, v1, v2;
        double lumMap = keyValue/avLum;
        double parameters = pow(2.0,sharpening) * keyValue;
        for(int i=0; i<height; ++i)
        {
            for(int j=0; j<width; ++j)
            {
                int index = i*width+j;
                for(k=0; k<8; ++k)
                {
                    v1 = responses[k][index][0] * lumMap;
                    v2 = responses[k+1][index][0] * lumMap;
                    s = pow(1.6, k);
                    v = (v1 - v2) / ((parameters / (s*s)) + v1);
                    if (qAbs(v) > threshold)
                    {
                        break;
                    }
                }
                (*outputImage)[i][j][Y] = ((*inputImage)[i][j][Y] * lumMap) / (1.0 + v1);
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
void ReinhardLocalOperator::updateKeyValue(int value)
{
    keyValue = double(value)/100.0;
    ui.keyValue->setText(QString("%1").arg(keyValue, 0, 'f', 2));
}

/**
 * Reads the sharpening parameter from the user interface.
 * 
 * This function is typically called when the sharpening parameter slider is moved.
 */
void ReinhardLocalOperator::updateSharpening(int value)
{
    sharpening = double(value);
    ui.sharpening->setText(QString("%1").arg(sharpening));
}


//----------------------------------------------------------------------

/**
 * \class ReinhardLocalOperatorFactory
 * \brief The ReinhardLocalOperatorFactory class creates instances of ReinhardLocalOperator.
 * 
 * This class is an implementation of ToneMappingOperatorFactory.
 */

/**
 * Returns a newly created ReinhardLocalOperator.
 */
ToneMappingOperatorPtr ReinhardLocalOperatorFactory::createOperator() const
{
    return ToneMappingOperatorPtr(new ReinhardLocalOperator);
}

/**
 * Returns the operator name: Reinhard Local Operator.
 */
QString ReinhardLocalOperatorFactory::operatorName() const
{
    return tr(OPERATOR_NAME);
}
