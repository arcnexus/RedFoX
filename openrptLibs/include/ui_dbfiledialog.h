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
** Form generated from reading UI file 'dbfiledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBFILEDIALOG_H
#define UI_DBFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DBFileDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QTreeWidget *_list;
    QHBoxLayout *hboxLayout;
    QLabel *_lblReport;
    QLineEdit *_name;
    QLabel *_gradeLabel;
    QSpinBox *_grade;
    QVBoxLayout *vboxLayout;
    QPushButton *_btnOk;
    QPushButton *_btnCancel;
    QSpacerItem *spacerItem;

    void setupUi(QDialog *DBFileDialog)
    {
        if (DBFileDialog->objectName().isEmpty())
            DBFileDialog->setObjectName(QStringLiteral("DBFileDialog"));
        DBFileDialog->resize(506, 291);
        horizontalLayout = new QHBoxLayout(DBFileDialog);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        _list = new QTreeWidget(DBFileDialog);
        _list->setObjectName(QStringLiteral("_list"));
        _list->setColumnCount(2);

        verticalLayout->addWidget(_list);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        _lblReport = new QLabel(DBFileDialog);
        _lblReport->setObjectName(QStringLiteral("_lblReport"));

        hboxLayout->addWidget(_lblReport);

        _name = new QLineEdit(DBFileDialog);
        _name->setObjectName(QStringLiteral("_name"));

        hboxLayout->addWidget(_name);

        _gradeLabel = new QLabel(DBFileDialog);
        _gradeLabel->setObjectName(QStringLiteral("_gradeLabel"));
        _gradeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        hboxLayout->addWidget(_gradeLabel);

        _grade = new QSpinBox(DBFileDialog);
        _grade->setObjectName(QStringLiteral("_grade"));
        _grade->setMinimum(-1);
        _grade->setValue(-1);

        hboxLayout->addWidget(_grade);


        verticalLayout->addLayout(hboxLayout);


        horizontalLayout->addLayout(verticalLayout);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        _btnOk = new QPushButton(DBFileDialog);
        _btnOk->setObjectName(QStringLiteral("_btnOk"));

        vboxLayout->addWidget(_btnOk);

        _btnCancel = new QPushButton(DBFileDialog);
        _btnCancel->setObjectName(QStringLiteral("_btnCancel"));

        vboxLayout->addWidget(_btnCancel);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);


        horizontalLayout->addLayout(vboxLayout);

#ifndef QT_NO_SHORTCUT
        _lblReport->setBuddy(_name);
        _gradeLabel->setBuddy(_grade);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(_name, _grade);
        QWidget::setTabOrder(_grade, _btnOk);
        QWidget::setTabOrder(_btnOk, _btnCancel);
        QWidget::setTabOrder(_btnCancel, _list);

        retranslateUi(DBFileDialog);
        QObject::connect(_btnOk, SIGNAL(clicked()), DBFileDialog, SLOT(accept()));
        QObject::connect(_btnCancel, SIGNAL(clicked()), DBFileDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DBFileDialog);
    } // setupUi

    void retranslateUi(QDialog *DBFileDialog)
    {
        DBFileDialog->setWindowTitle(QApplication::translate("DBFileDialog", "Database File", 0));
        QTreeWidgetItem *___qtreewidgetitem = _list->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("DBFileDialog", "Grade", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("DBFileDialog", "Name", 0));
        _lblReport->setText(QApplication::translate("DBFileDialog", "Report Name:", 0));
        _gradeLabel->setText(QApplication::translate("DBFileDialog", "Grade:", 0));
        _grade->setSpecialValueText(QApplication::translate("DBFileDialog", "##", 0));
        _btnOk->setText(QApplication::translate("DBFileDialog", "&OK", 0));
        _btnCancel->setText(QApplication::translate("DBFileDialog", "&Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class DBFileDialog: public Ui_DBFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBFILEDIALOG_H
