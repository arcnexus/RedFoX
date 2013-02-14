/*
 * OpenRPT report writer and rendering engine
 * Copyright (C) 2001-2011 by OpenMFG, LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact info@openmfg.com with any questions on this license.
 */

#ifndef parameter_h
#define parameter_h

#include <QList>
#include <QPair>
#include <QString>
#include <QVariant>

class ParameterList;

class Parameter
{
  friend class ParameterList;

  public:
    Parameter();
    Parameter(const QString &, const QVariant &);
    Parameter(const Parameter &);

    Parameter & operator= (const Parameter &);

inline
    QString  name() const  { return _name;   };
inline
    QVariant value() const { return _value;  };

  protected:
    QString  _name;
    QVariant _value;
};

class ParameterList : public QList<Parameter>
{
  public:
    void append(const QString &);
    void append(const QString &, const QVariant &);
    void append(const QString &, const QString &);
    void append(const QString &, const char *);
    void append(const QString &, const QDate &);
    void append(const QString &, unsigned int);
    void append(const QString &, int);
    void append(const QString &, double);
    void append(const Parameter &);

	void remove( QString &);

    QString  name(int) const;
    QVariant value(int, bool * = NULL) const;
    QVariant value(const QString &, bool * = NULL) const;
    bool     inList(const QString &) const;
};
  
#endif

