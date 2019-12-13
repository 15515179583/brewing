var pass = false
var temp = document.querySelector(".temp")
var weight = document.querySelector(".weight")
var stir = document.querySelector(".stir")
var water = document.querySelector(".water")

document.querySelector("#switch").value = 'start'

if ("WebSocket" in window) {
  // 打开一个 web socket
  var ws = new WebSocket("ws://localhost:8182");
  ws.onopen = function() {
    // Web Socket 已连接上，使用 send() 方法发送数据
   // ws.send('007790');
  };
  ws.onmessage = function (evt) {
    var received_msg = evt.data;
    console.log("数据已接收...");
    if(evt.data == 'true') {
      pass = true
      open_close_Switch()
      document.querySelector(".password").value =""
      document.querySelector(".password").disabled ="disabled"
      document.querySelector(".password").placeholder ="已连接"
    } else if (evt.data == 'false') {
      alert('密钥输入错误！！！')
    } else if(pass){
      console.log(evt.data)
      data = JSON.parse(evt.data)
      temp.innerHTML = data.temp
      //console(data.weight)
      weight.innerHTML = data.weight
      if(data.isWater == 0) {
        water.className = 'water state2'
      } else {
        water.className = 'water state1'
      }
      if(data.isStir==0) {
        stir.className = 'stir state2'
      } else {
        stir.className = 'stir state1'
      }
    }
  };
    ws.onclose = function()
    { 
    // 关闭 websocket
    alert("连接已关闭..."); 
    };
} else {
  // 浏览器不支持 WebSocket
  alert("您的浏览器不支持 WebSocket!");
}

function open_close_Switch() {
  let password = document.querySelector(".password").value
  let btn= document.querySelector("#switch")
  let data = btn.value
  if(!pass) {
    ws.send(password)
    return
  }
  if(data == 'end') {
    ws.send(data)
    water.className = 'water state2'
    stir.className = 'stir state2'
    weight.innerHTML = ''
    temp.innerHTML = ''
    document.querySelector("#switch").value = 'start'
    btn.innerHTML = "开"
  } else {
    ws.send(data)
    document.querySelector("#switch").value = 'end'
    btn.innerHTML = "关"
  }
  console.log(data)
}