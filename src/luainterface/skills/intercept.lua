local api = require("lua_api")
local M = {}

local is_done = false

function M.run_once(robot_id, team)
    local ball = api.get_ball_state()
    local state = api.get_robot_state(robot_id, team)
    if not state.active then return end

    -- Apunta al balón antes de moverse
    api.face_to(robot_id, team, ball)

    -- Avanza hacia la pelota mientras gira hacia ella
    api.move_to(robot_id, team, { x = ball.x, y = ball.y })

    -- Activa el dribbler
    api.dribbler(robot_id, team, 10)

    -- Verifica si está suficientemente cerca y alineado
    local dx = ball.x - state.x
    local dy = ball.y - state.y
    local distance = math.sqrt(dx * dx + dy * dy)
    is_done = distance < 0.3
end

function M.is_done()
    return is_done
end

function M.reset()
    is_done = false
end

return M























--[[ Implementacion 1
local api = require("lua_api")
local utils = require("utils")

local M = {}

local is_done = false

function M.run_once(robot_id, team)
    local robot = api.get_robot_state(robot_id, team)
    local ball = api.get_ball_state()
    if not robot.active then return end

    -- Estimación básica: interceptar en la posición actual de la pelota
    local target = { x = ball.x, y = ball.y }

    api.move_to(robot_id, team, target)

    -- Consideramos que interceptó si está suficientemente cerca de la pelota
    local dx = target.x - robot.x
    local dy = target.y - robot.y
    local dist = math.sqrt(dx * dx + dy * dy)

    is_done = dist < 0.2 -- puedes ajustar la tolerancia
end

function M.is_done()
    return is_done
end

function M.reset()
    is_done = false
end

return M
]]