-- script.lua
print("Script initialized")
package.path = "C:/Robocup/CondorSSL/scripts/luainterface/?.lua;" .. package.path

local move = require("move")
local capture = require("capture_ball")
local kick = require("kick_to_point")

robotId = 1
team = 0


--- Main process function.
--- You can switch between skills or chain them as needed.
function process()
    kick.kick_to_point(robotId, team, {x= 0, y = 0})
end
