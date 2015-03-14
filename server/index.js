
var express = require('express');
var compress = require('compression')
var logger = require('morgan')

var spawn = require('child_process').spawn;

var app = express();
var http = require('http').Server(app);

var WS = require('ws').Server;

var os = require('os');
var byline = require('byline');
var temp = require('temp');
var ncp = require('ncp');
var _ = require('underscore');


app.use(compress());

app.use(express.static(__dirname + '/client'));

app.use(logger());

http.listen(3000, function() {
    console.log("Listening");
});

ws = new WS({server: http});

function sendOutput(ws, output) {
    var r = {type: 'event', event: 'output', content: output};
    ws.send(JSON.stringify(r));
}

function talkToB2(ws, b2)
{
    ws.on('message', function(data) {
        console.log("> " + JSON.stringify(data));
        b2.stdin.write(data + "\n");
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
            // We've logged this output above, but since it's not valid
            // JSON, don't send to websocket.
        }                                

    });

    b2.stderr.on('data', function(data) {
        console.log("<!" + data);
    });

    ws.send(JSON.stringify({type: 'event', event: 'server-ready'}));
}

ws.on('connection', function(ws) {
    console.log("client connected");

    var binary = __dirname + "/../src/engine/bin.linuxx86_64.debug/b2"
    var example = __dirname + "/../example/libraries"

    temp.mkdir('libraries', function(err, destination) {
        ncp(example, destination, function (err) {
            if (err) {
                return console.error(err);
            } else {
                
                console.log("Copied example to" + destination);

                env = _.extend({}, process.env, {
                    BOOST_BUILD_PATH: __dirname + "/.."
                })
                
                var b2 = spawn(binary,
                               ["-a", "-d0", "--python", "--server"],
                               {
                                   cwd: destination,
                                   env: env
                               });
                
                talkToB2(ws, b2);
                
            }
        });
    });
    
});

ws.on('error', function(ws) {
    console.log('socket error');
});


