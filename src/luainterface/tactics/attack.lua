-- tactics/attack.lua
local api = require("lua_api")
local intercept = require("intercept")
local capture = require("capture_ball")
local dribble = require("dribble")
local kick = require("kick_to_point")
local M = {}

local state = "intercept"
local done = false

function M.run_once(robot_id, team, target)
    local ball = api.get_ball_state()
    if not ball then return end

    if state == "intercept" then
        intercept.run_once(robot_id, team)
        if intercept.is_done() then
            state = "capture"
            intercept.reset()
        end

    elseif state == "capture" then
        capture.run_once(robot_id, team, 10)
        if capture.is_done() then
            state = "dribble"
            capture.reset()
        end

    elseif state == "dribble" then
        dribble.run_once(robot_id, team, target)
        local robot = api.get_robot_state(robot_id, team)
        if robot and math.abs(robot.x - target.x) < 0.2 and math.abs(robot.y - target.y) < 0.2 then
            state = "kick"
            dribble.reset()
        end

    elseif state == "kick" then
        kick.run_once(robot_id, team, target)
        if kick.is_done() then
            state = "done"
            done = true
            kick.reset()
        end
    end
end

function M.is_done()
    return done
end

function M.reset()
    state = "intercept"
    done = false
    intercept.reset()
    capture.reset()
    dribble.reset()
    kick.reset()
end

return M
