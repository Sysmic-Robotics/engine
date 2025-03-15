-- robot.lua

Robot = {}

function Robot:new(id, team, x, y, vel_x, vel_y, orientation, active)
    local obj = {
        id = id or 0,
        team = team or 0,
        x = x or 0.0,
        y = y or 0.0,
        vel_x = vel_x or 0.0,
        vel_y = vel_y or 0.0,
        orientation = orientation or 0.0,
        active = active or false
    }
    setmetatable(obj, self)
    self.__index = self
    return obj
end

function Robot:print()
    print("Robot ID: " .. self.id)
    print("Team: " .. self.team)
    print("Position: x=" .. self.x .. ", y=" .. self.y)
    print("Velocity: x=" .. self.vel_x .. ", y=" .. self.vel_y)
    print("Orientation: " .. self.orientation)
    print("Active: " .. tostring(self.active))
end

return Robot
