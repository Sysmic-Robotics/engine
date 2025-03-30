-- script.lua
print("Script initialized")
package.path = "C:/Robocup/CondorSSL/src/luainterface/?.lua;" .. package.path

local move = require("move")
local capture = require("capture_ball")
local kick = require("kick_to_point")

robotId = 1
team = 0

--- Main process function.
--- You can switch between skills or chain them as needed.
function process()
    -- Example: Use the kick_to_point skill to capture the ball and kick it toward (100, 200)
    --move.move_to(0,0,{x=0,y=0})
    -- capture.process(robotId, team, 10)
    kick.kick_to_point(robotId, team, {x= 0, y = 0})
   -- kick.kick_to_point(robotId, team, { x = 100, y = 200 })
    
    -- Alternatively, you could call the capture_ball or move skill individually:
    -- capture.capture_ball(robotId, team)
    -- move.move_to(robotId, team, { x = 50, y = 75 })
end
