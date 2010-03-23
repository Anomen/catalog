/*
 * This file is part of catalog-server.
 * Copyright (C) 2008-2010  Kevin Vicrey <kevin.vicrey@gmail.com>
 * Copyright (C) 2008-2009  Romain Giraud <giraud.romain@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined __CEXCEPTION_H__
#define      __CEXCEPTION_H__

#include <string>

namespace nsCatalog
{
    class CException
    {
      public:
        CException (std::string Str = "") throw();
        CException (int Num)              throw();
        ~CException (void)                throw();

        const std::string & GetStr (void) const throw();

      private:
        std::string m_Str;

    };

    class CExceptCar : public CException { };
}

#include "CException.hxx"

#endif    /*  __CEXCEPTION_H__ */
