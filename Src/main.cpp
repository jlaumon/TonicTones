//      main.cpp
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


#include <QApplication>
#include <QtGui>
#include <TonicTones.h>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

void messageHandler(QtMsgType type, const char *msg)
{
    QMessageBox msgBox;
    switch (type) 
    {
        case QtWarningMsg:
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(msg);
            msgBox.exec();
            break;
        case QtCriticalMsg:
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(msg);
            msgBox.exec();
            break;
        case QtFatalMsg:
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(msg);
            msgBox.exec();
            abort();
            break;
        default:
#ifdef Q_OS_WIN
			OutputDebugStringA(msg);
			OutputDebugStringA("\n");
#else
            printf("%s\n", msg);
#endif
            break;
    }
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    qInstallMsgHandler(messageHandler);
    TonicTones t;
    t.show();

    return app.exec();
}

