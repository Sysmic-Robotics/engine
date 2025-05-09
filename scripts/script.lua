print("Script initialized")
package.path = "C:/Robocup/CondorSSL/scripts/luainterface/?.lua;" .. package.path

local move = require("move")
local capture = require("capture_ball")
local kick = require("kick_to_point")
local api = require("sysmickit.lua_api")

robotId = 0
team = 0

local radius = 4.0
local center = { x = 0, y = 0 }
local numPoints = 2
local points = {}
local currentIndex = 1
local threshold = 0.1

-- ✅ Precompute the circle waypoints without using table.insert
for i = 1, numPoints do
  local angle = (2 * math.pi) * (i - 1) / numPoints
  points[i] = {
    x = center.x + radius * math.cos(angle),
    y = center.y + radius * math.sin(angle),
  }
end

local function distance(p1, p2)
  local dx = p1.x - p2.x
  local dy = p1.y - p2.y
  return math.sqrt(dx * dx + dy * dy)
end

function process()
  local ball = api.get_ball_state()
  api.face_to(robotId, team, {x=ball.x,y=ball.y}, 1,1,0.05)
end
