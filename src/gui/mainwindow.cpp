#include "mainwindow.hpp"
#include <QMouseEvent>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    setupLeftPanel();
    mainLayout->addWidget(leftPanel);

    scene = new QGraphicsScene(0, 0, 900, 600, this);
    scene->setBackgroundBrush(QBrush(Qt::lightGray));
    view = new QGraphicsView(scene, this);
    mainLayout->addWidget(view);

    setCentralWidget(centralWidget);

    QPen pen(Qt::white);
    scene->addRect(0, 0, 900, 600, pen);
}

void MainWindow::setupLeftPanel() {
    leftPanel = new QWidget(this);
    buttonLayout = new QVBoxLayout(leftPanel);

    // Arrow Buttons
    leftArrowBtn = new QPushButton("←");
    rightArrowBtn = new QPushButton("→");
    
    QHBoxLayout *arrowLayout = new QHBoxLayout();
    arrowLayout->addWidget(leftArrowBtn);
    arrowLayout->addWidget(rightArrowBtn);

    // Info Panel
    robotInfoLabel = new QLabel("No robot selected");
    robotInfoLabel->setAlignment(Qt::AlignCenter);

    buttonLayout->addLayout(arrowLayout);
    buttonLayout->addWidget(robotInfoLabel);
    buttonLayout->addStretch();

    // Connect buttons
    connect(leftArrowBtn, &QPushButton::clicked, [=]() { selectNextRobot(-1); });
    connect(rightArrowBtn, &QPushButton::clicked, [=]() { selectNextRobot(1); });

    leftPanel->setLayout(buttonLayout);
}

void MainWindow::updateRobot(int id, int team, QVector2D position, float orientation) {
    QMap<int, RobotItem *> &robots = (team == 0) ? blueRobots : yellowRobots;
    QColor color = (team == 0) ? Qt::blue : Qt::yellow;

    if (!robots.contains(id)) {
        RobotItem *robot = new RobotItem(id, color);
        scene->addItem(robot);
        robots.insert(id, robot);
        robotIds.append(id);
    }

    robots[id]->setPosition(position);
    robots[id]->setOrientation(orientation);
}

void MainWindow::selectNextRobot(int direction) {
    if (robotIds.isEmpty()) return;

    selectedRobotIndex += direction;
    if (selectedRobotIndex >= robotIds.size()) selectedRobotIndex = 0;
    if (selectedRobotIndex < 0) selectedRobotIndex = robotIds.size() - 1;

    selectedRobotId = robotIds[selectedRobotIndex];
    selectedTeam = (blueRobots.contains(selectedRobotId)) ? 0 : 1;

    updateInfoPanel();
    emit robotSelected(selectedRobotId, selectedTeam);
}

void MainWindow::updateInfoPanel() {
    if (selectedRobotId == -1) {
        robotInfoLabel->setText("No robot selected");
        return;
    }

    // Get the correct robot map based on the team
    QMap<int, RobotItem *> &robots = (selectedTeam == 0) ? blueRobots : yellowRobots;

    // Restore the previous robot's color if it's different
    static int lastSelectedRobotId = -1;
    static int lastSelectedTeam = -1;

    if (lastSelectedRobotId != -1 && robots.contains(lastSelectedRobotId)) {
        robots[lastSelectedRobotId]->setSelected(false);
    }

    // Update the currently selected robot
    if (robots.contains(selectedRobotId)) {
        RobotItem *robot = robots[selectedRobotId];
        robot->setSelected(true);  // ✅ Highlight selected robot

        // Convert position to field coordinates
        QVector2D pos = QVector2D(robot->x(), robot->y());
        float x = (pos.x() / 100) - 4.5;
        float y = -((pos.y() / 100) - 3.0);

        robotInfoLabel->setText(QString("Robot %1\nX: %2\nY: %3")
                                .arg(selectedRobotId)
                                .arg(x, 0, 'f', 2)
                                .arg(y, 0, 'f', 2));

        // Store last selected robot for future unhighlighting
        lastSelectedRobotId = selectedRobotId;
        lastSelectedTeam = selectedTeam;
    }
}

