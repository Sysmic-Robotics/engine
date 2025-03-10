#ifndef SKILL_H
#define SKILL_H

#include <QObject>

class Skill : public QObject {
    Q_OBJECT  // ✅ Required for Qt meta-object system

public:
    explicit Skill(QObject *parent = nullptr) : QObject(parent) {}

    virtual ~Skill() override = default;  // ✅ Use defaulted destructor (No need for skill.cpp)
};

#endif // SKILL_H
