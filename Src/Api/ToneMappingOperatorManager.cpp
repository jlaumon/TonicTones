//      ToneMappingOperatorManager.cpp
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


#include <ToneMappingOperatorManager.h>

/**
 * \class ToneMappingOperatorManager
 * \brief The ToneMappingOperatorManager class loads ToneMappingOperator plugins and keeps an instance of the active ToneMappingOperator.
 * 
 * This class is a Singleton.
 */

/**
 * Returns a pointer to the current ToneMappingOperator.
 */
ToneMappingOperatorPtr ToneMappingOperatorManager::getActiveOperator()
{
    return activeOperator;
}

/**
 * Sets the active ToneMappingOperator to \a operatorName.
 */
void ToneMappingOperatorManager::setActiveOperator(const QString& operatorName)
{
    if(!operators.contains(operatorName))
    {
        activeOperator = ToneMappingOperatorPtr(NULL);
    }
    else
    {
        activeOperator = operators[operatorName]->createOperator();
    }
}

/**
 * Registers all the ToneMappingOperator plugins found in the directory with the name \a directory.
 */
QStringList ToneMappingOperatorManager::registerOperators(const QString &directory)
{
    QDir operatorsDir(qApp->applicationDirPath());
    operatorsDir.cd(directory);
    
    QStringList operatorList;
    
    if (operatorsDir.exists())
    {    
        foreach (QString fileName, operatorsDir.entryList(QDir::Files)) 
        {
            qDebug("Opening %s...", fileName.toStdString().c_str());
            QObject *plugin = QPluginLoader(operatorsDir.absoluteFilePath(fileName)).instance();
            ToneMappingOperatorFactory *operatorFactory = qobject_cast<ToneMappingOperatorFactory *>(plugin);
            if (operatorFactory)
            {
                operators.insert(operatorFactory->operatorName(), operatorFactory);
                operatorList << operatorFactory->operatorName();
                qDebug("\tLoaded: %s.", operatorFactory->operatorName().toStdString().c_str());
            }
        }
    }
    
    return operatorList;
}

/**
 * Returns true if the ToneMappingOperatorManager has no operator, otherwise returns false.
 */
bool ToneMappingOperatorManager::empty()
{
    return operators.empty();
}
