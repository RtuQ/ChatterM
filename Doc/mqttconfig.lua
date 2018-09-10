-- init mqtt client without logins, keepalive timer 120s
m = mqtt.Client("41146081", 120, "169571", "aJNBrCfwey6op5i3xdBM91=HyMA=")

m:on("connect", function(client) print ("connected") end)
m:on("offline", function(client) print ("offline") end)


m:on("message", function(client, topic, data) 
  print(topic .. ":" ) 
  if data ~= nil then
    print(data)
  end
end)



tem = 1
hum = 1
led = 2
stream="{\"temperature\":"..tem..",\"humidity\":"..hum.."}"
str="{\"LED\":"..led.."}"


m:connect("183.230.40.39", 6002, 0, function(client)
  print("connected")
  client:subscribe("/master", 0, function(client) print("subscribe success") end)
  client:publish("$dp",onenetstr(stream),0,0,function(client) print("sent")end)
end,
function(client, reason)
  print("failed reason: " .. reason)
end)

function onenetstr(json)
    buf = {}
    buf[0] = 0x03 -- Byte1 Type=3
    jsonlength = string.len(json)
    buf[1] = bit.rshift(bit.band(jsonlength, 0xFF00), 8) 
    buf[2] = bit.band(jsonlength, 0xFF) + 1
    return string.char(buf[0])..string.char(buf[1])..string.char(buf[2])..json.."\r" 
end

function find_data(str)
local i,j,d,data
    i=string.find(str,'T')
    j=string.find(str,'H')
    d=string.find(str,'D')
    if(i ~= nil)then
    tem = string.sub(str,i+1,i+2)
    hum = string.sub(str,j+1,j+2)
    stream="{\"temperature\":"..tem..",\"humidity\":"..hum.."}"
    m:publish("$dp",onenetstr(stream),0,0,function(client) print("sent")end)
    end
    if(d ~= nil)then
    data = string.sub(str,d+1)
    led = data
    data = data +'0'
     m:publish("/led",data,0,0,function(client) print("sent")end)
     str="{\"LED\":"..led.."}"
      m:publish("$dp",onenetstr(str),0,0,function(client) print("sent")end)
    end
end

uart.on("data","\r",
  function(data)
   find_data(data)
end, 0)




