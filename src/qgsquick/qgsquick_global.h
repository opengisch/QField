#ifndef QGSQUICK_GLOBAL_H
#define QGSQUICK_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(QGSQUICK_SHARED_LIBRARY)
#   define QGSQUICK_EXPORT Q_DECL_EXPORT
#else
#   define QGSQUICK_EXPORT Q_DECL_IMPORT
#endif

#endif // QGSQUICK_GLOBAL_H
