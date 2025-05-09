-- move.lua
local api = require("sysmickit.lua_api")
local M = {}

--- Moves the specified robot to a given target point.
--- @param robotId number The ID of the robot.
--- @param team number The team identifier.
--- @param target table A table containing target coordinates with keys `x` and `y`.
function M.move_to(robotId, team, target)
    api.move_to(robotId, team, target)
end

return M
