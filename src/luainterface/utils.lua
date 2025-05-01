-- utils.lua
local M = {}

--- Euclidean distance between two points.
-- @param a table {x, y}
-- @param b table {x, y}
-- @return number
function M.distance(a, b)
    return math.sqrt((a.x - b.x)^2 + (a.y - b.y)^2)
end

--- Minimal absolute difference between two angles (in radians).
-- @param a number
-- @param b number
-- @return number
function M.angle_diff(a, b)
    local diff = a - b
    while diff > math.pi do diff = diff - 2 * math.pi end
    while diff < -math.pi do diff = diff + 2 * math.pi end
    return math.abs(diff)
end

return M

