#include "grsim.hpp"
#include <QByteArray>
#include <QHostAddress>
#include <QtMath>
#include <QDebug>

Grsim::Grsim() {
    sendSocket = new QUdpSocket();
}

Grsim::~Grsim() {
    delete sendSocket;
}

void Grsim::communicate_grsim(int id, int team, double velangular, double kickspeedx,
                              double kickspeedz, double velx, double vely,
                              int spinner, bool wheelsspeed) {
    grSim_Packet package;
    grSim_Robot_Command* command = package.mutable_commands()->add_robot_commands();
    
    command->set_id(id);
    command->set_velangular(velangular);
    command->set_kickspeedx(kickspeedx);
    command->set_kickspeedz(kickspeedz);
    command->set_veltangent(velx);
    command->set_velnormal(vely);
    command->set_spinner(spinner);
    command->set_wheelsspeed(wheelsspeed);
    
    package.mutable_commands()->set_timestamp(0);
    package.mutable_commands()->set_isteamyellow(team);
    
    QByteArray data;
    std::string serializedData = package.SerializeAsString();
    data.append(serializedData.c_str(), static_cast<int>(serializedData.size()));


    if (!data.isEmpty()) {
        sendSocket->writeDatagram(data, QHostAddress("127.0.0.1"), GRSIM_COMMAND_PORT);
    }
}

void Grsim::communicate_pos_robot(int id, int yellowteam, double x, double y, double dir) {
    dir = qRadiansToDegrees(dir);
    grSim_Packet package;
    grSim_RobotReplacement* command = package.mutable_replacement()->add_robots();
    
    command->set_id(id);
    command->set_x(x);
    command->set_y(y);
    command->set_dir(dir);
    command->set_yellowteam(yellowteam);
    
    QByteArray data;
    data.append(package.SerializeAsString().c_str(), package.ByteSizeLong());
    
    if (!data.isEmpty()) {
        sendSocket->writeDatagram(data, QHostAddress("127.0.0.1"), GRSIM_COMMAND_PORT);
    }
    qDebug()<<"utilizando funcion puta";
}

void Grsim::communicate_pos_ball(double x, double y) {
    grSim_Packet packet;
    grSim_BallReplacement* ball = packet.mutable_replacement()->mutable_ball();

    ball->set_x(x);
    ball->set_y(y);
    ball->set_vx(0);
    ball->set_vy(0);

    QByteArray data;
    data.append(packet.SerializeAsString().c_str(), packet.ByteSizeLong());

    if (!data.isEmpty()) {
        sendSocket->writeDatagram(data, QHostAddress("127.0.0.1"), GRSIM_COMMAND_PORT);
    }

    qDebug() << "[grSim] Ball teleported to:" << x << y;
}
