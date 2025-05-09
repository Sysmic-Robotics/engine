-- pass_and_move.lua (versión simplificada y funcional)
local api = require("sysmickit.lua_api")
local move = require("core.move")
local capture = require("skills.capture_ball")
local kick = require("skills.kick_to_point")
local utils = require("utils")

local M = {}

local is_captured = false
local is_kicked = false

function M.run_once(robot_id, team, target)
    local robot = api.get_robot_state(robot_id, team)
    local ball = api.get_ball_state()
    if not robot or not ball then return end
    if is_kicked then return end

    if not is_captured then
        -- Alinearse con la pelota antes de capturar
        local angle_to_ball = math.atan(ball.y - robot.y, ball.x - robot.x)
        local angle_error = utils.angle_diff(robot.orientation, angle_to_ball)

        if math.abs(angle_error) > 0.2 then
            api.face_to(robot_id, team, { x = ball.x, y = ball.y })
            return
        end

        -- Intentar capturar
        capture.run_once(robot_id, team, 10)

        if capture.is_done() then
            is_captured = true
            print("[PassAndMove] Pelota capturada ✅")
        end
    else
        -- Patear al objetivo
        kick.run_once(robot_id, team, target)

        if kick.is_done() then
            is_kicked = true
            print("[PassAndMove] Pase completado ✅")
        end
    end
end

function M.is_done()
    return is_kicked
end

function M.reset()
    is_captured = false
    is_kicked = false
    capture.reset()
    kick.reset()
end

return M
