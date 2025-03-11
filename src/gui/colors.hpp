#ifndef COLORS_HPP
#define COLORS_HPP

#include <QColor>

namespace GUIColors {
    // ✅ Field Colors
    const QColor FIELD_BACKGROUND = QColor(180,180,180);  // Dark gray field background

    // ✅ Robot Colors
    const QColor BLUE_ROBOT = Qt::blue;  // Bright blue for blue team
    const QColor YELLOW_ROBOT = QColor(255, 204, 0);  // Bright yellow for yellow team
    const QColor ORIENTATION_COLOR = QColor(246, 248, 213);
    const QColor BALL = QColor(255,165,0);  // Bright blue for blue team

    // ✅ Target Marker
    const QColor TARGET_MARKER = QColor(0, 255, 0);  // Green target point

    // ✅ Trace Colors
    const QColor TRACE_COLOR = QColor(255, 0, 0);  // Red trace

    // ✅ UI Elements
    const QColor BUTTON_BACKGROUND = QColor(75, 75, 75);  // Dark gray for buttons
    const QColor BUTTON_TEXT = QColor(255, 255, 255);  // White text
}

#endif // COLORS_HPP
