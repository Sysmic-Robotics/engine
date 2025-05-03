-- capture_ball.lua
local api = require("sysmickit.lua_api")
local move = require("move")
local utils = require("utils")
local M = {}

-- FSM
local state = "approach"
local done = false

-- Parámetros
local safe_distance = 0.12
local threshold = 0.1
local angle_tolerance = 0.02
local approach_target = { x = 0, y = 0 }

--- Ejecuta un paso de la FSM
function M.run_once(robotId, team, dribbleSpeed)
    dribbleSpeed = dribbleSpeed or 10

    local ball = api.get_ball_state()
    local robot = api.get_robot_state(robotId, team)
    if not ball or not robot then
        print("[CaptureBall] Error: Falta estado de balón o robot.")
        return
    end

    if state == "approach" then
        local dx = robot.x - ball.x
        local dy = robot.y - ball.y
        local d = math.sqrt(dx * dx + dy * dy)
        if d == 0 then dx, dy, d = 1, 0, 1 end

        approach_target.x = ball.x + (dx / d) * safe_distance
        approach_target.y = ball.y + (dy / d) * safe_distance

        local dist_to_target = utils.distance(robot, approach_target)
        local angle_to_ball = math.atan(ball.y - robot.y, ball.x - robot.x)
        local angle_error = utils.angle_diff(robot.orientation, angle_to_ball)

        if dist_to_target > threshold then
            move.move_to(robotId, team, approach_target)
        end

        if angle_error > angle_tolerance then
            api.face_to(robotId, team, { x = ball.x, y = ball.y })
        end

        if dist_to_target < threshold and angle_error < angle_tolerance then
            state = "capture"
            print("[CaptureBall] → Estado: capture")
        end

    elseif state == "capture" then
        api.dribbler(robotId, team, dribbleSpeed)
        api.move_to(robotId, team, { x = ball.x, y = ball.y })

        if utils.distance(robot, ball) <= safe_distance then
            state = "idle"
            done = true
            print("[CaptureBall] → Estado: idle (completado)")
        end

    elseif state == "idle" then
        local dist = utils.distance(robot, ball)
        if dist > safe_distance then
            print("[CaptureBall] ⚠️ Pelota se escapó. Reintentando captura.")
            state = "capture"
        else
            api.dribbler(robotId, team, dribbleSpeed)
        end
    
end
end

--- ¿Ya terminó su trabajo?
function M.is_done()
    return done
end

--- Reiniciar FSM
function M.reset()
    state = "approach"
    done = false
    print("[CaptureBall] Reiniciado")
end

return M

