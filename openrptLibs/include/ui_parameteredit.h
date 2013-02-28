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

/********************************************************************************
** Form generated from reading UI file 'parameteredit.ui'
**
** Created: Wed 20. Feb 18:04:15 2013
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETEREDIT_H
#define UI_PARAMETEREDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParameterEdit
{
public:
    QHBoxLayout *horizontalLayout;
    QTableWidget *_table;
    QVBoxLayout *vboxLayout;
    QPushButton *_ok;
    QPushButton *_cancel;
    QFrame *_buttonDiv;
    QPushButton *_new;
    QPushButton *_edit;
    QPushButton *_list;
    QPushButton *_delete;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *ParameterEdit)
    {
        if (ParameterEdit->objectName().isEmpty())
            ParameterEdit->setObjectName(QString::fromUtf8("ParameterEdit"));
        ParameterEdit->resize(600, 304);
        horizontalLayout = new QHBoxLayout(ParameterEdit);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        _table = new QTableWidget(ParameterEdit);
        if (_table->columnCount() < 4)
            _table->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        _table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        _table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        _table->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        _table->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        _table->setObjectName(QString::fromUtf8("_table"));

        horizontalLayout->addWidget(_table);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        _ok = new QPushButton(ParameterEdit);
        _ok->setObjectName(QString::fromUtf8("_ok"));

        vboxLayout->addWidget(_ok);

        _cancel = new QPushButton(ParameterEdit);
        _cancel->setObjectName(QString::fromUtf8("_cancel"));

        vboxLayout->addWidget(_cancel);

        _buttonDiv = new QFrame(ParameterEdit);
        _buttonDiv->setObjectName(QString::fromUtf8("_buttonDiv"));
        _buttonDiv->setFrameShape(QFrame::HLine);
        _buttonDiv->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(_buttonDiv);

        _new = new QPushButton(ParameterEdit);
        _new->setObjectName(QString::fromUtf8("_new"));

        vboxLayout->addWidget(_new);

        _edit = new QPushButton(ParameterEdit);
        _edit->setObjectName(QString::fromUtf8("_edit"));

        vboxLayout->addWidget(_edit);

        _list = new QPushButton(ParameterEdit);
        _list->setObjectName(QString::fromUtf8("_list"));

        vboxLayout->addWidget(_list);

        _delete = new QPushButton(ParameterEdit);
        _delete->setObjectName(QString::fromUtf8("_delete"));

        vboxLayout->addWidget(_delete);

        spacerItem = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);


        horizontalLayout->addLayout(vboxLayout);

        QWidget::setTabOrder(_table, _ok);
        QWidget::setTabOrder(_ok, _cancel);
        QWidget::setTabOrder(_cancel, _new);
        QWidget::setTabOrder(_new, _edit);
        QWidget::setTabOrder(_edit, _list);
        QWidget::setTabOrder(_list, _delete);

        retranslateUi(ParameterEdit);

        QMetaObject::connectSlotsByName(ParameterEdit);
    } // setupUi

    void retranslateUi(QWidget *ParameterEdit)
    {
        ParameterEdit->setWindowTitle(QApplication::translate("ParameterEdit", "Parameter List", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = _table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ParameterEdit", "Active", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = _table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ParameterEdit", "Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = _table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ParameterEdit", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = _table->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("ParameterEdit", "Value", 0, QApplication::UnicodeUTF8));
        _ok->setText(QApplication::translate("ParameterEdit", "&OK", 0, QApplication::UnicodeUTF8));
        _ok->setShortcut(QApplication::translate("ParameterEdit", "Alt+O", 0, QApplication::UnicodeUTF8));
        _cancel->setText(QApplication::translate("ParameterEdit", "&Cancel", 0, QApplication::UnicodeUTF8));
        _cancel->setShortcut(QApplication::translate("ParameterEdit", "Alt+C", 0, QApplication::UnicodeUTF8));
        _new->setText(QApplication::translate("ParameterEdit", "&New", 0, QApplication::UnicodeUTF8));
        _new->setShortcut(QApplication::translate("ParameterEdit", "Alt+N", 0, QApplication::UnicodeUTF8));
        _edit->setText(QApplication::translate("ParameterEdit", "&Edit", 0, QApplication::UnicodeUTF8));
        _edit->setShortcut(QApplication::translate("ParameterEdit", "Alt+E", 0, QApplication::UnicodeUTF8));
        _list->setText(QApplication::translate("ParameterEdit", "List", 0, QApplication::UnicodeUTF8));
        _list->setShortcut(QApplication::translate("ParameterEdit", "Alt+E", 0, QApplication::UnicodeUTF8));
        _delete->setText(QApplication::translate("ParameterEdit", "Delete", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ParameterEdit: public Ui_ParameterEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETEREDIT_H
