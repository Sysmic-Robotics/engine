-- capture_ball.lua
local api = require("sysmickit.lua_api")
local move = require("move")
local M = {}

-- Define states: "approach", "capture", "idle"
local state = "approach"
local safe_distance = 0.12  -- desired gap from the ball (in meters)
local threshold = 0.05      -- distance threshold to consider the target reached
local angle_tolerance = 0.1 -- radians tolerance to consider the robot is facing the ball
-- The computed approach target (a table with x and y)
local approach_target = { x = 0, y = 0 }

-- Utility function: Euclidean distance between two points
local function distance(a, b)
    return math.sqrt((a.x - b.x)^2 + (a.y - b.y)^2)
end

-- Utility function: compute the minimal difference between two angles (in radians)
local function angle_diff(a, b)
    local diff = a - b
    while diff > math.pi do diff = diff - 2 * math.pi end
    while diff < -math.pi do diff = diff + 2 * math.pi end
    return math.abs(diff)
end

--- Process the capture ball state machine.
--- @param robotId number The ID of the robot.
--- @param team number The team identifier.
--- @param dribbleSpeed number Optional dribbler speed (0–10); default is 10.
function M.process(robotId, team, dribbleSpeed)
    dribbleSpeed = dribbleSpeed or 10

    local ball = api.get_ball_state()
    local robot = api.get_robot_state(robotId, team)
    if not ball or not robot then
        print("[CaptureBall] Error: Missing ball or robot state.")
        return
    end

    if state == "approach" then
        -- Compute an approach target that stops at a safe distance from the ball.
        local dx = robot.x - ball.x
        local dy = robot.y - ball.y
        local d = math.sqrt(dx * dx + dy * dy)
        if d == 0 then
            -- If the robot is exactly at the ball's position, choose a default direction.
            dx, dy, d = 1, 0, 1
        end
        approach_target.x = ball.x + (dx / d) * safe_distance
        approach_target.y = ball.y + (dy / d) * safe_distance

        -- Command the robot to move toward the approach target and face the ball.
        move.move_to(robotId, team, approach_target)
        api.face_to(robotId, team, { x = ball.x, y = ball.y })

        -- Check if the robot is near the target and facing the ball.
        local dist_to_target = distance(robot, approach_target)
        local angle_to_ball = math.atan(ball.y - robot.y, ball.x - robot.x)
        if dist_to_target < threshold and angle_diff(robot.orientation, angle_to_ball) < angle_tolerance then
            state = "capture"
            print("[CaptureBall] Transitioning to capture state.")
        end

    elseif state == "capture" then
        -- Activate the dribbler to make contact with the ball.
        api.dribbler(robotId, team, dribbleSpeed)
        api.move_to(robotId,team,{x=ball.x, y = ball.y})
        -- Check if the robot is sufficiently close to the ball.
        if distance(robot, ball) <= (safe_distance) then
            state = "idle"
            print("[CaptureBall] Transitioning to idle state.")
        end

    elseif state == "idle" then
        -- Maintain dribbler activation to hold the ball.
        api.dribbler(robotId, team, dribbleSpeed)
        if distance(robot, ball) > (safe_distance + threshold) then
            state = "approach"
        else
            print("[CaptureBall] Idle state: Holding the ball.")
        end
    end
end

return M
