/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QLabel *label_imgShow;
    QTextEdit *textEdit;
    QPushButton *pushButton_clear;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *lineEdit_thresh;
    QPushButton *pushButton_thresh;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_windowSize;
    QLabel *label_2;
    QPushButton *pushButton_erode;
    QPushButton *pushButton_dilate;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_openFile;
    QCheckBox *checkBox_isGrayScale;
    QPushButton *pushButton_toSrc;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_4;
    QLineEdit *lineEdit_avg;
    QPushButton *pushButton_mask;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_6;
    QComboBox *comboBox_adaptive;
    QPushButton *pushButton_adaptive;
    QPushButton *pushButton_userFilter;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_7;
    QComboBox *comboBox_filter;
    QPushButton *pushButton_filter;
    QWidget *tab_2;
    QPushButton *pushButton_createMask1;
    QWidget *layoutWidget5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_5;
    QLabel *label_8;
    QLineEdit *lineEdit_startX;
    QLabel *label_7;
    QLineEdit *lineEdit_startY;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_6;
    QLabel *label_9;
    QLineEdit *lineEdit_endX;
    QLabel *label_10;
    QLineEdit *lineEdit_endY;
    QWidget *layoutWidget6;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_up;
    QPushButton *pushButton_left;
    QPushButton *pushButton_increase;
    QPushButton *pushButton_increaseAngle;
    QPushButton *pushButton_down;
    QPushButton *pushButton_right;
    QPushButton *pushButton_cut;
    QPushButton *pushButton_reduceAngle;
    QWidget *layoutWidget7;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_25;
    QLabel *label_21;
    QLineEdit *lineEdit_step;
    QPushButton *pushButton_drawMask;
    QWidget *layoutWidget8;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_11;
    QLineEdit *lineEdit_scale1;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_12;
    QLineEdit *lineEdit_gap1;
    QWidget *layoutWidget9;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_16;
    QLineEdit *lineEdit_scale2;
    QHBoxLayout *horizontalLayout_23;
    QLabel *label_17;
    QLineEdit *lineEdit_gap2;
    QPushButton *pushButton_createMask2;
    QWidget *tab_3;
    QGroupBox *groupBox;
    QWidget *layoutWidget10;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_17;
    QPushButton *pushButton_openCamera;
    QPushButton *pushButton_closeCamera;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_14;
    QComboBox *comboBox_snapMode;
    QHBoxLayout *horizontalLayout_16;
    QPushButton *pushButton_startSnap;
    QPushButton *pushButton_stopSnap;
    QHBoxLayout *horizontalLayout_18;
    QPushButton *pushButton_trigger;
    QCheckBox *checkBox_calc;
    QGroupBox *groupBox_2;
    QWidget *layoutWidget11;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_32;
    QComboBox *comboBox_autoExposure;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_13;
    QLineEdit *lineEdit_exposure;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label_15;
    QComboBox *comboBox_autoWb;
    QHBoxLayout *horizontalLayout_31;
    QLabel *label_33;
    QLineEdit *lineEdit_level;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1920, 1080);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(10, 10, 1300, 740));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1298, 738));
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_imgShow = new QLabel(scrollAreaWidgetContents);
        label_imgShow->setObjectName(QStringLiteral("label_imgShow"));
        label_imgShow->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout->addWidget(label_imgShow, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(1320, 450, 581, 301));
        pushButton_clear = new QPushButton(centralWidget);
        pushButton_clear->setObjectName(QStringLiteral("pushButton_clear"));
        pushButton_clear->setGeometry(QRect(1320, 420, 75, 23));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(1320, 10, 780, 351));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        layoutWidget = new QWidget(tab);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 70, 244, 25));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        lineEdit_thresh = new QLineEdit(layoutWidget);
        lineEdit_thresh->setObjectName(QStringLiteral("lineEdit_thresh"));
        lineEdit_thresh->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_3->addWidget(lineEdit_thresh);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        pushButton_thresh = new QPushButton(layoutWidget);
        pushButton_thresh->setObjectName(QStringLiteral("pushButton_thresh"));

        horizontalLayout_4->addWidget(pushButton_thresh);

        layoutWidget1 = new QWidget(tab);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 40, 314, 25));
        horizontalLayout_10 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit_windowSize = new QLineEdit(layoutWidget1);
        lineEdit_windowSize->setObjectName(QStringLiteral("lineEdit_windowSize"));
        lineEdit_windowSize->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(lineEdit_windowSize);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);


        horizontalLayout_10->addLayout(horizontalLayout);

        pushButton_erode = new QPushButton(layoutWidget1);
        pushButton_erode->setObjectName(QStringLiteral("pushButton_erode"));

        horizontalLayout_10->addWidget(pushButton_erode);

        pushButton_dilate = new QPushButton(layoutWidget1);
        pushButton_dilate->setObjectName(QStringLiteral("pushButton_dilate"));

        horizontalLayout_10->addWidget(pushButton_dilate);

        layoutWidget2 = new QWidget(tab);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 10, 433, 29));
        horizontalLayout_9 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_openFile = new QPushButton(layoutWidget2);
        pushButton_openFile->setObjectName(QStringLiteral("pushButton_openFile"));

        horizontalLayout_2->addWidget(pushButton_openFile);

        checkBox_isGrayScale = new QCheckBox(layoutWidget2);
        checkBox_isGrayScale->setObjectName(QStringLiteral("checkBox_isGrayScale"));
        checkBox_isGrayScale->setChecked(false);

        horizontalLayout_2->addWidget(checkBox_isGrayScale);


        horizontalLayout_5->addLayout(horizontalLayout_2);

        pushButton_toSrc = new QPushButton(layoutWidget2);
        pushButton_toSrc->setObjectName(QStringLiteral("pushButton_toSrc"));

        horizontalLayout_5->addWidget(pushButton_toSrc);


        horizontalLayout_9->addLayout(horizontalLayout_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_4 = new QLabel(layoutWidget2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMaximumSize(QSize(90, 16777215));

        horizontalLayout_8->addWidget(label_4);

        lineEdit_avg = new QLineEdit(layoutWidget2);
        lineEdit_avg->setObjectName(QStringLiteral("lineEdit_avg"));
        lineEdit_avg->setMaximumSize(QSize(60, 16777215));
        lineEdit_avg->setReadOnly(true);

        horizontalLayout_8->addWidget(lineEdit_avg);


        horizontalLayout_9->addLayout(horizontalLayout_8);

        pushButton_mask = new QPushButton(tab);
        pushButton_mask->setObjectName(QStringLiteral("pushButton_mask"));
        pushButton_mask->setGeometry(QRect(10, 160, 75, 23));
        layoutWidget3 = new QWidget(tab);
        layoutWidget3->setObjectName(QStringLiteral("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 100, 222, 25));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        comboBox_adaptive = new QComboBox(layoutWidget3);
        comboBox_adaptive->setObjectName(QStringLiteral("comboBox_adaptive"));

        horizontalLayout_6->addWidget(comboBox_adaptive);

        pushButton_adaptive = new QPushButton(layoutWidget3);
        pushButton_adaptive->setObjectName(QStringLiteral("pushButton_adaptive"));

        horizontalLayout_6->addWidget(pushButton_adaptive);

        pushButton_userFilter = new QPushButton(tab);
        pushButton_userFilter->setObjectName(QStringLiteral("pushButton_userFilter"));
        pushButton_userFilter->setGeometry(QRect(90, 160, 75, 23));
        layoutWidget4 = new QWidget(tab);
        layoutWidget4->setObjectName(QStringLiteral("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(10, 130, 139, 25));
        horizontalLayout_7 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        comboBox_filter = new QComboBox(layoutWidget4);
        comboBox_filter->setObjectName(QStringLiteral("comboBox_filter"));

        horizontalLayout_7->addWidget(comboBox_filter);

        pushButton_filter = new QPushButton(layoutWidget4);
        pushButton_filter->setObjectName(QStringLiteral("pushButton_filter"));

        horizontalLayout_7->addWidget(pushButton_filter);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        pushButton_createMask1 = new QPushButton(tab_2);
        pushButton_createMask1->setObjectName(QStringLiteral("pushButton_createMask1"));
        pushButton_createMask1->setGeometry(QRect(10, 130, 75, 23));
        layoutWidget5 = new QWidget(tab_2);
        layoutWidget5->setObjectName(QStringLiteral("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(10, 10, 192, 52));
        verticalLayout = new QVBoxLayout(layoutWidget5);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_5 = new QLabel(layoutWidget5);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_11->addWidget(label_5);

        label_8 = new QLabel(layoutWidget5);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_11->addWidget(label_8);

        lineEdit_startX = new QLineEdit(layoutWidget5);
        lineEdit_startX->setObjectName(QStringLiteral("lineEdit_startX"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit_startX->sizePolicy().hasHeightForWidth());
        lineEdit_startX->setSizePolicy(sizePolicy);
        lineEdit_startX->setMinimumSize(QSize(40, 0));
        lineEdit_startX->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_11->addWidget(lineEdit_startX);

        label_7 = new QLabel(layoutWidget5);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_11->addWidget(label_7);

        lineEdit_startY = new QLineEdit(layoutWidget5);
        lineEdit_startY->setObjectName(QStringLiteral("lineEdit_startY"));
        sizePolicy.setHeightForWidth(lineEdit_startY->sizePolicy().hasHeightForWidth());
        lineEdit_startY->setSizePolicy(sizePolicy);
        lineEdit_startY->setMinimumSize(QSize(40, 0));
        lineEdit_startY->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_11->addWidget(lineEdit_startY);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_6 = new QLabel(layoutWidget5);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_12->addWidget(label_6);

        label_9 = new QLabel(layoutWidget5);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_12->addWidget(label_9);

        lineEdit_endX = new QLineEdit(layoutWidget5);
        lineEdit_endX->setObjectName(QStringLiteral("lineEdit_endX"));
        sizePolicy.setHeightForWidth(lineEdit_endX->sizePolicy().hasHeightForWidth());
        lineEdit_endX->setSizePolicy(sizePolicy);
        lineEdit_endX->setMinimumSize(QSize(40, 0));
        lineEdit_endX->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_12->addWidget(lineEdit_endX);

        label_10 = new QLabel(layoutWidget5);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_12->addWidget(label_10);

        lineEdit_endY = new QLineEdit(layoutWidget5);
        lineEdit_endY->setObjectName(QStringLiteral("lineEdit_endY"));
        sizePolicy.setHeightForWidth(lineEdit_endY->sizePolicy().hasHeightForWidth());
        lineEdit_endY->setSizePolicy(sizePolicy);
        lineEdit_endY->setMinimumSize(QSize(40, 0));
        lineEdit_endY->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_12->addWidget(lineEdit_endY);


        verticalLayout->addLayout(horizontalLayout_12);

        layoutWidget6 = new QWidget(tab_2);
        layoutWidget6->setObjectName(QStringLiteral("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(10, 70, 320, 54));
        gridLayout_2 = new QGridLayout(layoutWidget6);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_up = new QPushButton(layoutWidget6);
        pushButton_up->setObjectName(QStringLiteral("pushButton_up"));

        gridLayout_2->addWidget(pushButton_up, 0, 0, 1, 1);

        pushButton_left = new QPushButton(layoutWidget6);
        pushButton_left->setObjectName(QStringLiteral("pushButton_left"));

        gridLayout_2->addWidget(pushButton_left, 0, 1, 1, 1);

        pushButton_increase = new QPushButton(layoutWidget6);
        pushButton_increase->setObjectName(QStringLiteral("pushButton_increase"));

        gridLayout_2->addWidget(pushButton_increase, 0, 2, 1, 1);

        pushButton_increaseAngle = new QPushButton(layoutWidget6);
        pushButton_increaseAngle->setObjectName(QStringLiteral("pushButton_increaseAngle"));

        gridLayout_2->addWidget(pushButton_increaseAngle, 0, 3, 1, 1);

        pushButton_down = new QPushButton(layoutWidget6);
        pushButton_down->setObjectName(QStringLiteral("pushButton_down"));

        gridLayout_2->addWidget(pushButton_down, 1, 0, 1, 1);

        pushButton_right = new QPushButton(layoutWidget6);
        pushButton_right->setObjectName(QStringLiteral("pushButton_right"));

        gridLayout_2->addWidget(pushButton_right, 1, 1, 1, 1);

        pushButton_cut = new QPushButton(layoutWidget6);
        pushButton_cut->setObjectName(QStringLiteral("pushButton_cut"));

        gridLayout_2->addWidget(pushButton_cut, 1, 2, 1, 1);

        pushButton_reduceAngle = new QPushButton(layoutWidget6);
        pushButton_reduceAngle->setObjectName(QStringLiteral("pushButton_reduceAngle"));

        gridLayout_2->addWidget(pushButton_reduceAngle, 1, 3, 1, 1);

        layoutWidget7 = new QWidget(tab_2);
        layoutWidget7->setObjectName(QStringLiteral("layoutWidget7"));
        layoutWidget7->setGeometry(QRect(210, 10, 86, 53));
        verticalLayout_2 = new QVBoxLayout(layoutWidget7);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setSpacing(6);
        horizontalLayout_25->setObjectName(QStringLiteral("horizontalLayout_25"));
        label_21 = new QLabel(layoutWidget7);
        label_21->setObjectName(QStringLiteral("label_21"));

        horizontalLayout_25->addWidget(label_21);

        lineEdit_step = new QLineEdit(layoutWidget7);
        lineEdit_step->setObjectName(QStringLiteral("lineEdit_step"));
        lineEdit_step->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_25->addWidget(lineEdit_step);


        verticalLayout_2->addLayout(horizontalLayout_25);

        pushButton_drawMask = new QPushButton(layoutWidget7);
        pushButton_drawMask->setObjectName(QStringLiteral("pushButton_drawMask"));

        verticalLayout_2->addWidget(pushButton_drawMask);

        layoutWidget8 = new QWidget(tab_2);
        layoutWidget8->setObjectName(QStringLiteral("layoutWidget8"));
        layoutWidget8->setGeometry(QRect(310, 10, 115, 52));
        verticalLayout_3 = new QVBoxLayout(layoutWidget8);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_11 = new QLabel(layoutWidget8);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_13->addWidget(label_11);

        lineEdit_scale1 = new QLineEdit(layoutWidget8);
        lineEdit_scale1->setObjectName(QStringLiteral("lineEdit_scale1"));
        lineEdit_scale1->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_13->addWidget(lineEdit_scale1);


        verticalLayout_3->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_12 = new QLabel(layoutWidget8);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_14->addWidget(label_12);

        lineEdit_gap1 = new QLineEdit(layoutWidget8);
        lineEdit_gap1->setObjectName(QStringLiteral("lineEdit_gap1"));
        lineEdit_gap1->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_14->addWidget(lineEdit_gap1);


        verticalLayout_3->addLayout(horizontalLayout_14);

        layoutWidget9 = new QWidget(tab_2);
        layoutWidget9->setObjectName(QStringLiteral("layoutWidget9"));
        layoutWidget9->setGeometry(QRect(440, 10, 116, 52));
        verticalLayout_6 = new QVBoxLayout(layoutWidget9);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        label_16 = new QLabel(layoutWidget9);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_22->addWidget(label_16);

        lineEdit_scale2 = new QLineEdit(layoutWidget9);
        lineEdit_scale2->setObjectName(QStringLiteral("lineEdit_scale2"));
        lineEdit_scale2->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_22->addWidget(lineEdit_scale2);


        verticalLayout_6->addLayout(horizontalLayout_22);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        label_17 = new QLabel(layoutWidget9);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_23->addWidget(label_17);

        lineEdit_gap2 = new QLineEdit(layoutWidget9);
        lineEdit_gap2->setObjectName(QStringLiteral("lineEdit_gap2"));
        lineEdit_gap2->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_23->addWidget(lineEdit_gap2);


        verticalLayout_6->addLayout(horizontalLayout_23);

        pushButton_createMask2 = new QPushButton(tab_2);
        pushButton_createMask2->setObjectName(QStringLiteral("pushButton_createMask2"));
        pushButton_createMask2->setGeometry(QRect(100, 130, 75, 23));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        groupBox = new QGroupBox(tab_3);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 180, 151));
        layoutWidget10 = new QWidget(groupBox);
        layoutWidget10->setObjectName(QStringLiteral("layoutWidget10"));
        layoutWidget10->setGeometry(QRect(11, 21, 160, 117));
        verticalLayout_4 = new QVBoxLayout(layoutWidget10);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        pushButton_openCamera = new QPushButton(layoutWidget10);
        pushButton_openCamera->setObjectName(QStringLiteral("pushButton_openCamera"));

        horizontalLayout_17->addWidget(pushButton_openCamera);

        pushButton_closeCamera = new QPushButton(layoutWidget10);
        pushButton_closeCamera->setObjectName(QStringLiteral("pushButton_closeCamera"));
        pushButton_closeCamera->setEnabled(true);

        horizontalLayout_17->addWidget(pushButton_closeCamera);


        verticalLayout_4->addLayout(horizontalLayout_17);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_14 = new QLabel(layoutWidget10);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_15->addWidget(label_14);

        comboBox_snapMode = new QComboBox(layoutWidget10);
        comboBox_snapMode->setObjectName(QStringLiteral("comboBox_snapMode"));

        horizontalLayout_15->addWidget(comboBox_snapMode);


        verticalLayout_4->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        pushButton_startSnap = new QPushButton(layoutWidget10);
        pushButton_startSnap->setObjectName(QStringLiteral("pushButton_startSnap"));

        horizontalLayout_16->addWidget(pushButton_startSnap);

        pushButton_stopSnap = new QPushButton(layoutWidget10);
        pushButton_stopSnap->setObjectName(QStringLiteral("pushButton_stopSnap"));

        horizontalLayout_16->addWidget(pushButton_stopSnap);


        verticalLayout_4->addLayout(horizontalLayout_16);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        pushButton_trigger = new QPushButton(layoutWidget10);
        pushButton_trigger->setObjectName(QStringLiteral("pushButton_trigger"));

        horizontalLayout_18->addWidget(pushButton_trigger);

        checkBox_calc = new QCheckBox(layoutWidget10);
        checkBox_calc->setObjectName(QStringLiteral("checkBox_calc"));

        horizontalLayout_18->addWidget(checkBox_calc);


        verticalLayout_4->addLayout(horizontalLayout_18);

        groupBox_2 = new QGroupBox(tab_3);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 170, 180, 131));
        layoutWidget11 = new QWidget(groupBox_2);
        layoutWidget11->setObjectName(QStringLiteral("layoutWidget11"));
        layoutWidget11->setGeometry(QRect(10, 11, 162, 108));
        verticalLayout_5 = new QVBoxLayout(layoutWidget11);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        label_32 = new QLabel(layoutWidget11);
        label_32->setObjectName(QStringLiteral("label_32"));

        horizontalLayout_19->addWidget(label_32);

        comboBox_autoExposure = new QComboBox(layoutWidget11);
        comboBox_autoExposure->setObjectName(QStringLiteral("comboBox_autoExposure"));
        comboBox_autoExposure->setMinimumSize(QSize(80, 0));
        comboBox_autoExposure->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_19->addWidget(comboBox_autoExposure);


        verticalLayout_5->addLayout(horizontalLayout_19);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        label_13 = new QLabel(layoutWidget11);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_20->addWidget(label_13);

        lineEdit_exposure = new QLineEdit(layoutWidget11);
        lineEdit_exposure->setObjectName(QStringLiteral("lineEdit_exposure"));
        lineEdit_exposure->setMinimumSize(QSize(80, 0));
        lineEdit_exposure->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_20->addWidget(lineEdit_exposure);


        verticalLayout_5->addLayout(horizontalLayout_20);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        label_15 = new QLabel(layoutWidget11);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout_21->addWidget(label_15);

        comboBox_autoWb = new QComboBox(layoutWidget11);
        comboBox_autoWb->setObjectName(QStringLiteral("comboBox_autoWb"));
        comboBox_autoWb->setMinimumSize(QSize(80, 0));
        comboBox_autoWb->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_21->addWidget(comboBox_autoWb);


        verticalLayout_5->addLayout(horizontalLayout_21);

        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setSpacing(6);
        horizontalLayout_31->setObjectName(QStringLiteral("horizontalLayout_31"));
        label_33 = new QLabel(layoutWidget11);
        label_33->setObjectName(QStringLiteral("label_33"));

        horizontalLayout_31->addWidget(label_33);

        lineEdit_level = new QLineEdit(layoutWidget11);
        lineEdit_level->setObjectName(QStringLiteral("lineEdit_level"));
        lineEdit_level->setEnabled(false);
        lineEdit_level->setMinimumSize(QSize(80, 0));
        lineEdit_level->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_31->addWidget(lineEdit_level);


        verticalLayout_5->addLayout(horizontalLayout_31);

        tabWidget->addTab(tab_3, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1920, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
#ifndef QT_NO_SHORTCUT
        label->setBuddy(lineEdit_windowSize);
        label_5->setBuddy(lineEdit_startX);
        label_6->setBuddy(lineEdit_endX);
        label_21->setBuddy(lineEdit_step);
#endif // QT_NO_SHORTCUT

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label_imgShow->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        pushButton_clear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\346\230\276\347\244\272", 0));
        label_3->setText(QApplication::translate("MainWindow", "\345\233\272\345\256\232\344\272\214\345\200\274\345\214\226\351\230\210\345\200\274\357\274\232", 0));
        lineEdit_thresh->setText(QApplication::translate("MainWindow", "100", 0));
        pushButton_thresh->setText(QApplication::translate("MainWindow", "\345\233\272\345\256\232\351\230\210\345\200\274\344\272\214\345\200\274\345\214\226", 0));
        label->setText(QApplication::translate("MainWindow", "\347\252\227\345\217\243\345\244\247\345\260\217\357\274\232", 0));
        lineEdit_windowSize->setText(QApplication::translate("MainWindow", "3", 0));
        label_2->setText(QApplication::translate("MainWindow", "(1~32)", 0));
        pushButton_erode->setText(QApplication::translate("MainWindow", "\350\205\220\350\232\200", 0));
        pushButton_dilate->setText(QApplication::translate("MainWindow", "\350\206\250\350\203\200", 0));
        pushButton_openFile->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\347\211\207", 0));
        checkBox_isGrayScale->setText(QApplication::translate("MainWindow", "\347\201\260\345\272\246\346\226\271\345\274\217\346\211\223\345\274\200", 0));
        pushButton_toSrc->setText(QApplication::translate("MainWindow", "\345\216\237\345\233\276", 0));
        label_4->setText(QApplication::translate("MainWindow", "\345\233\276\345\203\217\345\271\263\345\235\207\347\201\260\345\272\246\345\200\274\357\274\232", 0));
        pushButton_mask->setText(QApplication::translate("MainWindow", "\346\250\241\346\235\277\345\244\204\347\220\206", 0));
        comboBox_adaptive->clear();
        comboBox_adaptive->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\271\263\345\235\207\345\200\274\350\207\252\351\200\202\345\272\224\346\226\271\346\263\225", 0)
         << QApplication::translate("MainWindow", "\351\253\230\346\226\257\347\252\227\345\217\243\350\207\252\351\200\202\345\272\224\346\226\271\346\263\225", 0)
        );
        pushButton_adaptive->setText(QApplication::translate("MainWindow", "\350\207\252\351\200\202\345\272\224\351\230\210\345\200\274\345\214\226", 0));
        pushButton_userFilter->setText(QApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211\346\273\244\346\263\242", 0));
        comboBox_filter->clear();
        comboBox_filter->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "R\345\210\206\351\207\217", 0)
         << QApplication::translate("MainWindow", "G\345\210\206\351\207\217", 0)
         << QApplication::translate("MainWindow", "B\345\210\206\351\207\217", 0)
         << QApplication::translate("MainWindow", "W\345\210\206\351\207\217", 0)
        );
        pushButton_filter->setText(QApplication::translate("MainWindow", "\351\242\234\350\211\262\346\273\244\346\263\242", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\346\223\215\344\275\234", 0));
        pushButton_createMask1->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220\346\250\241\346\235\2771", 0));
        label_5->setText(QApplication::translate("MainWindow", "\350\265\267\347\202\271\345\235\220\346\240\207\357\274\232", 0));
        label_8->setText(QApplication::translate("MainWindow", "X-", 0));
        lineEdit_startX->setText(QApplication::translate("MainWindow", "708", 0));
        label_7->setText(QApplication::translate("MainWindow", "Y-", 0));
        lineEdit_startY->setText(QApplication::translate("MainWindow", "964", 0));
        label_6->setText(QApplication::translate("MainWindow", "\347\273\210\347\202\271\345\235\220\346\240\207\357\274\232", 0));
        label_9->setText(QApplication::translate("MainWindow", "X-", 0));
        lineEdit_endX->setText(QApplication::translate("MainWindow", "820", 0));
        label_10->setText(QApplication::translate("MainWindow", "Y-", 0));
        lineEdit_endY->setText(QApplication::translate("MainWindow", "898", 0));
        pushButton_up->setText(QApplication::translate("MainWindow", "\344\270\212\347\247\273", 0));
        pushButton_left->setText(QApplication::translate("MainWindow", "\345\267\246\347\247\273", 0));
        pushButton_increase->setText(QApplication::translate("MainWindow", "\345\242\236\345\244\247", 0));
        pushButton_increaseAngle->setText(QApplication::translate("MainWindow", " \350\247\222\345\272\246\345\242\236\345\212\240", 0));
        pushButton_down->setText(QApplication::translate("MainWindow", "\344\270\213\347\247\273", 0));
        pushButton_right->setText(QApplication::translate("MainWindow", "\345\217\263\347\247\273", 0));
        pushButton_cut->setText(QApplication::translate("MainWindow", "\347\274\251\345\260\217", 0));
        pushButton_reduceAngle->setText(QApplication::translate("MainWindow", "\350\247\222\345\272\246\345\207\217\345\260\217", 0));
        label_21->setText(QApplication::translate("MainWindow", "\346\255\245\351\225\277\357\274\232", 0));
        lineEdit_step->setText(QApplication::translate("MainWindow", "2", 0));
        pushButton_drawMask->setText(QApplication::translate("MainWindow", "\347\273\230\345\210\266\346\250\241\346\235\277", 0));
        label_11->setText(QApplication::translate("MainWindow", "\346\257\224\345\210\227\345\233\240\345\255\2201\357\274\232", 0));
        lineEdit_scale1->setText(QApplication::translate("MainWindow", "0.3", 0));
        label_12->setText(QApplication::translate("MainWindow", "\351\227\264\351\232\2311\357\274\232", 0));
        lineEdit_gap1->setText(QApplication::translate("MainWindow", "200", 0));
        label_16->setText(QApplication::translate("MainWindow", "\346\257\224\344\276\213\345\233\240\345\255\2202\357\274\232", 0));
        lineEdit_scale2->setText(QApplication::translate("MainWindow", "0.2", 0));
        label_17->setText(QApplication::translate("MainWindow", "\351\227\264\351\232\2312\357\274\232", 0));
        lineEdit_gap2->setText(QApplication::translate("MainWindow", "500", 0));
        pushButton_createMask2->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220\346\250\241\346\235\2772", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\346\250\241\346\235\277", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "\350\256\276\345\244\207", 0));
        pushButton_openCamera->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\347\233\270\346\234\272", 0));
        pushButton_closeCamera->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\347\233\270\346\234\272", 0));
        label_14->setText(QApplication::translate("MainWindow", "\351\207\207\351\233\206\346\250\241\345\274\217\357\274\232", 0));
        comboBox_snapMode->clear();
        comboBox_snapMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\350\277\236\347\273\255\351\207\207\351\233\206", 0)
         << QApplication::translate("MainWindow", "\350\275\257\344\273\266\350\247\246\345\217\221", 0)
        );
        pushButton_startSnap->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\351\207\207\351\233\206", 0));
        pushButton_stopSnap->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\351\207\207\351\233\206", 0));
        pushButton_trigger->setText(QApplication::translate("MainWindow", "\350\275\257\344\273\266\350\247\246\345\217\221", 0));
        checkBox_calc->setText(QApplication::translate("MainWindow", "\346\265\213\351\207\217", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\346\233\235\345\205\211", 0));
        label_32->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\346\233\235\345\205\211\357\274\232", 0));
        comboBox_autoExposure->clear();
        comboBox_autoExposure->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "off", 0)
         << QApplication::translate("MainWindow", "continuous", 0)
        );
        label_13->setText(QApplication::translate("MainWindow", "\346\233\235\345\205\211\346\227\266\351\227\264\357\274\232", 0));
        lineEdit_exposure->setText(QApplication::translate("MainWindow", "60000", 0));
        label_15->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\347\231\275\345\271\263\350\241\241\357\274\232", 0));
        comboBox_autoWb->clear();
        comboBox_autoWb->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "off", 0)
         << QApplication::translate("MainWindow", "continuous", 0)
        );
        label_33->setText(QApplication::translate("MainWindow", "\346\234\237\346\234\233\347\201\260\345\272\246\357\274\232", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\347\233\270\346\234\272", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
