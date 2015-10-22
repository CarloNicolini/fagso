/* This file is part of FAGSO, a program to find network partitions
*
*  Copyright (C) 2014-2015 Carlo Nicolini <carlo.nicolini@iit.it>
*
*  FAGSO is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 3 of the License, or (at your option) any later version.
*
*  Alternatively, you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of
*  the License, or (at your option) any later version.
*
*  FAGSO is distributed in the hope that it will be useful, but WITHOUT ANY
*  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*  FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License and a copy of the GNU General Public License along with
*  FAGSO. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTIL_H
#define UTIL_H
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <Eigen/Core>
#include "FagsoParams.h"
#include "DisjointSet.h"


using std::ostream;

/**
 * @brief randomizeStart
 */
void randomizeStart();

/**
 * @brief has_suffix
 * @param str
 * @param suffix
 * @return
 */
bool has_suffix(const std::string &str, const std::string &suffix);


/**
 * @brief str2int
 * @param x
 * @return
 */
int str2int(const std::string &x);

/**
 * @brief str2int
 * @param x
 * @return
 */
double str2double(const std::string &x);

/**
 * @brief int2str
 * @param x
 * @return
 */
std::string int2str(int x);

/**
 * @brief double2str
 * @param x
 * @return
 */
std::string double2str(double x);


/**
 * @brief skipSpaces
 * @param str
 */
void skipSpaces(std::stringstream &str);

/**
 * @brief readChar
 * @param str
 * @param c
 * @return
 */
int readChar(std::stringstream &str, int c);

/**
 * @brief skipSpacesReadChar
 * @param str
 * @param c
 * @return
 */
int skipSpacesReadChar(std::stringstream &str, int c);

/**
 * @brief nextChar
 * @param str
 * @return
 */
int nextChar(std::stringstream &str);


/**
 * @brief savestring
 * @param params
 * @param S
 * @return
 */
std::string savestring(Parameters params, double S);

#endif // UTIL_H
