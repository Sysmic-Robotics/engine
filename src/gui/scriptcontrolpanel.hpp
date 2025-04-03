#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class ScriptControlPanel : public QWidget {
    Q_OBJECT

public:
    ScriptControlPanel(QWidget *parent = nullptr);

signals:
    void loadScriptRequested();
    void runScriptRequested();

private:
    QPushButton *loadScriptBtn;
    QPushButton *runScriptBtn;
};
