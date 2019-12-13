let net = require('net')
let util = require('util')
let WebSocketServer = require('ws').Server
let webClients = {}
let pass = false
wss = new WebSocketServer({ port: 8182 });
wss.on('connection', function (ws) {
  console.log('client connected');
  webClients['smile'] = ws
  ws.on('message', function (message) {
		if(pass) {
			sendMsg('kangkang', message)
		} else if(message == clients['kangkang'].password) {
			console.log('已连接')
			pass = true
			ws.send('true')
			sendMsg('kangkang', 'start')
		} else {
			console.log('密码错误')
			ws.send('false')
		}
  });
});

//let HOST = 'localhost'
let HOST = '192.168.137.1'
let PORT = 18001
let clients = {}

function broadcast (msg) {
	for (let name in clients) {
		clients[name].write(msg + '\r\n')
	}
}
function sendMsg(username, msg) {
	for (let name in clients) {
		if (username == name) {
			clients[name].write(msg + '\r\n')
		}
	}
}
function sendMsgToWeb(username, msg) {
	for (let name in webClients) {
		if (username == name) {
			webClients[name].send(msg + '\r\n')
		}
	}
}
function randomPassword () {
	let password = ''
	let number
	for (let  i= 0;  i< 6; i++) {
		number = Math.floor(Math.random()*10)
		password += ''
		password += number
	}
	return password
}
let server = net.createServer((socket) => {
	socket.setEncoding('utf8')
	server.getConnections((err, count) => {
		console.log('在线人数是' + count + '位，请输入你的昵称:\r\n')
		socket.write('please input your nickname: \r\n')
	})
	
	let username
	let tousername
	socket.on('data', (data) => {
		data = data.replace(/\r\n/, '')

		if (username) {
			if(tousername) {
				sendMsg(tousername, `${data}`)
				sendMsgToWeb(tousername,`${data}`)
				console.log(data)
				//socket.write(`Send successfully to ${tousername}\r\n`)
			} else {
				tousername = data
				socket.write(`${username} will send msg to ${tousername}\r\n`)
				console.log(`${username} 将发信息给 ${tousername}\r\n`)
			}
		} else {
			if (clients[data]) {
        socket.write('Your nickname < ' + data + ' >are Occupied,Please change your nickname\r\n')
        console.log('您的昵称' + data + '被占用了，请您更换新的昵称\r\n')
			} else {
				username = data
				socket.password =  randomPassword ()
        clients[username] = socket
				console.log(`欢迎${username}加入`)
				console.log(`${username} password is ${socket.password}`)
				socket.write(`your password is: <${socket.password}>\r\n`)
				socket.write('please input your msg where to send: \r\n')
			}
		}
	})
	
	socket.on('end', () => {
		//broadcast(username, `${username} leave`)
		clients[username] && clients[username].destroy()
		delete clients[username]
	})
})


server.listen(PORT, HOST, () => {
	console.log(`tcp服务器已启动，地址是${util.inspect(server.address())}`)
})