//      ImageLoaderManager.cpp
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

#include <ImageLoaderManager.h>
#include <Exceptions.h>

#define tr(arg) QObject::tr(arg)

/**
 * \class ImageLoaderManager
 * \brief The ImageLoaderManager class loads ImageLoader plugins and creates appropriate ImageLoader instances for each image file format.
 * 
 * Appropriate loaders are chosen according to file name extensions.
 * 
 * This class is a Singleton.
 */

/**
 * Returns a pointer to a loader able to load the image with the name \a fileName.
 */
ImageLoaderPtr ImageLoaderManager::getLoader(const QString &fileName)
{
    int dotPos = fileName.lastIndexOf('.');
    if (dotPos == -1)
        throw Exception(tr("Error while loading %1 : No extension found.").arg(QDir(fileName).dirName()));
    
    QString ext = fileName.right(fileName.size() - (dotPos+1)).toLower();
    
    if(!loaders.contains(ext))
        throw Exception(tr("Error while loading %1 : No suitable loader found.").arg(QDir(fileName).dirName()));
        
    return loaders[ext]->createLoader(fileName);
}

/**
 * Registers all the loaders found in the directory with the name \a directory.
 */
void ImageLoaderManager::registerLoaders(const QString& directory)
{
    QDir loadersDir(qApp->applicationDirPath());
    loadersDir.cd(directory);
    if (loadersDir.exists())
        foreach (QString fileName, loadersDir.entryList(QDir::Files)) 
        {
#ifdef Q_OS_WIN
			if (!fileName.endsWith("dll")) continue;
#endif

            qDebug("Opening %s...", fileName.toStdString().c_str());
			QPluginLoader pluginLoader(loadersDir.absoluteFilePath(fileName));
            QObject *plugin = pluginLoader.instance();
            ImageLoaderFactory *loaderFactory = qobject_cast<ImageLoaderFactory *>(plugin);
            if (loaderFactory)
            {
                foreach(QString ext, loaderFactory->extensions())
                {
                    loaders.insert(ext.toLower(), loaderFactory);
                    qDebug("\tAdding support for .%s", ext.toStdString().c_str());
                }
            }
			else
			{
				qDebug("\t%s", pluginLoader.errorString().toStdString().c_str());
			}
        }
}

/**
 * Returns true if the ImageLoaderManager has no loader, otherwise returns false.
 */
bool ImageLoaderManager::empty()
{
    return loaders.empty();
}


