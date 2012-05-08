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
    inputImage(NULL),
    outputImage(NULL),
	pixmapBuffer(NULL),
    gamma(1.0),
    gammaEnabled(true),
    operatorEnabled(true),
    loaderManager(*ImageLoaderManager::instance()),
    operatorManager(*ToneMappingOperatorManager::instance())
{
    xyzToRgbMatrix[0][0]= 2.5651; xyzToRgbMatrix[0][1]=-1.1665; xyzToRgbMatrix[0][2]=-0.3986;
	xyzToRgbMatrix[1][0]=-1.0217; xyzToRgbMatrix[1][1]= 1.9777; xyzToRgbMatrix[1][2]= 0.0439;
	xyzToRgbMatrix[2][0]= 0.0753; xyzToRgbMatrix[2][1]=-0.2543; xyzToRgbMatrix[2][2]= 1.1892;
    
    setupUi(this);
    
    scrollArea = new ImageScrollArea;
    displayAreaLayout->addWidget(scrollArea);
    
    zoomLabel = new QLabel;
    statusbar->addPermanentWidget(zoomLabel, 1);
    
    operatorLabel = new QLabel;
    statusbar->addPermanentWidget(operatorLabel, 2);
    
    conversionTimeLabel = new QLabel;
    statusbar->addPermanentWidget(conversionTimeLabel, 1);
    
    displayTimeLabel = new QLabel;
    statusbar->addPermanentWidget(displayTimeLabel, 1);

    
    connect(operatorComboBox, SIGNAL(currentIndexChanged(const QString&)), 
            this, SLOT(updateOperator(const QString&)));
    connect(scrollArea, SIGNAL(scaleChanged(double)), this, SLOT(updateZoom(double)));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actionScreenColors, SIGNAL(triggered()), this, SLOT(openScreenColorsDialog()));
    connect(operatorGroupBox, SIGNAL(toggled(bool)), this, SLOT(enableOperator(bool)));
    connect(gammaGroupBox, SIGNAL(toggled(bool)), this, SLOT(enableGamma(bool)));
    connect(gammaSlider, SIGNAL(sliderReleased()), this, SLOT(displayImage()));
    connect(gammaSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGamma(int)));

    loaderManager.registerLoaders("Loaders");
    
    QStringList operatorList = operatorManager.registerOperators("Operators");
    operatorComboBox->addItems(operatorList);
    
    
    if (loaderManager.empty())
    {
        qFatal("No image loader found! TonicTones will not be able to open images! Exiting now.");
    }
    if (operatorManager.empty())
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
    
    try
    {
        if (!fileName.isEmpty())
        {
            HdrImage* newImage = new HdrImage(fileName); // may throw an exception
            delete inputImage;
            inputImage = newImage;

			delete pixmapBuffer;
			pixmapBuffer = new int[inputImage->size().width() * inputImage->size().height()];
            
            if(operatorManager.getActiveOperator())
            {
                operatorManager.getActiveOperator()->setImage(inputImage);
            }
            else
            {
                operatorEnabled = false;
                updateImage();
            }
            
            setWindowTitle(QString("TonicTones - %1").arg(QDir(fileName).dirName()));
                
            scrollArea->scaleImage(1.0, false); // set zoom 100%
        }
    }
    catch(const Exception& e)
    {
        qWarning() << e.what();
    }
}

/**
 * Gets the image outputted by the current operator and transforms it into RGB space if needed.
 * 
 * This function also calls displayImage().
 */
void TonicTones::updateImage()
{
    const HdrImage* image;
    if(operatorEnabled)
        image = operatorManager.getActiveOperator()->getToneMappedImage();
    else
        image = inputImage;
    if (image)
    {
        QTime t;
        t.start();
        
        delete outputImage;
        try
        {    
            outputImage = image->toRgb(xyzToRgbMatrix);
        }
        catch(const Exception& e)
        {
            qWarning() << e.what();
        }
        conversionTimeLabel->setText(tr("Transformation to RGB: %1 ms").arg(t.elapsed()));
        displayImage();
    }
}

/**
 * Displays the current image.
 */
void TonicTones::displayImage() const
{
    if (outputImage)
    {
        QTime t;
        t.start();
        
        QSize size = outputImage->size();
        int width = size.width();
        int height = size.height();
        
        if(gammaEnabled && gamma!=1.0)
        {
            double gamma_corr = 1.0/gamma;
            for(int i=0; i<height; ++i)
                for(int j=0; j<width; ++j)
                {
                    Color p = (*outputImage)[i][j].clamp();
                    pixmapBuffer[i*width+j] = qRgb(pow((double)p[0],gamma_corr)*255.0,pow((double)p[1],gamma_corr)*255.0,pow((double)p[2],gamma_corr)*255.0);
                }
        }
        else
        {
            for(int i=0; i<height; ++i)
                for(int j=0; j<width; ++j)
                {
                    Color p = (*outputImage)[i][j].clamp();
                    pixmapBuffer[i*width+j] = qRgb(p[0]*255.0,p[1]*255.0,p[2]*255.0);
                }
        }
        QImage ldrImage((uchar*)pixmapBuffer, width, height, QImage::Format_RGB32);

        scrollArea->image()->setPixmap(QPixmap::fromImage(ldrImage));
        displayTimeLabel->setText(tr("Display: %1 ms").arg(t.elapsed()));
    }
}

/**
 * Updates the text showing the current zoom.
 */
void TonicTones::updateZoom(double scaleFactor)
{
    zoomLabel->setText(tr("Zoom: %1 %").arg(scaleFactor*100, 0, 'f', 0));
}

/**
 * Changes the current operator to \a operatorName.
 */
void TonicTones::updateOperator(const QString& operatorName)
{
    qDebug("%s selected.", operatorName.toStdString().c_str());

    // set new operator
    operatorManager.setActiveOperator(operatorName);
    
    // remove previous operator ui
    QLayoutItem* item = operatorOptionsGroupBox->layout()->itemAt(0);
    operatorOptionsGroupBox->layout()->removeItem(item);
    if(item)
        item->widget()->close();
    
    // add new operator ui
    QWidget *wrapper = new QWidget;
    wrapper->setAttribute(Qt::WA_DeleteOnClose);
    operatorOptionsGroupBox->layout()->addWidget(wrapper);
    operatorManager.getActiveOperator()->setupUi(wrapper);

    connect(operatorManager.getActiveOperator().data(),SIGNAL(imageUpdated()),
            this, SLOT(updateImage()));
    connect(operatorManager.getActiveOperator().data(),SIGNAL(message(const QString&)),
            operatorLabel, SLOT(setText(const QString&)));
    // if an image is already loaded, send it to the new operator
    if (inputImage)
    {
        operatorManager.getActiveOperator()->setImage(inputImage);
    }
}

/**
 * Updates the text showing the current gamma correction.
 */
void TonicTones::updateGamma(int value)
{
    gamma = float(value)/100.0;
    gammaValue->setText(QString("%1").arg(gamma, 0, 'f', 2));
}

/**
 * Enables or disables the current operator and its options.
 * 
 * This function is typically called when the operator check box is clicked. It also
 * calls updateImage().
 */
void TonicTones::enableOperator(bool enabled)
{
    operatorEnabled = enabled;
    operatorOptionsGroupBox->setEnabled(enabled);
    updateImage();
}

/**
 * Enable or disables gamma correction.
 * 
 * This function is typically called when the gamma check box is clicked. It also
 * calls displayImage().
 */
void TonicTones::enableGamma(bool enabled)
{
    gammaEnabled = enabled;
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
            m(i,j) = xyzToRgbMatrix[i][j];
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
            xyzToRgbMatrix[i][j] = m(i,j);
            
    updateImage();
}

