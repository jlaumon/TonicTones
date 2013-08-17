//      TonicTones.cpp
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


#include <QtGui>
#include <TonicTones.h>
#include <Exceptions.h>
#include <iostream>
#include <etwprof.h>
#include <Profiler.h>

/**
 * \mainpage
 * 
 * \section intro Introduction:
 * \par
 * This is the TonicTones documentation. TonicTones is a tone mapping
 * application developped in C++ using extensively Qt.
 * 
 * \section structure Structure:
 * \par 
 * The application is composed of a core and several plugins. There are two
 * types of plugins, image loaders and tone mapping operators.
 * 
 * \subsection loaders Image Loaders:
 * \par
 * Image loaders are used to read image files. Each image loader can handle
 * one or more image formats. When loading an image, an image loader manager
 * choses a specific loader according to the file extension.
 * \par
 * There are currently two image loaders:
 * \li <b>LdrFormatsLoader:</b> This loader can load all usual images formats (png, jpeg, bmp, etc.).
 * \li <b>OpenExrLoader:</b> This loader can load OpenEXR images (exr). This format can store HDR data. 
 * Format description here: http://www.openexr.com/.
 * 
 * \subsection operators Tone Mapping Operators:
 * \par
 * Tone mapping operators do the actual job of tone mapping. Each 
 * tone mapping operator can have its own user interface which is
 * integrated to the main user interface when the operator is selected. A
 * tone mapping operator manager keeps a list of available operators
 * and an instance of the active operator.
 * 
 * \par
 * There are currently two operators:
 * \li <b>ReinhardGlobalOperator</b>
 * \li <b>ReinhardLocalOperator</b>
 * 
 * These two operators are based on Erik Reinhard's work : http://www.cs.utah.edu/~reinhard/cdrom/.
 * 
 * \section licence Licence:
 * \par
 * This software and its documentation is released under the terms of the 
 * GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * \author Jérémy Laumon
 * 
 * \par
 * \image html screenshot.png
 * 
 * 
 * 
 */

/**
 * \class TonicTones
 * \brief The TonicTones class is the main widget of this application.
 * 
 * 
 * 
 */

/**
 * Constructs a TonicTones widget with a given \a parent.
 */
TonicTones::TonicTones(QWidget *parent) : 
    QMainWindow(parent),
    m_inputImage(NULL),
    m_outputImage(NULL),
	m_pixmapBuffer(NULL),
    m_gamma(1.0),
    m_gammaEnabled(true),
    m_operatorEnabled(true),
    m_loaderManager(*ImageLoaderManager::instance()),
    m_operatorManager(*ToneMappingOperatorManager::instance())
{
    m_xyzToRgbMatrix[0][0]= 2.5651f; m_xyzToRgbMatrix[0][1]=-1.1665f; m_xyzToRgbMatrix[0][2]=-0.3986f;
	m_xyzToRgbMatrix[1][0]=-1.0217f; m_xyzToRgbMatrix[1][1]= 1.9777f; m_xyzToRgbMatrix[1][2]= 0.0439f;
	m_xyzToRgbMatrix[2][0]= 0.0753f; m_xyzToRgbMatrix[2][1]=-0.2543f; m_xyzToRgbMatrix[2][2]= 1.1892f;
    
    setupUi(this);
    
    m_scrollArea = new ImageScrollArea;
    displayAreaLayout->addWidget(m_scrollArea);
    
    m_zoomLabel = new QLabel;
    statusbar->addPermanentWidget(m_zoomLabel, 1);
    
    m_operatorLabel = new QLabel;
    statusbar->addPermanentWidget(m_operatorLabel, 2);
    
    m_conversionTimeLabel = new QLabel;
    statusbar->addPermanentWidget(m_conversionTimeLabel, 1);
    
    m_displayTimeLabel = new QLabel;
    statusbar->addPermanentWidget(m_displayTimeLabel, 1);

    
    connect(operatorComboBox, SIGNAL(currentIndexChanged(const QString&)), 
            this, SLOT(updateOperator(const QString&)));
    connect(m_scrollArea, SIGNAL(scaleChanged(double)), this, SLOT(updateZoom(double)));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actionScreenColors, SIGNAL(triggered()), this, SLOT(openScreenColorsDialog()));
    connect(operatorGroupBox, SIGNAL(toggled(bool)), this, SLOT(enableOperator(bool)));
    connect(gammaGroupBox, SIGNAL(toggled(bool)), this, SLOT(enableGamma(bool)));
    connect(gammaSlider, SIGNAL(sliderReleased()), this, SLOT(displayImage()));
    connect(gammaSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGamma(int)));

    m_loaderManager.registerLoaders("Loaders");
    
    QStringList operatorList = m_operatorManager.registerOperators("Operators");
    operatorComboBox->addItems(operatorList);
    
    
    if (m_loaderManager.empty())
    {
        qFatal("No image loader found! TonicTones will not be able to open images! Exiting now.");
    }
    if (m_operatorManager.empty())
    {
        qWarning("No tone mapping operator found. Satisfaction not guaranteed.");
        operatorGroupBox->hide();
        operatorOptionsGroupBox->hide();
    }
}

/**
 * Opens a dialog for selecting a file, opens the selected file and sends it to the current operator.
 */
void TonicTones::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);

	open(fileName);
}

#ifdef PROFILING_ENABLED
#define PROFILING_LOOP_COUNT 20
#endif

#define OPTIM_VERSION "_V00_"

void TonicTones::open(const QString& fileName)
{
    try
    {
#if PROFILING_LOOP_COUNT > 0
		for (int profLoop=0; profLoop < PROFILING_LOOP_COUNT; ++profLoop)
#endif
		{
			PROFILE_FUNC();
			if (!fileName.isEmpty())
			{
				HdrImage* newImage = new HdrImage(fileName); // may throw an exception
				delete m_inputImage;
				m_inputImage = newImage;

				delete m_pixmapBuffer;
				m_pixmapBuffer = new int[m_inputImage->size().width() * m_inputImage->size().height()];
            
				if(m_operatorManager.getActiveOperator())
				{
					m_operatorManager.getActiveOperator()->setImage(m_inputImage);
				}
				else
				{
					m_operatorEnabled = false;
					updateImage();
				}
            
				setWindowTitle(QString("TonicTones - %1").arg(QDir(fileName).dirName()));
                
				m_scrollArea->scaleImage(1.0, false); // set zoom 100%
			}
		}
    }
    catch(const Exception& e)
    {
        qWarning() << e.what();
    }

#ifdef PROFILING_ENABLED
	Profiler::instance()->display();
	QString baseName = QFileInfo(fileName).completeBaseName();
	QString currDate = QDateTime::currentDateTime().toString("yyMMddhhmmss");
	Profiler::instance()->writeCSV(QString("Prof/" + baseName + OPTIM_VERSION + currDate + ".csv").toStdString().c_str());
#endif
}

/**
 * Gets the image outputted by the current operator and transforms it into RGB space if needed.
 * 
 * This function also calls displayImage().
 */
void TonicTones::updateImage()
{
    const HdrImage* image;
    if(m_operatorEnabled)
        image = m_operatorManager.getActiveOperator()->getToneMappedImage();
    else
        image = m_inputImage;
    if (image)
    {
        QTime t;
        t.start();
        
        delete m_outputImage;
        try
        {    
            m_outputImage = image->toRgb(m_xyzToRgbMatrix);
        }
        catch(const Exception& e)
        {
            qWarning() << e.what();
        }
        m_conversionTimeLabel->setText(tr("Transformation to RGB: %1 ms").arg(t.elapsed()));
        displayImage();
    }
}

/**
 * Displays the current image.
 */
void TonicTones::displayImage() const
{
    if (m_outputImage)
    {
        QTime t;
        t.start();
        
        QSize size = m_outputImage->size();
        int width = size.width();
        int height = size.height();
        
        if(m_gammaEnabled && m_gamma!=1.0)
        {
			PROFILE("TonicTones::displayImage - Gamma correction");

            double gamma_corr = 1.0/m_gamma;
            for(int i=0; i<height; ++i)
                for(int j=0; j<width; ++j)
                {
                    Color p = (*m_outputImage)[i][j].clamp();
                    m_pixmapBuffer[i*width+j] = qRgb(pow((double)p[0],gamma_corr)*255.0,pow((double)p[1],gamma_corr)*255.0,pow((double)p[2],gamma_corr)*255.0);
                }
        }
        else
        {
			PROFILE_FUNC();

            for(int i=0; i<height; ++i)
                for(int j=0; j<width; ++j)
                {
                    Color p = (*m_outputImage)[i][j].clamp();
                    m_pixmapBuffer[i*width+j] = qRgb(p[0]*255.0,p[1]*255.0,p[2]*255.0);
                }
        }
        QImage ldrImage((uchar*)m_pixmapBuffer, width, height, QImage::Format_RGB32);

        m_scrollArea->image()->setPixmap(QPixmap::fromImage(ldrImage));
        m_displayTimeLabel->setText(tr("Display: %1 ms").arg(t.elapsed()));
    }
}

/**
 * Updates the text showing the current zoom.
 */
void TonicTones::updateZoom(double scaleFactor)
{
    m_zoomLabel->setText(tr("Zoom: %1 %").arg(scaleFactor*100, 0, 'f', 0));
}

/**
 * Changes the current operator to \a operatorName.
 */
void TonicTones::updateOperator(const QString& operatorName)
{
    qDebug("%s selected.", operatorName.toStdString().c_str());

    // set new operator
    m_operatorManager.setActiveOperator(operatorName);
    
    // remove previous operator ui
    QLayoutItem* item = operatorOptionsGroupBox->layout()->itemAt(0);
    operatorOptionsGroupBox->layout()->removeItem(item);
    if(item)
        item->widget()->close();
    
    // add new operator ui
    QWidget *wrapper = new QWidget;
    wrapper->setAttribute(Qt::WA_DeleteOnClose);
    operatorOptionsGroupBox->layout()->addWidget(wrapper);
    m_operatorManager.getActiveOperator()->setupUi(wrapper);

    connect(m_operatorManager.getActiveOperator().data(),SIGNAL(imageUpdated()),
            this, SLOT(updateImage()));
    connect(m_operatorManager.getActiveOperator().data(),SIGNAL(message(const QString&)),
            m_operatorLabel, SLOT(setText(const QString&)));
    // if an image is already loaded, send it to the new operator
    if (m_inputImage)
    {
        m_operatorManager.getActiveOperator()->setImage(m_inputImage);
    }
}

/**
 * Updates the text showing the current m_gamma correction.
 */
void TonicTones::updateGamma(int value)
{
    m_gamma = float(value)/100.0;
    gammaValue->setText(QString("%1").arg(m_gamma, 0, 'f', 2));
}

/**
 * Enables or disables the current operator and its options.
 * 
 * This function is typically called when the operator check box is clicked. It also
 * calls updateImage().
 */
void TonicTones::enableOperator(bool enabled)
{
    m_operatorEnabled = enabled;
    operatorOptionsGroupBox->setEnabled(enabled);
    updateImage();
}

/**
 * Enable or disables m_gamma correction.
 * 
 * This function is typically called when the m_gamma check box is clicked. It also
 * calls displayImage().
 */
void TonicTones::enableGamma(bool enabled)
{
    m_gammaEnabled = enabled;
    displayImage();
}


/**
 * Opens a dialog to edit the RGB to XYZ matrix.
 */
void TonicTones::openScreenColorsDialog()
{
    QDialog dialog;
    uiScreenColors.setupUi(&dialog);
    
    // invert XYZ to RGB matrix to get RGB to XYZ
    QMatrix4x4 m;
    for (int i=0; i<3; ++i)
        for(int j=0; j<3; ++j)
            m(i,j) = m_xyzToRgbMatrix[i][j];
    m = m.inverted();
    // set ui spin boxes values
    uiScreenColors.m00->setValue(m(0,0)); uiScreenColors.m01->setValue(m(0,1)); uiScreenColors.m02->setValue(m(0,2));
    uiScreenColors.m10->setValue(m(1,0)); uiScreenColors.m11->setValue(m(1,1)); uiScreenColors.m12->setValue(m(1,2)); 
    uiScreenColors.m20->setValue(m(2,0)); uiScreenColors.m21->setValue(m(2,1)); uiScreenColors.m22->setValue(m(2,2)); 
    
    connect(&dialog, SIGNAL(accepted()), this, SLOT(updateScreenColors()));
    dialog.exec();
}

/**
 * Modifies the RGB to XYZ matrix according to the values entered in the dialog opened by openScreenColorsDialog().
 * 
 * This function also calls updateImage().
 */
void TonicTones::updateScreenColors()
{
    // get ui spin boxes values and
    // invert RGB to XYZ matrix to get XYZ to RGB
    QMatrix4x4 m = QMatrix4x4(uiScreenColors.m00->value(), uiScreenColors.m01->value(), uiScreenColors.m02->value(), 0.0,
                              uiScreenColors.m10->value(), uiScreenColors.m11->value(), uiScreenColors.m12->value(), 0.0,
                              uiScreenColors.m20->value(), uiScreenColors.m21->value(), uiScreenColors.m22->value(), 0.0,
                              0.0,             0.0,             0.0            , 1.0).inverted();
    for (int i=0; i<3; ++i)
        for(int j=0; j<3; ++j)
            m_xyzToRgbMatrix[i][j] = m(i,j);
            
    updateImage();
}

