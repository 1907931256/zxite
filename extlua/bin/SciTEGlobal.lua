-- SciTEGlobal.lua an initialisation script for SciTE

function alert(message)
	message = gsub(message, "\r\n", "\n")
	message = gsub(message, "\n", "\n> ")
	trace("> " .. message .. "\n")
end
_ALERT=alert
SCI_SETCARETFORE=SCI_START+69

function onSavePointReached()
	editor:send(SCI_SETCARETFORE,RGB(0,0,0),0)
	return 0
end

function onSavePointLeft()
	editor:send(SCI_SETCARETFORE,RGB(128,0,128),0)
	return 0
end
