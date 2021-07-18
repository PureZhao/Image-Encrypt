#include "MainWindow.h"
#include <Windows.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//设置界面基本信息
	this->setWindowTitle("Image Encryption");
	this->setWindowIcon(QIcon(":/Icon/lock.png"));
	ui.procTypeComboBox->addItem("Encrypt");
	ui.procTypeComboBox->addItem("Decrypt");
	// Connect Select Btn
	connect(ui.selectFileBtn, &QPushButton::clicked, [=]() {
		ui.srcImgShowLabel->clear();
		ui.procImgShowLabel->clear();
		QString filePath = QFileDialog::getOpenFileName(nullptr,
			"Select an image",
			".",
			"JPEG Files(*.jpg);;PNG Files(*.png)");
		if (filePath == "") {
			QMessageBox::information(this, "Select File", "You need to select an image file");
			return;
		}
		// 检查格式
		QStringList suffixes = { {".jpg"},{".jpeg"},{".png"} };
		bool isCorrect = false;
		for (int i = 0; i < suffixes.length(); i++)
		{
			if (filePath.endsWith(suffixes[i]))
				isCorrect = true;
		}
		if (!isCorrect)
		{
			QMessageBox::warning(this, "Select File", "This format is not Supported Now!");
			return;
		}
		QStringList filePathParts = filePath.split('/');
		fileName = filePathParts.back();
		// 读取图片
		ui.selectFilePath->setText(filePath);
		sourceImage.load(filePath);
		//设置展示宽高
		int h = sourceImage.height();
		int w = sourceImage.width();
		if (h < 100 || w < 100) {
			QMessageBox::warning(this, "Image Size", "Image size is too small !!!!!");
			return;
		}
		if (h > w) {
			double rate = 400.0 / h;
			h = 400;
			w = int(w * rate);
			ui.srcImgShowLabel->move((450 - w) / 2, 50);
			ui.procImgShowLabel->move((450 - w) / 2, 50);
		}
		else {
			double rate = 450.0 / w;
			h = int(h * rate);
			w = 450;
			ui.srcImgShowLabel->move(0, (450 - h) / 2 + 25);
			ui.procImgShowLabel->move(0, (450 - h) / 2 + 25);
		}
		displayAreaSize.setWidth(w);
		displayAreaSize.setHeight(h);
		ui.srcImgShowLabel->resize(displayAreaSize);
		ui.procImgShowLabel->resize(displayAreaSize);
		QImage srcDisplayImage = sourceImage.scaled(displayAreaSize);
		ui.srcImgShowLabel->setPixmap(QPixmap::fromImage(srcDisplayImage));

	});
	// Connect Go Btn
	connect(ui.goBtn, &QPushButton::clicked, [=]() {
		//获取操作代码
		QString opType = ui.procTypeComboBox->currentText();
		QString code = ui.codeText->text() == "" ?
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX" : ui.codeText->text();
		Transformer en(code);		//实例化变换对象
		if (opType == "Encrypt")
		{
			QImage padded = Transformer::paddingImage(sourceImage);
			dstImage = en.encode(padded);
			//保存原始尺寸到图像中 有些散 后续整合
			en.recordOriginSize(sourceImage.width(), sourceImage.height(), dstImage);
		}
		else {
			dstImage = en.decode(sourceImage);
		}
		lastOperation = opType;
		QPixmap procDisplayImage = QPixmap::fromImage(dstImage).scaled(displayAreaSize);
		ui.procImgShowLabel->setPixmap(procDisplayImage);
	});
	// Connect Save Btn
	connect(ui.saveBtn, &QPushButton::clicked, [=]() {
		QString savePath = QFileDialog::getExistingDirectory(nullptr, "Save", ".");
		if (savePath == "" || !QFileInfo::exists(savePath)) {
			QMessageBox::warning(this, "Directory Not Exist", "The Directory is NOT exist");
			return;
		}
		QStringList fileNameParts = fileName.split('.');
		if (lastOperation == "Encrypt") {
			savePath += "/" + fileNameParts.front() + "_encrypted.png";
		}
		else {
			QStringList fileNameParts = fileName.split('.');
			savePath += "/" + fileNameParts.front() + "_decrypted.png";
		}
		dstImage.save(savePath);
	});
}

