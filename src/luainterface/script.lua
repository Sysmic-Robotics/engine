print("Prueba de la táctica: defend.lua")

package.path = "C:/CondorSSL/src/luainterface/?.lua;" ..
               "C:/CondorSSL/src/luainterface/core/?.lua;" ..
               "C:/CondorSSL/src/luainterface/skills/?.lua;" ..
               "C:/CondorSSL/src/luainterface/tactics/?.lua;" ..
               "C:/CondorSSL/src/luainterface/sysmickit/?.lua;" ..
               "C:/CondorSSL/src/luainterface/plays/?.lua;" .. package.path

local defend = require("defend")
local attack = require("attack")
local robot_id = 1
local team = 0
local goal = { x = 4.5, y = 0 }
local simple_cycle = require("simple_cycle")
defend.reset()

function process()
    --attack.run_once(robot_id, team, goal)
    --defend.run_once(3, 1)
    simple_cycle.execute()
end





--[[
local simple_cycle = require("simple_cycle")

function process()
    simple_cycle.execute()
end

simple_cycle.init(0)  -- team = 0

]]