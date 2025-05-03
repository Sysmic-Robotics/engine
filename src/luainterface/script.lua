print("Prueba de receive_pass con hold 🧲")

package.path = "C:/CondorSSL/src/luainterface/?.lua;" ..
               "C:/CondorSSL/src/luainterface/core/?.lua;" ..
               "C:/CondorSSL/src/luainterface/skills/?.lua;" ..
               "C:/CondorSSL/src/luainterface/tactics/?.lua;" ..
               "C:/CondorSSL/src/luainterface/sysmickit/?.lua;" ..
               "C:/CondorSSL/src/luainterface/plays/?.lua;" .. package.path

local api = require("lua_api")
local kick = require("kick_to_point")
local receive = require("receive_pass")

local team = 0
local robot_kicker = 0
local robot_receiver = 1

local simple_cycle = require("simple_cycle")

local sent = false
local position_receiver = { x = 1.0, y = 0.0 }
local position_target = { x = 1.0, y = 0.0 } -- Donde apuntará el pase

receive.reset()
kick.reset()

function process()
    simple_cycle.execute()
end



--[[
local simple_cycle = require("simple_cycle")

function process()
    simple_cycle.execute()
end

simple_cycle.init(0)  -- team = 0

]]