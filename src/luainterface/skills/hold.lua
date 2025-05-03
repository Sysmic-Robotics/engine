local api = require("lua_api")
local M = {}

local is_done = false

function M.run_once(robot_id, team)
    local ball = api.get_ball_state()
    local state = api.get_robot_state(robot_id, team)
    if not state.active then return end

    -- No se mueve: se queda en su lugar
    -- Solo apunta hacia la pelota
    api.face_to(robot_id, team, ball)
    
    -- Mantiene el dribbler activo
    api.dribbler(robot_id, team, 10)

    -- Consideramos que siempre está "hecho"
    is_done = true
end

function M.is_done()
    return is_done
end

function M.reset()
    is_done = false
end

return M
