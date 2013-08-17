#ifndef TONICTONES_EXPORTS_H
#define TONICTONES_EXPORTS_H

#include <QtCore/QtGlobal>

#ifdef TONICTONES_EXPORT
#define TT_API Q_DECL_EXPORT
#else
#define TT_API Q_DECL_IMPORT
#endif

#endif