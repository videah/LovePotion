io.stdout:setvbuf("no")
function love.load()
	local http = require 'socket.http'

	print("{Lua} Requesting file..")

	local file, code = http.request("http://stabyourself.net/dl.php?file=mari0-1006/mari0-source.zip")

	print(string.format(file, "%x"))
end