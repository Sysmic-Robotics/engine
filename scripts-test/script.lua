local robotId = 0
local team = 0


local field_limits = {
    x = {-0.5, 0.5},
    y = {-0.5, 0.5}
}
local function is_out_of_bounds(x, y)
    return x < field_limits.x[1] or x > field_limits.x[2]
        or y < field_limits.y[1] or y > field_limits.y[2]
end

local function distance(a, b)
    local dx = a.x - b.x
    local dy = a.y - b.y
    return math.sqrt(dx * dx + dy * dy)
end

local frames = 0
local waiting = true

function process()
    local robot = get_robot_state(robotId, team)
    local ball = get_ball_state()
    if frames >= 60 * 10 then
      send_velocity(robotId, team, 0, 0, 0)
    else
      --send_velocity(robotId, team, 0, 2, 0)
      move_to(robotId, team, {x = 0.0, y = 0.0})
      --face_to(robotId, team, ball)
      --motion(robotId, team,  {x = 0.0, y = 0.0}, 0.576, 1, 2.03, 0.92)
      --motion(robotId, team, {x = 0.0, y = 0.0}, 10.46, 39.28, 12.9, 71.67)
    end
    


    frames = frames + 1
end