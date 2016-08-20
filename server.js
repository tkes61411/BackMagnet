var http = require("http");
var url = require("url");
function start(route, handle) {
    function onRequest(request, response) {
        route(handle, request, response);

        //response.writeHead(200, {"Content-Type": "text/plain"});
        //response.write("Hello World");
        //response.end();
    }

    http.createServer(onRequest).listen(8888);
    console.log("Server has started at 127.0.0.1/8888.");
}

exports.start = start;
