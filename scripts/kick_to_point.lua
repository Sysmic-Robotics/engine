-- kick_to_point.lua
local api = require("sysmickit.lua_api")
local capture = require("capture_ball")
local M = {}

-- Helper function: compute the minimal absolute difference between two angles (in radians)
local function angle_diff(a, b)
    local diff = a - b
    while diff > math.pi do diff = diff - 2 * math.pi end
    while diff < -math.pi do diff = diff + 2 * math.pi end
    return math.abs(diff)
end

--- Executes a kick-to-point skill:
--- 1. Captures the ball (moves to it).
--- 2. Commands the robot to face the target.
--- 3. Kicks the ball when the robot is facing the target.
---
--- @param robotId number The ID of the robot.
--- @param team number The team identifier.
--- @param target table A table containing target coordinates with keys `x` and `y`.
function M.kick_to_point(robotId, team, target)
    -- Capture the ball first.
    capture.process(robotId, team, 10)
    
    -- Command the robot to face the target point.
    api.face_to(robotId, team, target)
    
    -- Get the current robot state.
    local robot = api.get_robot_state(robotId, team)
    if robot then
        -- Compute the angle from the robot to the target.
        local angle_to_target = math.atan(target.y - robot.y, target.x - robot.x)
        -- Only kick if the robot's orientation is within tolerance of the target direction.
        if angle_diff(robot.orientation, angle_to_target) < 0.1 then
            api.kickx(robotId, team)
        end
    end
end

return M
