// -----------------------------------------------------------------
// TestTrs  - a protocol test util
// Copyright (C) 2019  Gabriele Bonacini
//
// This program is free software for no profit use; you can redistribute
// it and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
// A commercial license is also available for a lucrative use.
// ----------------------------------------------------------------

#ifndef LIBUNITTESTS_GLOBAL_H
#define LIBUNITTESTS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBUNITTESTS_LIBRARY)
#  define LIBUNITTESTSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBUNITTESTSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBUNITTESTS_GLOBAL_H
