#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include "luainterface.hpp" // Ensure this header defines LuaInterface
class ScriptControlPanel : public QWidget {
    Q_OBJECT

    
public:
    ScriptControlPanel(LuaInterface *luaInterface, QWidget *parent = nullptr);


private slots:
    void onRunScriptClicked();


private:
    QPushButton *runScriptBtn;    
    LuaInterface* m_luaInterface;   // renamed to avoid shadowing
};
