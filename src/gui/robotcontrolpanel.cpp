#include "robotcontrolpanel.hpp"

RobotControlPanel::RobotControlPanel(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("🔹 Robot Control");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-weight: bold;");

    targetPointBtn = new QPushButton("🎯 Select Target");
    faceToBtn = new QPushButton("🎯 Point to Aim");

    layout->addWidget(label);
    layout->addWidget(targetPointBtn);
    layout->addWidget(faceToBtn);
    layout->addStretch();

    connect(targetPointBtn, &QPushButton::clicked, this, &RobotControlPanel::handleTargetPointClicked);
    connect(faceToBtn, &QPushButton::clicked, this, &RobotControlPanel::handleFaceToClicked);
}

void RobotControlPanel::handleTargetPointClicked() {
    emit targetPointRequested();
    targetPointBtn->setText("Click on Field...");
    faceToBtn->setEnabled(false);
}

void RobotControlPanel::handleFaceToClicked() {
    emit faceToPointRequested();
    faceToBtn->setText("Click on Field...");
    targetPointBtn->setEnabled(false);
}

void RobotControlPanel::resetButtons() {
    targetPointBtn->setText("🎯 Select Target");
    targetPointBtn->setEnabled(true);
    faceToBtn->setText("🎯 Point to Aim");
    faceToBtn->setEnabled(true);
}
