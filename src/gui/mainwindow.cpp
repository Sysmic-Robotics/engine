#include "mainwindow.hpp"
#include <QMouseEvent>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Setup main layout
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    setupLeftPanel();
    mainLayout->addWidget(leftPanel);

    // Initialize scene and view
    scene = new QGraphicsScene(0, 0, 900, 600, this);
    scene->setBackgroundBrush(Qt::lightGray);
    view = new QGraphicsView(scene, this);
    mainLayout->addWidget(view);

    setCentralWidget(centralWidget);

    // Draw field boundaries
    scene->addRect(0, 0, 900, 600, QPen(Qt::white));
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

    // Robot Info Panel
    robotInfoLabel = new QLabel("No robot selected");
    robotInfoLabel->setAlignment(Qt::AlignCenter);

    // Trace Toggle Checkbox
    showTraceCheckbox = new QCheckBox("Show Trace");
    connect(showTraceCheckbox, &QCheckBox::toggled, this, &MainWindow::onTraceCheckboxToggled);

    // Target Point Selection Button
    targetPointBtn = new QPushButton("Select Target Point");
    connect(targetPointBtn, &QPushButton::clicked, this, &MainWindow::onTargetPointButtonClicked);

    // Layout setup
    buttonLayout->addLayout(arrowLayout);
    buttonLayout->addWidget(robotInfoLabel);
    buttonLayout->addWidget(showTraceCheckbox);
    buttonLayout->addWidget(targetPointBtn);
    buttonLayout->addStretch();

    // Connect arrow buttons
    connect(leftArrowBtn, &QPushButton::clicked, [=]() { selectNextRobot(-1); });
    connect(rightArrowBtn, &QPushButton::clicked, [=]() { selectNextRobot(1); });

    leftPanel->setLayout(buttonLayout);
}



void MainWindow::updateRobot(int id, int team, QVector2D position, float orientation) {
    auto &robots = (team == 0) ? blueRobots : yellowRobots;
    QColor color = (team == 0) ? Qt::blue : Qt::yellow;

    if (!robots.contains(id)) {
        RobotItem *robot = new RobotItem(id, color);
        scene->addItem(robot);
        robots.insert(id, robot);
        robotIds.append(id);
    }

    robots[id]->setPosition(position);
    robots[id]->setOrientation(orientation);

    // ✅ Update trace only if it's the selected robot
    if (showTrace && id == selectedRobotId && team == 0) {
        updateRobotTrace(position);
    }
}


void MainWindow::updateRobotTrace(QVector2D position) {
    if (!showTrace || selectedRobotId == -1) return;

    // Convert field position to scene coordinates
    float x = (position.x() + 4.5) * 100;
    float y = (-position.y() + 3.0) * 100;
    QPointF newPoint(x, y);

    // ✅ If trace is uninitialized, create it
    if (!traceItem) {
        selectedRobotTrace = QPainterPath(newPoint);  // ✅ Start the path at the first position
        traceItem = new QGraphicsPathItem(selectedRobotTrace);  // ✅ Initialize the trace item with the path
        traceItem->setPen(QPen(Qt::red, 2));
        scene->addItem(traceItem);
    } else {
        selectedRobotTrace.lineTo(newPoint);  // ✅ Append new position to the path
        traceItem->setPath(selectedRobotTrace);  // ✅ Update path in the scene
    }
}

void MainWindow::clearSelectedRobotTrace() {
    if (traceItem) {
        scene->removeItem(traceItem);
        delete traceItem;
        traceItem = nullptr;
    }
    selectedRobotTrace = QPainterPath();  // Clear stored trace data
}


void MainWindow::onTraceCheckboxToggled(bool checked) {
    showTrace = checked;
    if (!showTrace) {
        clearSelectedRobotTrace();
    }
}



void MainWindow::selectNextRobot(int direction) {
    if (robotIds.isEmpty()) return;

    // Clear previous robot's trace
    clearSelectedRobotTrace();

    // Update selected robot index
    selectedRobotIndex = (selectedRobotIndex + direction + robotIds.size()) % robotIds.size();
    selectedRobotId = robotIds[selectedRobotIndex];
    selectedTeam = blueRobots.contains(selectedRobotId) ? 0 : 1;

    updateInfoPanel();
    emit robotSelected(selectedRobotId, selectedTeam);
}


void MainWindow::updateInfoPanel() {
    if (selectedRobotId == -1) {
        robotInfoLabel->setText("No robot selected");
        return;
    }

    auto &robots = (selectedTeam == 0) ? blueRobots : yellowRobots;

    static int lastSelectedRobotId = -1;
    if (lastSelectedRobotId != -1 && robots.contains(lastSelectedRobotId)) {
        robots[lastSelectedRobotId]->setSelected(false);
    }

    if (robots.contains(selectedRobotId)) {
        RobotItem *robot = robots[selectedRobotId];
        robot->setSelected(true);

        QVector2D pos(robot->x(), robot->y());
        float x = (pos.x() / 100) - 4.5;
        float y = -((pos.y() / 100) - 3.0);

        robotInfoLabel->setText(QString("Robot %1\nX: %2\nY: %3")
                                .arg(selectedRobotId)
                                .arg(x, 0, 'f', 2)
                                .arg(y, 0, 'f', 2));

        lastSelectedRobotId = selectedRobotId;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    int adjustedX = event->pos().x() - leftPanel->width();
    if (adjustedX < 0) return;

    QPointF scenePos = view->mapToScene(QPoint(adjustedX, event->pos().y()));

    if (waitingForTargetPoint) {
        QRectF fieldRect = scene->sceneRect();
        float x = ((scenePos.x() - fieldRect.left()) / fieldRect.width()) * 9.0 - 4.5;
        float y = -((scenePos.y() - fieldRect.top()) / fieldRect.height()) * 6.0 + 3.0;

        QVector2D targetPoint(x, y);
        drawTargetMarker(targetPoint);
        emit targetPointSelected(targetPoint);

        targetPointBtn->setText("Select Target Point");
        waitingForTargetPoint = false;
        return;
    }
}

void MainWindow::onTargetPointButtonClicked() {
    waitingForTargetPoint = true;
    targetPointBtn->setText("Click on Field...");
}

void MainWindow::drawTargetMarker(QVector2D point) {
    if (!targetMarker) {
        targetMarker = new TargetMarker();
        scene->addItem(targetMarker);
    }
    targetMarker->setPosition(point);
}

