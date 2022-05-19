
/*****************************************************************************
 * mainwindow.h
 *
 * Created: 19.05.2022 2022 by Alex Povod
 *
 * Copyright (c) 2020. All rights reserved
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/*-----------------Init SerialPort-----------------*/
// Qt Serial Port provides the basic functionality, which
// includes configuring, I/O operations, getting and setting
// the control signals of the RS-232 pinouts.
#include <QtSerialPort>
#include <QtSerialPortVersion>
#include <QSerialPort>
#include <QSerialPortInfo>


#include <QDebug>
#include <QByteArray>
#include <QLabel>
#include <QTimer> // provides repetitive and single-shot timers
#include <QThread> //  platform-independent way to manage threads.

#include <string>

enum ASCII_HEX {ASCII, HEX,};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitialSerialPortInfo();


private slots:
    void Write();

    void on_rBtnASCIISend_clicked();
    void on_rBtnHEXSend_clicked();


    void on_btnSend_clicked();

    void on_btnClearSend_clicked();

    void on_btnClearReceive_clicked();

    void on_btnOpen_clicked();

private:
    Ui::MainWindow *ui;

    bool ASCII_HEXControl;

    QSerialPortInfo portInfo;
    QSerialPort port;


    QTimer time;

    QLabel StatusConnect;
    QLabel ConnectSeriaPortInfo;

    bool Open;
};

#endif // MAINWINDOW_H
