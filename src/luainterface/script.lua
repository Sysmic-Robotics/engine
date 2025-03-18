-- print("Lua package path: " .. package.path)
print("Script initialized")

Robot = require("robot")  -- Import the Robot struct

-- Define the robot's physical size (diameter in meters)
local ROBOT_DIAMETER = 0.18
-- Use the robot's diameter as the capture threshold
local CAPTURE_DISTANCE = ROBOT_DIAMETER  

-- Robot identification variables.
robotId = 1       
team = 0    

-- process() is called every frame.
function process()
    -- Retrieve the current ball and robot state.
    local ball = get_ball_state()
    local robot = get_robot_state(robotId, team)
    
    -- Calculate the distance between the robot and the ball.
    local dx = ball.x - robot.x
    local dy = ball.y - robot.y
    local distance = math.sqrt(dx * dx + dy * dy)
    
    -- Command the robot to move toward the ball using a point with named keys.
    
    
    -- Rotate the robot so that it faces the ball.
    
    
    if distance < CAPTURE_DISTANCE then
        -- Activate the dribbler at maximum speed to capture the ball.
        dribbler(robotId, team, 10)
        face_to(robotId,team, {x = ball.x, y = ball.y } )
    else
        -- If the ball is too far, turn off the dribbler.
        dribbler(robotId, team, 0)
        move_to(robotId, team, {x = ball.x, y = ball.y})
        face_to(robotId,team, {x = ball.x, y = ball.y } )
        
    end
end
