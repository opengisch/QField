#ifndef QFIELDCORE_GLOBAL_H
#define QFIELDCORE_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(QFIELDCORE_SHARED_LIBRARY)
#   define QFIELDCORE_EXPORT Q_DECL_EXPORT
#else
#   define QFIELDCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // QFIELDCORE_GLOBAL_H
