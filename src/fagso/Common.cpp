/* This file is part of FAGSO, a program to find network partitions
*
*  Copyright (C) 2014-2015 Carlo Nicolini <carlo.nicolini@iit.it>
*
*  Surprise++ is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 3 of the License, or (at your option) any later version.
*
*  Alternatively, you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of
*  the License, or (at your option) any later version.
*
*  Surprise++ is distributed in the hope that it will be useful, but WITHOUT ANY
*  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*  FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License and a copy of the GNU General Public License along with
*  Surprise++. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Common.h"

namespace Common
{

/**
Get the next string with a given delimiter from an open stringstream object
@param str stringstream to iterate on
@param delimiters a char which specifies the delimiter to tokenize the string stream, typicall space or tab
@return The tokenized string, the stringstream is also advanced
**/
string nextToken(stringstream &str, const char *delimiters)
{
    string ret("");
    unsigned char c;
    int i;
    while( (c = str.get()) && !str.eof())
    {
        for(i = 0; delimiters[i]; i++)
        {
            if(c == delimiters[i])
                break;
        }
        if(c == delimiters[i])
            break;
        ret.append(1,c);
    }
    if(!str.eof())
        str.unget();
    return ret;
}

/**
@ingroup Common
Skip space on an open stringstream until a non-space or non-tab or non return is found
@param str stringstream to advance on
**/
void skipSpaces(stringstream &str)
{
    char c;
    while (((c = str.get()) == ' ') || c == '\t' || c == '\r' || c == '\n');
    str.unget();
}

/**
@ingroup Common
Read a specific char from a stringstream
@param str stringstream to advance on
@param c char to find
**/
void readChar(stringstream &str, int c)
{
    int cc;
    if ((cc = str.get()) != c)
        cerr << "expected " << (char) c << "but found " << (char) cc << "\n";
}

/**
@ingroup Common
Read a given char ignoring spaces or tabs
@param str stringstream to advance on
@param c char to find
**/
void skipSpacesReadChar(stringstream &str, int c)
{
    skipSpaces(str);
    readChar(str, c);
    skipSpaces(str);
}

/**
@ingroup Common
Get the next char
@param str stringstream to iterate on
**/
int nextChar(stringstream &str)
{
    int c = str.get();
    str.unget();
    return c;
}

/**
@ingroup Common
Trim the string from spaces or tabulation or carriage returns
**/
string trim(string &s)
{
    char drop[] = " \t\r";

    s.erase(s.find_last_not_of(drop)+1);
    s.erase(0,s.find_first_not_of(drop));

    return s;
}

/**
@ingroup Common
Return true if two strings are equal ignoring case
@param s1 string 1 to compare
@param s2 string 2 to compare
@return 1 if the strings are equal, 0 otherwise
**/
int equalCaseInsensitive(string s1, string s2)
{
    if(s1.size() != s2.size())
        return 0;

    int n = s1.size();

    for(int i = 0; i < n; i++)
    {
        if(tolower(s1.at(i)) != tolower(s2.at(i)))
            return 0;
    }

    return 1;
}

/**
  @ingroup Common
  Tokenize a stirng and returns a vector of strings tokenized with a given delimiter
  @param str string to tokenize
  @param tokens return vector of strings
  @param delimiters string containing char delimiters
**/
void tokenizeString(const string& str, vector<string>& tokens, const string& delimiters)
{
    tokens.clear();
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
/**
 *@ingroup Common
 * @brief isnumeric Check if a string is numeric or contains non numeric characters
 * @param p
 * @return
 */
bool isnumeric(const char *p)
{
    if (*p)
    {
        char c;
        while ((c=*p++))
        {
            if (!isdigit(c) || !ispunct(c))
                return false;
        }
        return true;
    }
    return false;
}

/**
@ingroup Common
@return Return a uniform random number in [0,1]
**/
double unifRand()
{
    return mtrand1.rand();
}

/**
@ingroup Common
@return Return a uniform random number in interval [a,b]
@param a minimum limit
@param b maximum limit
@return a number with random uniform distrbution in [a,b]
**/
double unifRand(double a, double b)
{
    //return (b-a)*unifRand() + a;
    return (b-a)*mtrand1.rand() +a ;
}

/**
@ingroup Common
@return Return a uniform random number in open interval [0,end)
@param end maximum limit excluded
**/
double unifRandExc(double end)
{
    return mtrand1.randExc(end);
}

/**
@ingroup Common
@return Return a uniform random integer number in [a,b]
@param a  lower limit
@param b higher limit
**/
int unifRand(int a, int b)
{
    //return (b-a)*unifRand() + a;
    return (b-a)*mtrand1.randInt() + a;
}

/**
@ingroup Common
@return Return a uniform random long integer number in [0,n]
@param n higher limit
**/
long unifRand(long n)
{

    if (n < 0) n = -n;
    if (n==0) return 0;

    long num = mtrand1.randInt( n );
    return num;
}

int unifPosOrNeg()
{
    int res=0;
    (unifRand(1)== 0) ? res=1 : res= -1;
    return res;
}


int iReadChar(stringstream &str, int c)
{
    int cc;
    if ((cc = str.get()) != c)
    {
        cerr << "expected \"" << (char) c << "\" but found \"" << (char) cc << "\" ";
        return 1;
    }
    return 0;
}


int iSkipSpacesReadChar(stringstream &str, int c)
{
    skipSpaces(str);
    int ret=iReadChar(str, c);
    skipSpaces(str);
    return ret;
}


int iNextChar(stringstream &str)
{
    int c = str.get();
    str.unget();
    return c;
}

}
