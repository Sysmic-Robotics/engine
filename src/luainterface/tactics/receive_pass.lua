local api = require("sysmickit.lua_api")
local move = require("core.move")
local capture = require("skills.capture_ball")
local hold = require("skills.hold")
local utils = require("utils")

local M = {}

local state = "move"
local done = false

local angle_tolerance = 0.3
local activation_distance = 0.2

function M.run_once(robotId, team, target)
    local robot = api.get_robot_state(robotId, team)
    local ball = api.get_ball_state()
    if not robot or not ball then return end

    if state == "move" then
        if utils.distance(robot, target) > 0.03 then
            move.move_to(robotId, team, target)
        end

        local angle_to_ball = math.atan(ball.y - robot.y, ball.x - robot.x)
        local angle_error = utils.angle_diff(robot.orientation, angle_to_ball)
        if angle_error > angle_tolerance then
            api.face_to(robotId, team, { x = ball.x, y = ball.y })
        end

        if utils.distance(robot, target) <= 0.03 then
            state = "wait"
            print("[ReceivePass] En posición. Esperando balón...")
        end

    elseif state == "wait" then
        local angle_to_ball = math.atan(ball.y - robot.y, ball.x - robot.x)
        local angle_error = utils.angle_diff(robot.orientation, angle_to_ball)

        -- Alinear y activar dribbler mientras espera
        api.face_to(robotId, team, { x = ball.x, y = ball.y })
        api.dribbler(robotId, team, 10)

        if utils.distance(robot, ball) < activation_distance then
            state = "capture"
            print("[ReceivePass] Balón cerca. Activando captura...")
        end

    elseif state == "capture" then
        capture.run_once(robotId, team, 10)

        if capture.is_done() then
            state = "hold"
            print("[ReceivePass] Balón capturado. Activando hold...")
        end

    elseif state == "hold" then
        hold.run_once(robotId, team)
        done = true
        state = "done"
        print("[ReceivePass] En control del balón 🧤")

    elseif state == "done" then
        -- Esperando futuras acciones
    end
end

function M.is_done()
    return done
end

function M.reset()
    state = "move"
    done = false
    capture.reset()
    hold.reset()
    print("[ReceivePass] Reiniciado.")
end

return M


