
var express = require('express');

var spawn = require('child_process').spawn;

var app = express();
var http = require('http').Server(app);

var WS = require('ws').Server;

var os = require('os');
var byline = require('byline');

app.use(express.static(__dirname + '/client'));

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

    sendOutput(ws, "Hi there");

    var b2 = spawn("/home/ghost/Sources/boost/tools/build/src/engine/bin.linuxx86_64/b2",
                   ["-a", "-d0", "--python", "--server"],
                   {
                       cwd: "/home/ghost/Sources/boost/tools/build/example/libraries"
                   });

    b2.on('error', function(err) {
        console.log(JSON.stringify(err));
    });

    b2.on('exit', function(code, signal) {
        console.log("Process exit: " + code);
    });

    b2.on('close', function(code) {
        console.log('close: ' + code);
    });

    byline.createStream(b2.stdout).on('data', function(line) {
        console.log("< " + line);
        if (typeof line === 'object') {
            line = line.toString();
        }

        try {
            j = JSON.parse(line);
            s = JSON.stringify(j);
            ws.send(s);
        } catch(e) {
            console.log(e);
        }                                

    });

    b2.stderr.on('data', function(data) {
        console.log("<!" + data);
    });

    //b2.stdin.write(JSON.stringify({type: 'request', request: 'build'}) + "\n");
    
    ws.on('message', function(data) {
        console.log("> " + JSON.stringify(data));
        b2.stdin.write(data + "\n");
    });
});

ws.on('error', function(ws) {
    console.log('socket error');
});


