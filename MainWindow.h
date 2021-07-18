#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QPainter>
#include "Transformer.h"
#include <QMap>
#include <QFile>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QAction>
#include <QDate>
#include <QTime>
#include <QMessageBox>
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	QImage sourceImage;
	QImage dstImage;
	QSize displayAreaSize;
	QString fileName;
	QString lastOperation;
private:
	Ui::MainWindowClass ui;
};
