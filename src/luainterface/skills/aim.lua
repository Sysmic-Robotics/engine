-- skills/aim.lua
local api = require("lua_api")

local aim = {}

local done = {}

function aim.run_once(id, team, target)
    done[id] = false
    api.face_to(id, team, target)
    done[id] = true
end

function aim.is_done(id)
    return done[id] == true
end

function aim.reset(id)
    done[id] = false
end

return aim

