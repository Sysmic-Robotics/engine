print("Lua package path: " .. package.path)
print("Script initialized")

Robot = require("robot")  -- Import the Robot struct


function distance(x1, y1, x2, y2)
    return math.sqrt((x2 - x1)^2 + (y2 - y1)^2)
end

point = {x = 0, y = 0}
function process()

    move_to(1 , 0, point)
    
end
