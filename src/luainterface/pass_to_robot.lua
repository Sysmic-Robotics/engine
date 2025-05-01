-- pass_to_robot.lua
local api = require("sysmickit.lua_api")
local kick = require("kick_to_point")
local M = {}

function M.pass_to_robot(passerId, team, receiverId)
    local receiver = api.get_robot_state(receiverId, team)
    local target = { x = receiver.x, y = receiver.y }
    kick.run_once(passerId, team, target)
end

function M.reset()
    kick.reset()
end

function M.is_done()
    return kick.is_done()
end

return M


