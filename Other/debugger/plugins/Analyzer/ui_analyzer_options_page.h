/********************************************************************************
** Form generated from reading UI file 'analyzer_options_page.ui'
**
** Created: Sat Sep 15 17:40:48 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALYZER_OPTIONS_PAGE_H
#define UI_ANALYZER_OPTIONS_PAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AnalyzerOptionsPage
{
public:
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *AnalyzerOptionsPage)
    {
        if (AnalyzerOptionsPage->objectName().isEmpty())
            AnalyzerOptionsPage->setObjectName(QString::fromUtf8("AnalyzerOptionsPage"));
        AnalyzerOptionsPage->resize(400, 300);
        verticalLayout = new QVBoxLayout(AnalyzerOptionsPage);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        checkBox = new QCheckBox(AnalyzerOptionsPage);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout->addWidget(checkBox);

        verticalSpacer = new QSpacerItem(20, 262, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(AnalyzerOptionsPage);

        QMetaObject::connectSlotsByName(AnalyzerOptionsPage);
    } // setupUi

    void retranslateUi(QWidget *AnalyzerOptionsPage)
    {
        AnalyzerOptionsPage->setWindowTitle(QApplication::translate("AnalyzerOptionsPage", "Analyzer Plugin", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("AnalyzerOptionsPage", "Use fuzzy logic to find functions", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AnalyzerOptionsPage: public Ui_AnalyzerOptionsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALYZER_OPTIONS_PAGE_H
