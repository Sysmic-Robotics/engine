#ifndef GRSIM_HPP
#define GRSIM_HPP

#include <QUdpSocket>
#include "grSim_Packet.pb.h"

class Grsim {
public:
    Grsim();
    ~Grsim();

    void communicate_grsim(int id, int team, double velangular = 0, double kickspeedx = 0,
                           double kickspeedz = 0, double velx = 0, double vely = 0,
                           int spinner = 0, bool wheelsspeed = false);

    void communicate_pos_robot(int id = 0, int yellowteam = 0,
                               double x = 0, double y = 0, double dir = 0);

private:
    QUdpSocket* sendSocket;
    static constexpr int GRSIM_COMMAND_PORT = 20011;
};

#endif // GRSIM_HPP
