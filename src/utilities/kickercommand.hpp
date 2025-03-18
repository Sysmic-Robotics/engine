#ifndef KICKER_COMMAND_HPP
#define KICKER_COMMAND_HPP

#include <QVector2D>
#include <QString>

class KickerCommand {
public:
    KickerCommand(int id, int team);

    // Getters
    int getId() const;
    int getTeam() const;
    bool getKickX() const;
    bool getKickZ() const;
    double getDribbler() const;

    // Setters
    void setKickX(bool flag);
    void setKickZ(bool flag);
    void setDribbler(double speed);


private:
    bool kicker_x = false;
    bool kicker_z = false;
    double dribbler_speed = 0.0;
    int team;
    int id;
};

#endif // KICKER_COMMAND_HPP
