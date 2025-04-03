#include "scriptcontrolpanel.hpp"

ScriptControlPanel::ScriptControlPanel(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("📜 Script Control");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-weight: bold;");

    loadScriptBtn = new QPushButton("📂 Load Script");
    loadScriptBtn->setEnabled(false);  // Placeholder logic
    runScriptBtn = new QPushButton("▶️ Run Script");

    layout->addWidget(label);
    layout->addWidget(loadScriptBtn);
    layout->addWidget(runScriptBtn);
    layout->addStretch();

    connect(loadScriptBtn, &QPushButton::clicked, this, &ScriptControlPanel::loadScriptRequested);
    connect(runScriptBtn, &QPushButton::clicked, this, &ScriptControlPanel::runScriptRequested);
}
