print("Lua package path: " .. package.path)
print("Script initialized")

Robot = require("robot")  -- Import the Robot struct


function distance(x1, y1, x2, y2)
    return math.sqrt((x2 - x1)^2 + (y2 - y1)^2)
end

point = {x = 0, y = 0}
function process()
    local robot = get_robot_state(1, 0)  -- Get robot state (ID=1, Team=0)
    if distance(0,0, robot.x, robot.y) < 0.01 then
        point = {x = 1, y = 0 }
    else if distance(1,0, robot.x, robot.y) < 0.01 then
        point = {x = 0, y = 0}
    end
    move_to(1 , 0, point)
    
end
