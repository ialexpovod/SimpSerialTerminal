
/*****************************************************************************
 * mainwindow.cpp
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
#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("Serial port debugging assistant"));
    ui->statusBar->addWidget(new QLabel(QString::fromLocal8Bit("Serial port info:")));
    ui->statusBar->addWidget(&StatusConnect);
    ui->statusBar->addWidget(&ConnectSeriaPortInfo);

    InitialSerialPortInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::InitialSerialPortInfo()
{
    ui->cBoxComPort->clear();       // 1/6
    ui->cBoxBaundRate->clear();     // 2/6
    ui->cBoxDataBits->clear();      // 3/6
    ui->cBoxStopBit->clear();       // 4/6
    ui->cBoxParityBit->clear();     // 5/6
    ui->cBoxFlowControl->clear();   // 6/6

    // Init COM-PORT
    foreach (const QSerialPortInfo &infoSerial, QSerialPortInfo::availablePorts())
    {
        QString items_ports = infoSerial.portName();
        qDebug() << "List port:" << items_ports;
        ui->cBoxComPort->addItem(infoSerial.portName());
    }

    // Init Baund Rate
    QList<QString>items_cBoxBaundRate;
    items_cBoxBaundRate<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
    ui->cBoxBaundRate->addItems(items_cBoxBaundRate);
    ui->cBoxBaundRate->setCurrentIndex(1);
    qDebug()<< "List Baund Rate: " << items_cBoxBaundRate;

    // Init Data Bits
    QList<QString>items_DataBits;
    items_DataBits<<"5"<<"6"<<"7"<<"8";
    ui->cBoxDataBits->addItems(items_DataBits);
    ui->cBoxDataBits->setCurrentIndex(3);
    qDebug() << "List Data bits: " << items_DataBits;

    // Init Stop Bits
    QList<QString>items_cBoxStopBit;
    items_cBoxStopBit<<"1"<<"1.5"<<"2";
    ui->cBoxStopBit->addItems(items_cBoxStopBit);

    // Init Parity Bit
    QList<QString>items_cBoxParityBit;
    items_cBoxParityBit<<"None"<<"Even"<<"Odd"<<"Mark"<<"Space";
    ui->cBoxParityBit->addItems(items_cBoxParityBit);

    // Init
    QList<QString>items_cBoxFlowControl;
    items_cBoxFlowControl<<"None"<<"RTS/CTS"<<"XON/XOFF";;
    ui->cBoxFlowControl->addItems(items_cBoxFlowControl);


}

void MainWindow::Write()
{
    port.clear();

    if(ASCII_HEXControl == ASCII_HEX::HEX)
    {
        QString str = ui->tEditSend->toPlainText();
        QByteArray enter = str.toLatin1();
        qDebug()<<QString::fromLocal8Bit("HEX:") << str << enter;
        port.write(enter);
    }

    if( ASCII_HEXControl == ASCII_HEX::ASCII)
    {
        // todo
        QString str = "page set";
        QByteArray flag;
        flag.append(0xff);
        flag.append(0xff);
        flag.append(0xff);
        QByteArray enter = str.toLatin1()+flag;
        // toHMI = enter;
        qDebug()<<"USE:"<<enter;
        qDebug()<<"USE To Hex:"<<enter.toHex();
        port.write(enter);
    }
}

void MainWindow::on_btnSend_clicked()
{
    Write();
}

void MainWindow::on_btnOpen_clicked()
{
    Open = !Open;
    if(Open)
    {
        qint32 BaudRate;
        QSerialPort::DataBits DataBits;
        QSerialPort::Parity Parity;
        QSerialPort::StopBits StopBits;
        QSerialPort::FlowControl FlowControl;

        // COM-PORT
        foreach (const QSerialPortInfo &infoSerial, QSerialPortInfo::availablePorts())
        {
            if(infoSerial.portName() == ui->cBoxComPort->currentText())
            {
                portInfo = infoSerial;
                qDebug()<<QString::fromLocal8Bit("Select serial port:")<<portInfo.portName();
            }
        }

        // BAUND RATE
        if(ui->cBoxBaundRate->currentText() == "9600") BaudRate = QSerialPort::Baud9600;
        else if(ui->cBoxBaundRate->currentText() == "19200") BaudRate = QSerialPort::Baud19200;
        else if(ui->cBoxBaundRate->currentText() == "38400") BaudRate = QSerialPort::Baud38400;
        else if(ui->cBoxBaundRate->currentText() == "57600") BaudRate = QSerialPort::Baud57600;
        else if(ui->cBoxBaundRate->currentText() == "115200") BaudRate = QSerialPort::Baud115200;

        // DATA BITS
        if(ui->cBoxDataBits->currentText() == "5") DataBits = QSerialPort::Data5;
        else if(ui->cBoxDataBits->currentText() == "6") DataBits = QSerialPort::Data6;
        else if(ui->cBoxDataBits->currentText() == "7") DataBits = QSerialPort::Data7;
        else if(ui->cBoxDataBits->currentText() == "8") DataBits = QSerialPort::Data8;

        // PARITY
        if(ui->cBoxParityBit->currentText() == "None") Parity = QSerialPort::NoParity;
        else if(ui->cBoxParityBit->currentText() == "Even") Parity = QSerialPort::EvenParity;
        else if(ui->cBoxParityBit->currentText() == "Odd") Parity = QSerialPort::OddParity;
        else if(ui->cBoxParityBit->currentText() == "Mark") Parity = QSerialPort::MarkParity;
        else if(ui->cBoxParityBit->currentText() == "Space") Parity = QSerialPort::SpaceParity;

        // STOP BITS
        if(ui->cBoxStopBit->currentText() == "1") StopBits = QSerialPort::OneStop;
        else if(ui->cBoxStopBit->currentText() == "1.5") StopBits = QSerialPort::OneAndHalfStop;
        else if(ui->cBoxStopBit->currentText() == "2") StopBits = QSerialPort::TwoStop;

        // FLOW CONTROL
        if(ui->cBoxFlowControl->currentText() == "None") FlowControl = QSerialPort::NoFlowControl;
        else if(ui->cBoxFlowControl->currentText() == "RTS/CTS") FlowControl = QSerialPort::HardwareControl;
        else if(ui->cBoxFlowControl->currentText() == "XON/XOFF") FlowControl = QSerialPort::SoftwareControl;

        // Fill to the port
        {
            port.setPort(portInfo);
            port.setBaudRate(BaudRate);
            port.setDataBits(DataBits);
            port.setParity(Parity);
            port.setStopBits(StopBits);
            port.setFlowControl(FlowControl);
        }
        port.clearError();

        int ret = port.open(QIODevice::ReadWrite);
        if(ret)
        {
            qDebug()<<QString::fromLocal8Bit("Successfully opened the serial port!");
            QString connect = port.portName() +" "+ QString::number(port.baudRate()) + " " +ui->cBoxDataBits->currentText()+" "+ ui->cBoxParityBit->currentText()+" "+ui->cBoxStopBit->currentText()+" "+ui->cBoxFlowControl->currentText();
            this->StatusConnect.setText(QString::fromLocal8Bit("Connect  "));
            this->ConnectSeriaPortInfo.setText(connect);

            {
                // Connected to all non-optional
                ui->cBoxComPort->setEnabled(false);
                ui->cBoxBaundRate->setEnabled(false);
                ui->cBoxDataBits->setEnabled(false);
                ui->cBoxParityBit->setEnabled(false);
                ui->cBoxStopBit->setEnabled(false);
                ui->cBoxFlowControl->setEnabled(false);
                ui->btnOpen->setText(QString::fromLocal8Bit("Close"));
            }

            time.start(1000);
        }
        else
        {
            QString Error = port.errorString();
            qDebug() << Error;
            this->StatusConnect.setText(QString::fromLocal8Bit("Error"));
            this->ConnectSeriaPortInfo.setText(Error);
        }
    }
    else
    {
        // todo
        port.clear();
        port.close();
        {
            // Connected to all non-optional
            ui->cBoxComPort->setEnabled(true);
            ui->cBoxBaundRate->setEnabled(true);
            ui->cBoxDataBits->setEnabled(true);
            ui->cBoxParityBit->setEnabled(true);
            ui->cBoxStopBit->setEnabled(true);
            ui->cBoxFlowControl->setEnabled(true);
            ui->btnOpen->setText(QString::fromLocal8Bit("Open"));
        }
        this->StatusConnect.setText(QString::fromLocal8Bit("disconnect"));
        this->ConnectSeriaPortInfo.setText("");

    }
}

void MainWindow::on_rBtnASCIISend_clicked()
{
    ASCII_HEXControl = ASCII_HEX::HEX;
}

void MainWindow::on_rBtnHEXSend_clicked()
{
    ASCII_HEXControl = ASCII_HEX::ASCII;
}



void MainWindow::on_btnClearSend_clicked()
{
    ui->tEditSend->clear();
}

void MainWindow::on_btnClearReceive_clicked()
{
    ui->tEditSend->clear();
}


