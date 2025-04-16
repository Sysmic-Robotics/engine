#include "scriptcontrolpanel.hpp"

ScriptControlPanel::ScriptControlPanel(LuaInterface *luaInterface, QWidget *parent) : QWidget(parent) , m_luaInterface(luaInterface) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("📜 Script Control");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-weight: bold;");
    runScriptBtn = new QPushButton("▶️ Run Script");

    layout->addWidget(label);
    layout->addWidget(runScriptBtn);
    layout->addStretch();

    qDebug() << "[ScriptControlPanel] luaInterface @" << luaInterface;
    connect(runScriptBtn, &QPushButton::clicked, this, &ScriptControlPanel::onRunScriptClicked);
}


void ScriptControlPanel::onRunScriptClicked() {
    m_luaInterface->runScript();
}

