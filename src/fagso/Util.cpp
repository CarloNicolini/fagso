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


#include <iostream>
#include <map>
#include <vector>
#include <sys/time.h>
#include "FagsoParams.h"
#include "Util.h"

using std::string;
using std::stringstream;
using std::cerr;
using std::endl;
using std::ifstream;
using std::map;
using std::vector;

/**
 * @brief randomizeStart
 */
void randomizeStart()
{
    struct timeval time;
    gettimeofday(&time,NULL);
    std::srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/**
 * @brief has_suffix
 * @param str
 * @param suffix
 * @return
 */
bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}


/**
 * @brief skipSpaces
 * @param str
 */
void skipSpaces(std::stringstream &str)
{
    char c;
    while (((c = str.get()) == ' ') || c == '\t' || c == '\r' || c == '\n');
    str.unget();
}

/**
 * @brief readChar
 * @param str
 * @param c
 * @return
 */
int readChar(stringstream &str, int c)
{
    int cc;
    if ((cc = str.get()) != c)
    {
        cerr << "expected \"" << (char) c << "\" but found \"" << (char) cc << "\" ";
        return 1;
    }
    return 0;
}

/**
 * @brief skipSpacesReadChar
 * @param str
 * @param c
 * @return
 */
int skipSpacesReadChar(stringstream &str, int c)
{
    skipSpaces(str);
    int ret = readChar(str, c);
    skipSpaces(str);
    return ret;
}

/**
 * @brief nextChar
 * @param str
 * @return
 */
int nextChar(stringstream &str)
{
    int c = str.get();
    str.unget();
    return c;
}



/**
 * @brief str2int
 * @param x
 * @return
 */
int str2int(const std::string &x)
{
    int result;          //number which will contain the result
    std::istringstream convert(x); // stringstream used for the conversion constructed with the contents of 'Text'

    if ( !(convert >> result) ) //give the value to 'Result' using the characters in the stream
        result = 0;             //if that fails set 'Result' to 0
    return result;
}

/**
 * @brief str2int
 * @param x
 * @return
 */
double str2double(const std::string &x)
{
    double result;          //number which will contain the result
    std::istringstream convert(x); // stringstream used for the conversion constructed with the contents of 'Text'

    if ( !(convert >> result) ) //give the value to 'Result' using the characters in the stream
        result = 0;             //if that fails set 'Result' to 0
    return result;
}

/**
 * @brief int2str
 * @param x
 * @return
 */
string int2str(int x)
{
    string res;          // string which will contain the result
    std::ostringstream convert;   // stream used for the conversion
    convert << x;      // insert the textual representation of 'Number' in the characters in the stream
    return convert.str(); // set 'Result' to the contents of the stream
}

/**
 * @brief double2str
 * @param x
 * @return
 */
string double2str(double x)
{
    string res;          // string which will contain the result
    std::ostringstream convert;   // stream used for the conversion
    convert << x;      // insert the textual representation of 'Number' in the characters in the stream
    return convert.str(); // set 'Result' to the contents of the stream
}

/**
 * @brief savestring
 * @param params
 * @param S
 * @return
 */
std::string savestring(Parameters params, double S)
{
    std::string outputfilename;
    outputfilename += params.inputGraphFileName + "_S_" + double2str(S);
    //+ "_m_" + int2str(static_cast<int>(params.sortmethod));
    return outputfilename;
}
