-- run_des.lua
-- Run the DES encrypt/decrypt executable from Lua.
-- Usage:
--   lua run_des.lua encrypt 0123456789ABCDEF 133457799BBCDFF1
--   lua run_des.lua decrypt 85E813540F0AB405 133457799BBCDFF1

local args = {...}

if #args ~= 3 then
  io.stderr:write("Usage: lua run_des.lua <encrypt|decrypt> <hex-data> <key>\n")
  os.exit(1)
end

local mode, data, key = args[1], args[2], args[3]

if mode ~= "encrypt" and mode ~= "decrypt" then
  io.stderr:write("Mode must be 'encrypt' or 'decrypt'.\n")
  os.exit(1)
end

local command = string.format('./des_app %s %s %s', mode, data, key)
local success, _, exit_code = os.execute(command)

if not success then
  io.stderr:write("Failed to run DES command.\n")
  os.exit(exit_code or 1)
end

os.exit(0)
