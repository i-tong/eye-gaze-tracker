/*
GuiGazeParamToolbar.cpp

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2018 University of British Columbia

--- begin license - do not edit ---

    This file is part of CGaze UI. 
   
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




#include "GuiParamToolbar.h"


GazeParamToolbar::GazeParamToolbar(void) {
	initLayout();

	// Initialize values
    textBox_numPixKeep->setText("0.3");
    textBox_numPixKeepROI->setText("N/A");
	textBox_roughPupilMinIQ->setText("0.82");
    textBox_roughPupilMinArea->setText("500");
    textBox_roughPupilMaxArea->setText("10000");
    textBox_finePupilThresMultiplier->setText("1.1");
	textBox_finePupilMinIQ->setText("0.82");
    textBox_finePupilMinArea->setText("500");
    textBox_finePupilMaxArea->setText("10000");

	// Event handling
	connect(pushButton_set,SIGNAL(clicked()),this,SLOT(onSetButtonClicked()));
}


GazeParamToolbar::~GazeParamToolbar(void) {
}

void GazeParamToolbar::initLayout() {
	pushButton_set = new QPushButton;
	pushButton_set->setText("Set Parameters");
	textBox_numPixKeep = new QLineEdit;
	QLabel* labelNumPixKeep = new QLabel("Pixel % Threshold");
	textBox_numPixKeepROI = new QLineEdit;
	QLabel* labelNumPixKeepROI = new QLabel("Pixel % Threshold (ROI)");
	textBox_roughPupilMinIQ = new QLineEdit;
	QLabel* labelRoughPupilMinIQ = new QLabel("Rough Min IQ");
	textBox_roughPupilMinArea = new QLineEdit;
	QLabel* labelRoughPupilMinArea = new QLabel("Rough Min Area");
	textBox_roughPupilMaxArea = new QLineEdit;
	QLabel* labelRoughPupilMaxArea = new QLabel("Rough Max Area");
	textBox_finePupilThresMultiplier = new QLineEdit;
	QLabel* labelFinePupilThresMultiplier = new QLabel("Fine Thres Mult");
	textBox_finePupilMinIQ = new QLineEdit;
	QLabel* labelFinePupilMinIQ = new QLabel("Fine Min IQ");
	textBox_finePupilMinArea = new QLineEdit;
	QLabel* labelFinePupilMinArea = new QLabel("Fine Min Area");
	textBox_finePupilMaxArea = new QLineEdit;
	QLabel* labelFinePupilMaxArea = new QLabel("Fine Max Area");

	QGridLayout* layout = new QGridLayout;
	
	layout->addWidget(labelNumPixKeep,0,0);
	layout->addWidget(textBox_numPixKeep,0,1);
	layout->addWidget(labelNumPixKeepROI,1,0);
	layout->addWidget(textBox_numPixKeepROI,1,1);

	layout->addWidget(labelRoughPupilMinIQ,2,0);
	layout->addWidget(textBox_roughPupilMinIQ,2,1);

	layout->addWidget(labelRoughPupilMinArea,3,0);
	layout->addWidget(textBox_roughPupilMinArea,3,1);

	layout->addWidget(labelRoughPupilMaxArea,4,0);
	layout->addWidget(textBox_roughPupilMaxArea,4,1);

	layout->addWidget(labelFinePupilThresMultiplier,5,0);
	layout->addWidget(textBox_finePupilThresMultiplier,5,1);

	layout->addWidget(labelFinePupilMinIQ,6,0);
	layout->addWidget(textBox_finePupilMinIQ,6,1);

	layout->addWidget(labelFinePupilMinArea,7,0);
	layout->addWidget(textBox_finePupilMinArea,7,1);

	layout->addWidget(labelFinePupilMaxArea,8,0);
	layout->addWidget(textBox_finePupilMaxArea,8,1);

	QWidget* paramInputWidget = new QWidget;
	paramInputWidget->setLayout(layout);
	this->addWidget(paramInputWidget);
	this->addWidget(pushButton_set);

}

void GazeParamToolbar::onSetButtonClicked() {
	// Check ranges for values
	bool dataOk = true;
	QString textVal;
	textVal = textBox_numPixKeep->text();
	if (textVal.toFloat() < 0 || textVal.toFloat() > 1) {
		dataOk = false;
	} else {
		paramHash.insert("numPixKeep",textVal.toFloat());
	}

	textVal = textBox_numPixKeepROI->text();
	if (textVal.toFloat() < 0 || textVal.toFloat() > 1) {
		dataOk = false;
	} else {
		paramHash.insert("numPixKeepROI",textVal.toFloat());
	}

	textVal = textBox_roughPupilMinIQ->text();
	if (textVal.toFloat() < 0 || textVal.toFloat() > 1) {
		dataOk = false;
	} else {
		paramHash.insert("roughPupilMinIQ",textVal.toFloat());
	}

	textVal = textBox_roughPupilMinArea->text();
	if (textVal.toFloat() < 0 ) {
		dataOk = false;
	} else {
		paramHash.insert("roughPupilMinArea",textVal.toFloat());
	}

	textVal = textBox_roughPupilMaxArea->text();
	if (textVal.toFloat() < 0 ) {
		dataOk = false;
	} else {
		paramHash.insert("roughPupilMaxArea",textVal.toFloat());
	}

	textVal = textBox_finePupilThresMultiplier->text();
	if (textVal.toFloat() <= 0) {
		dataOk = false;
	} else {
		paramHash.insert("finePupilThresMultiplier",textVal.toFloat());
	}

	textVal = textBox_finePupilMinIQ->text();
	if (textVal.toFloat() < 0 || textVal.toFloat() > 1) {
		dataOk = false;
	} else {
		paramHash.insert("finePupilMinIQ",textVal.toFloat());
	}

	textVal = textBox_finePupilMinArea->text();
	if (textVal.toFloat() < 0) {
		dataOk = false;
	} else {
		paramHash.insert("finePupilMinArea",textVal.toFloat());
	}

	textVal = textBox_finePupilMaxArea->text();
	if (textVal.toFloat() < 0) {
		dataOk = false;
	} else {
		paramHash.insert("finePupilMaxArea",textVal.toFloat());
	}


	// Place new data in hash
	if (dataOk == true) {
		emit paramChanged(paramHash);
	}


}


/*
	// Initialize values
	textBox_numPixKeep->setText("");
	textBox_numPixKeepROI->setText("");
	textBox_roughPupilMinIQ->setText("");
	textBox_roughPupilMinArea->setText("");
	textBox_roughPupilMaxArea->setText("");
	textBox_finePupilThresMultiplier->setText("");
	textBox_finePupilMinIQ->setText("");
	textBox_finePupilMinArea->setText("");
	textBox_finePupilMaxArea->setText("");
*/
