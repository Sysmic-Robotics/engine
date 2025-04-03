#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class RobotControlPanel : public QWidget {
    Q_OBJECT

public:
    RobotControlPanel(QWidget *parent = nullptr);

signals:
    void targetPointRequested();
    void faceToPointRequested();

private:
    QPushButton *targetPointBtn;
    QPushButton *faceToBtn;

public slots:
    void resetButtons();

private slots:
    void handleTargetPointClicked();
    void handleFaceToClicked();
};
