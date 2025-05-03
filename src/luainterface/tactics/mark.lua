-- mark.lua
local api = require("sysmickit.lua_api")
local move = require("move")
local M = {}


--- Mark an opponent more intelligently by staying between them and our goal,
-- slightly closer to the opponent and facing them.
-- @param robotId number Our robot's ID
-- @param team number Our team ID (0 or 1)
-- @param opponentId number The ID of the opponent to mark
-- @param alpha number Optional balance factor (0 = on goal, 1 = on opponent). Default 0.7
function M.mark(robotId, team, opponentId, alpha)
    alpha = alpha or 0.7  -- closer to opponent by default

    local opponent = api.get_robot_state(opponentId, 1 - team)
    if not opponent or not opponent.active then return end
    

    -- Goal position based on team
    local goal = { x = -4.5, y = 0 }
    if team == 1 then goal.x = 4.5 end

    -- Weighted position: closer to the opponent
    local target = {
        x = goal.x * (1 - alpha) + opponent.x * alpha,
        y = goal.y * (1 - alpha) + opponent.y * alpha
    }

    -- Move and face the opponent
    move.move_to(robotId, team, target)
    api.face_to(robotId, team, { x = opponent.x, y = opponent.y })
end

return M
