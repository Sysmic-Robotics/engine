print("Ciclo simple de pases entre 3 robots (corregido y fluido) 👑⚽")

package.path = "C:/CondorSSL/src/luainterface/?.lua;" .. package.path

local api = require("sysmickit.lua_api")
local utils = require("utils")
local capture = require("capture_ball")
local kick = require("kick_to_point")
local receive = require("receive_pass")

local team = 0

local robot1 = 0
local robot2 = 1
local robot3 = 2

local turn = 1
local delay = 0
local delay_frames = 60
local state = "play"

local positions = {
    [robot1] = { x = 0.0, y = -2.0 },
    [robot2] = { x = 2.0, y = 0.0 },
    [robot3] = { x = -2.0, y = 1.5 }
}

capture.reset()
kick.reset()
receive.reset()

function process()
    if delay > 0 then
        delay = delay - 1
        return
    end

    local passer, receiver
    if turn == 1 then
        passer = robot1
        receiver = robot2
    elseif turn == 2 then
        passer = robot2
        receiver = robot3
    elseif turn == 3 then
        passer = robot3
        receiver = robot1
    end

    -- TODOS excepto el pasador actual se posicionan como receptores
    for _, id in ipairs({robot1, robot2, robot3}) do
        if id ~= passer then
            receive.run_once(id, team, positions[id])
        end
    end

    if state == "play" then
        capture.run_once(passer, team, 10)
        kick.run_once(passer, team, positions[receiver])

        if kick.is_done() then
            print("[Turn " .. turn .. "] Pase ejecutado ✅")
            state = "wait"
            delay = delay_frames
        end

    elseif state == "wait" then
        if receive.is_done() then
            -- Siguiente ciclo
            turn = turn % 3 + 1
            state = "play"
            delay = delay_frames
            capture.reset()
            kick.reset()
            receive.reset()
            print("[Turn " .. turn .. "] Nuevo turno iniciado 👟")
        end
    end
end

