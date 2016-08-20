//const spawn = require('child_process').spawn;
//const cat_fifo = spawn('cat', ['fifo']);
//
//cat_fifo.stdout.on('data', (data) => {
//    console.log(`stdout: ${data}`);
//});


// FUNCTION: sleep
function sleep(milliSeconds) {
    var startTime = new Date().getTime();
    while (new Date().getTime() < startTime + milliSeconds);
}

// build http connection
//http = require('http');
//http.createServer(function (req, res) {
//  res.writeHead(200, {'Content-Type': 'text/plain'});
//  //res.end('Hello World\n');
//  res.end("http://cc.ee.ntu.edu.tw/~cmli/");
//}).listen(1337, '127.0.0.1');
//console.log('Server running at http://127.0.0.1:1337/');

var server = require("./server");
var router = require("./router");
var requestHandlers = require("./requestHandlers");
var spawn = require("child_process").spawn;

var handle = {}
handle["/"] = requestHandlers.start;
handle["/start"] = requestHandlers.start;
handle["/upload"] = requestHandlers.upload;

// launch ccode
//const ccode = spawn("./simulator");

server.start(router.route, handle);
