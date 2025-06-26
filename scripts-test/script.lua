print("Script initialized")
package.path = "C:/Robocup/CondorSSL/scripts/luainterface/?.lua;" .. package.path

local move = require("move")
local capture = require("capture_ball")
local kick = require("kick_to_point")
local api = require("sysmickit.lua_api")
local move = require("motion")

robotId = 0
team = 0



function process()
  api.motion()
end
