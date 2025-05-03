local api = require("lua_api")

local M = {}

local is_done = false
local goal = nil
local id, team_id = nil, nil

function M.run_once(robot_id, team, target)
    if is_done then return end

    id = robot_id
    team_id = team
    goal = target

    api.dribbler(robot_id, team, 10)
    api.move_to(robot_id, team, target)

    -- Verificación se hace en cada frame
    local state = api.get_robot_state(robot_id, team)
    if not state.active then return end

    local dx = goal.x - state.x
    local dy = goal.y - state.y
    local dist = math.sqrt(dx * dx + dy * dy)
    if dist < 0.2 then
        is_done = true
        print("[Dribble] Objetivo alcanzado 🏁")
    end
end

function M.is_done()
    return is_done
end

function M.reset()
    is_done = false
    goal = nil
end

return M

