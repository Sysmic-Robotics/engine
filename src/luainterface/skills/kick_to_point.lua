-- kick_to_point.lua
local api = require("sysmickit.lua_api")
local utils = require("utils")
local M = {}

local kick_distance = 0.12
local angle_tolerance = 0.025  -- tolerancia de 3 grados (0.052 radianes)
local done = false

--- Ejecuta un disparo hacia un punto, si está en condiciones
function M.run_once(robotId, team, target)
    if done then return end  -- ya disparó, no hacer nada

    local robot = api.get_robot_state(robotId, team)
    local ball = api.get_ball_state()
    if not robot or not ball then return end

    local dist = utils.distance(robot, ball)
    if dist > kick_distance then
        print("[Kick] Muy lejos del balón. No se patea.")
        return
    end

    local angle_to_target = math.atan(target.y - robot.y, target.x - robot.x)
    local angle_error = utils.angle_diff(robot.orientation, angle_to_target)

    if angle_error > angle_tolerance then
        api.face_to(robotId, team, target)
        print(string.format("[Kick] Alineando... error: %.2f rad", angle_error))
        return
    end

    api.kickx(robotId, team)
    done = true
    print("[Kick] ⚽ Disparo ejecutado.")
end

--- ¿Ya disparó?
function M.is_done()
    return done
end

--- Permite reutilizar la skill
function M.reset()
    done = false
    print("[Kick] Reiniciado.")
end

return M




