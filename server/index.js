
var express = require('express');

var spawn = require('child_process').spawn;

var app = express();
var http = require('http').Server(app);

var WS = require('ws').Server;

app.get('/', function(req, res) {
    res.send('<h1>Hello, World</h1>')
});

http.listen(3000, function() {
    console.log("Listening");
});

ws = new WS({server: http});

function sendOutput(ws, output) {
    var r = {type: 'event', event: 'output', content: output};
    ws.send(JSON.stringify(r));
}

ws.on('connection', function(ws) {
    console.log("client connected");

    ws.on('message', function(xm) {
        var m = JSON.parse(xm);

        if (m.type === 'request') {

            if (m.request === 'build') {

                var b2 = spawn("/home/ghost/Sources/boost/b2",
                               ["-a"],
                               {
                                   cwd: "/home/ghost/Sources/boost/tools/build/example/hello"
                               });

                b2.stdout.on('data', function(data) {
                    if (typeof data === 'object') {
                        data = data.toString();
                    }
                    console.log(JSON.stringify(data));
                    sendOutput(ws, data);
                });

                setTimeout(function() {
                    sendOutput(ws, "message from server");
                }, 1500);
            }

        }

        console.log("got message: " + m);
    });
});


