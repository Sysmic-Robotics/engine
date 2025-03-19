#include "mainwindow.hpp"
#include "colors.hpp"  // ✅ Include the color definitions
#include <QMouseEvent>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Setup main layout
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    setupLeftPanel();
    mainLayout->addWidget(leftPanel);

    // Initialize scene and view
    scene = new QGraphicsScene(0, 0, 900, 600, this);
    scene->setBackgroundBrush(GUIColors::FIELD_BACKGROUND);
    view = new QGraphicsView(scene, this);
    mainLayout->addWidget(view);

    setCentralWidget(centralWidget);
    ball = new BallItem();

    scene->addItem(ball);

}


void MainWindow::setupLeftPanel() {
    leftPanel = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(leftPanel);

    // **Select Robot Panel (Above Tabs)**
    QWidget *selectRobotPanel = new QWidget();
    QVBoxLayout *selectRobotLayout = new QVBoxLayout(selectRobotPanel);

    QLabel *selectRobotLabel = new QLabel("🤖 Robot Info");
    selectRobotLabel->setAlignment(Qt::AlignCenter);
    selectRobotLabel->setStyleSheet("font-weight: bold;");

    QHBoxLayout *arrowLayout = new QHBoxLayout();
    leftArrowBtn = new QPushButton("⬅️");
    rightArrowBtn = new QPushButton("➡️");
    arrowLayout->addWidget(leftArrowBtn);
    arrowLayout->addWidget(rightArrowBtn);

    robotInfoLabel = new QLabel(QString("Robot ID: No selected\nX: 0\nY: 0\n speed: 0 [m/s]") );
    robotInfoLabel->setAlignment(Qt::AlignCenter);
    
    // Trace Toggle Checkbox
    showTraceCheckbox = new QCheckBox("Show Trace");
    connect(showTraceCheckbox, &QCheckBox::toggled, this, &MainWindow::onTraceCheckboxToggled);
    selectRobotLayout->addWidget(selectRobotLabel);
    selectRobotLayout->addLayout(arrowLayout);
    selectRobotLayout->addWidget(robotInfoLabel);
    selectRobotLayout->addWidget(showTraceCheckbox);
    selectRobotPanel->setLayout(selectRobotLayout);

    // **Tab Widget for Controls**
    QTabWidget *tabWidget = new QTabWidget(leftPanel);

    // **Tab 1: Robot Control**
    QWidget *robotControlTab = new QWidget();
    QVBoxLayout *robotLayout = new QVBoxLayout(robotControlTab);

    QLabel *robotControlLabel = new QLabel("🔹 Robot Control");
    robotControlLabel->setAlignment(Qt::AlignCenter);
    robotControlLabel->setStyleSheet("font-weight: bold;");

    targetPointBtn = new QPushButton("🎯 Select Target");
    connect(targetPointBtn, &QPushButton::clicked, this, &MainWindow::onTargetPointButtonClicked);

    faceToBtn = new QPushButton("🎯 Point to Aim");
    connect(faceToBtn, &QPushButton::clicked, this, &MainWindow::onFaceToPointButtonClicked);

    robotLayout->addWidget(robotControlLabel);
    robotLayout->addWidget(targetPointBtn);
    robotLayout->addWidget(faceToBtn);
    robotLayout->addStretch();
    robotControlTab->setLayout(robotLayout);

    // **Tab 2: Script Run**
    QWidget *scriptRunTab = new QWidget();
    QVBoxLayout *scriptLayout = new QVBoxLayout(scriptRunTab);

    QLabel *scriptControlLabel = new QLabel("📜 Script Control");
    scriptControlLabel->setAlignment(Qt::AlignCenter);
    scriptControlLabel->setStyleSheet("font-weight: bold;");

    loadScriptBtn = new QPushButton("📂 Load Script");
    loadScriptBtn->setEnabled(false);
    connect(loadScriptBtn, &QPushButton::clicked, this, &MainWindow::onLoadScriptClicked);

    runScriptBtn = new QPushButton("▶️ Run Script");
    connect(runScriptBtn, &QPushButton::clicked, this, &MainWindow::onRunScriptClicked);

    scriptLayout->addWidget(scriptControlLabel);
    scriptLayout->addWidget(loadScriptBtn);
    scriptLayout->addWidget(runScriptBtn);
    scriptLayout->addStretch();
    scriptRunTab->setLayout(scriptLayout);

    // **Add tabs to QTabWidget**
    tabWidget->addTab(robotControlTab, "🤖 Robot");
    tabWidget->addTab(scriptRunTab, "📜 Script");
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    // **Final Layout**
    mainLayout->addWidget(selectRobotPanel); // Add Select Robot Panel ABOVE tabs
    mainLayout->addWidget(tabWidget);
    leftPanel->setLayout(mainLayout);

    // **Connect Robot Selection Buttons**
    connect(leftArrowBtn, &QPushButton::clicked, [=]() { selectNextRobot(-1); });
    connect(rightArrowBtn, &QPushButton::clicked, [=]() { selectNextRobot(1); });
}

void MainWindow::onTabChanged(int index) {
   if (index == 1) {
        drawTargetMarker(QVector2D(-20,-20)); // Hide target
        emit setRobotControl(false);
    }else{
        emit setRobotControl(true);
    }
}


void MainWindow::onLoadScriptClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select Lua Script", "", "Lua Files (*.lua)");
    if (!filePath.isEmpty()) {
        emit scriptLoaded(filePath);  // Send signal with selected script path
    }
}

void MainWindow::onRunScriptClicked() {
    emit scriptRunRequested();  // Send signal to execute script
}

void MainWindow::updateBall(const BallState &ballState) {
    QVector2D position = ballState.getPosition();
    ball->setPosition(position);
}

void MainWindow::updateRobot(const RobotState &robotState) {
    int id = robotState.getId();
    int team = robotState.getTeam();
    QVector2D position = robotState.getPosition();
    float orientation = robotState.getOrientation();

    auto &robots = (team == 0) ? blueRobots : yellowRobots;
    QColor color = (team == 0) ? GUIColors::BLUE_ROBOT : GUIColors::YELLOW_ROBOT;

    if (!robots.contains(id)) {
        RobotItem *robot = new RobotItem(id, color);
        scene->addItem(robot);
        robots.insert(id, robot);
        robotIds.append(id);
    }

    robots[id]->setPosition(position);
    robots[id]->setOrientation(orientation);

    if (id == selectedRobotId && selectedTeam == team) {
        selectedRobotState.setPosition(position);  // ✅ Explicit update
        selectedRobotState.setVelocity(robotState.getVelocity());  // ✅ Explicit update
        selectedRobotState.setOrientation(orientation);  // ✅ Explicit update

        updateInfoPanel();  // ✅ Ensure UI reflects the latest data
    }

    if (showTrace && id == selectedRobotId && selectedTeam == team) {
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

    // ✅ Get scene coordinates
    QVector2D scenePos = (selectedTeam == 0) 
                             ? blueRobots[selectedRobotId]->getPosition()
                             : yellowRobots[selectedRobotId]->getPosition();

    // ✅ Convert to field coordinates
    QVector2D fieldPos(
        (scenePos.x() / 100) - 4.5,   // Convert X from scene to field
        -((scenePos.y() / 100) - 3.0) // Convert Y from scene to field
    );
    drawTargetMarker(QVector2D(-20,-20)); // Hide target
    emit targetPointSelected(fieldPos);
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

        float x = selectedRobotState.getPosition().x(); 
        float y = selectedRobotState.getPosition().y();
        QVector2D velocity = selectedRobotState.getVelocity();
        float velocityNorm = velocity.length();  // Compute magnitude

        robotInfoLabel->setText(QString("Robot %1\nX: %2\nY: %3\n speed: %4 [m/s]")
                                .arg(selectedRobotId)
                                .arg(x, 0, 'f', 2)
                                .arg(y, 0, 'f', 2)
                                .arg(velocityNorm, 0, 'f', 2));
        
        lastSelectedRobotId = selectedRobotId;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    int adjustedX = event->pos().x() - leftPanel->width();
    if (adjustedX < 0) return;

    QPointF scenePos = view->mapToScene(QPoint(adjustedX, event->pos().y()));

    if (waitingForTargetPoint) {
        float x = ((scenePos.x() - scene->sceneRect().left()) / scene->sceneRect().width()) * 9.0 - 4.5;
        float y = -((scenePos.y() - scene->sceneRect().top()) / scene->sceneRect().height()) * 6.0 + 3.0;

        QVector2D targetPoint(x, y);
        drawTargetMarker(targetPoint);
        emit targetPointSelected(targetPoint);

        targetPointBtn->setText("Select Target Point");
        waitingForTargetPoint = false;
        faceToBtn->setEnabled(true); // Re-enable faceToBtn

        return;
    }

    if (waitingForFaceTo) {
        float x = ((scenePos.x() - scene->sceneRect().left()) / scene->sceneRect().width()) * 9.0 - 4.5;
        float y = -((scenePos.y() - scene->sceneRect().top()) / scene->sceneRect().height()) * 6.0 + 3.0;

        QVector2D faceToTarget(x, y);
        drawTargetMarker(faceToTarget);
        emit faceToDebug(faceToTarget);

        faceToBtn->setText("Select point to aim");
        waitingForFaceTo = false;
        targetPointBtn->setEnabled(true); // Re-enable targetPointBtn
    }
}

void MainWindow::onTargetPointButtonClicked() {
    if (waitingForFaceTo) return;  // Prevent conflict

    waitingForTargetPoint = true;
    waitingForFaceTo = false;
    
    targetPointBtn->setText("Click on Field...");
    faceToBtn->setEnabled(false); // Disable faceToBtn while selecting target
}

void MainWindow::onFaceToPointButtonClicked() {
    if (waitingForTargetPoint) return;  // Prevent conflict

    waitingForFaceTo = true;
    waitingForTargetPoint = false;

    faceToBtn->setText("Click on Field...");
    targetPointBtn->setEnabled(false); // Disable targetPointBtn while debugging face_to
}

void MainWindow::drawTargetMarker(QVector2D point) {
    if (!targetMarker) {
        targetMarker = new TargetMarker();
        scene->addItem(targetMarker);
    }
    targetMarker->setPosition(point);
}

