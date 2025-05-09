local api = require("lua_api")
local move = require("core.move")
local utils = require("utils")

local M = {}

local done = false
local threshold = 0.1

function M.run_once(robot_id, team)
    local ball = api.get_ball_state()
    local robot = api.get_robot_state(robot_id, team)
    if not ball or not robot then return end

    -- Posición del arco propio (ajústala si juegas como azul o amarillo)
    if team == 0 then
        goal_x = -4.5  -- defensa lado derecho
    else
        goal_x = 4.5  -- defensa lado izquierdo
    end

    -- Calcula punto intermedio entre balón y arco propio
    local defend_x = (ball.x + goal_x) / 2
    local defend_y = ball.y

    local target = { x = defend_x, y = defend_y }

    -- Mover hacia ese punto
    move.move_to(robot_id, team, target)
    api.face_to(robot_id, team, { x = ball.x, y = ball.y })

    -- Verificar si ya llegó
    if utils.distance(robot, target) < threshold then
        done = true
    end
end

function M.is_done()
    return done
end

function M.reset()
    done = false
end

return M
