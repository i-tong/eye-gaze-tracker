/********************************************************************************
** Form generated from reading UI file 'HeadCompensationDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEADCOMPENSATIONDIALOG_H
#define UI_HEADCOMPENSATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HeadCompensationDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_introduction;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_left;
    QPushButton *pushButton_right;
    QPushButton *pushButton_up;
    QPushButton *pushButton_down;
    QLabel *label_instructions;
    QSpacerItem *verticalSpacer;
    QProgressBar *progressBar;
    QPushButton *pushButton_finished;

    void setupUi(QDialog *HeadCompensationDialog)
    {
        if (HeadCompensationDialog->objectName().isEmpty())
            HeadCompensationDialog->setObjectName(QStringLiteral("HeadCompensationDialog"));
        HeadCompensationDialog->resize(400, 300);
        verticalLayout_2 = new QVBoxLayout(HeadCompensationDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_introduction = new QLabel(HeadCompensationDialog);
        label_introduction->setObjectName(QStringLiteral("label_introduction"));

        verticalLayout->addWidget(label_introduction);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_left = new QPushButton(HeadCompensationDialog);
        pushButton_left->setObjectName(QStringLiteral("pushButton_left"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_left->sizePolicy().hasHeightForWidth());
        pushButton_left->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton_left);

        pushButton_right = new QPushButton(HeadCompensationDialog);
        pushButton_right->setObjectName(QStringLiteral("pushButton_right"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_right->sizePolicy().hasHeightForWidth());
        pushButton_right->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButton_right);

        pushButton_up = new QPushButton(HeadCompensationDialog);
        pushButton_up->setObjectName(QStringLiteral("pushButton_up"));

        horizontalLayout->addWidget(pushButton_up);

        pushButton_down = new QPushButton(HeadCompensationDialog);
        pushButton_down->setObjectName(QStringLiteral("pushButton_down"));

        horizontalLayout->addWidget(pushButton_down);


        verticalLayout->addLayout(horizontalLayout);

        label_instructions = new QLabel(HeadCompensationDialog);
        label_instructions->setObjectName(QStringLiteral("label_instructions"));

        verticalLayout->addWidget(label_instructions);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        progressBar = new QProgressBar(HeadCompensationDialog);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        pushButton_finished = new QPushButton(HeadCompensationDialog);
        pushButton_finished->setObjectName(QStringLiteral("pushButton_finished"));

        verticalLayout->addWidget(pushButton_finished);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(HeadCompensationDialog);

        QMetaObject::connectSlotsByName(HeadCompensationDialog);
    } // setupUi

    void retranslateUi(QDialog *HeadCompensationDialog)
    {
        HeadCompensationDialog->setWindowTitle(QApplication::translate("HeadCompensationDialog", "Dialog", Q_NULLPTR));
        label_introduction->setText(QApplication::translate("HeadCompensationDialog", "Calibrate each of the four directions below.", Q_NULLPTR));
        pushButton_left->setText(QApplication::translate("HeadCompensationDialog", "Left", Q_NULLPTR));
        pushButton_right->setText(QApplication::translate("HeadCompensationDialog", "Right", Q_NULLPTR));
        pushButton_up->setText(QApplication::translate("HeadCompensationDialog", "Up", Q_NULLPTR));
        pushButton_down->setText(QApplication::translate("HeadCompensationDialog", "Down", Q_NULLPTR));
        label_instructions->setText(QString());
        pushButton_finished->setText(QApplication::translate("HeadCompensationDialog", "Finished", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class HeadCompensationDialog: public Ui_HeadCompensationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEADCOMPENSATIONDIALOG_H
/*
ui_HeadCompensationDialog.h

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2017-2018 University of British Columbia

--- begin license - do not edit ---
    This file is a part of CGaze UI.
    
    CGaze UI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cgaze UI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cgaze UI.  If not, see <https://www.gnu.org/licenses/>.
--- end license ---
*/

