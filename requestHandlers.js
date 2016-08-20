var exec = require("child_process").exec;
var exec_sync = require("child_process").execSync;
var spawn = require("child_process").spawn;
var spawn_sync = require("child_process").spawnSync;
var url = require("url");
var querystring = require("querystring")
var fs = require('fs');
var string = require('string');

// Main function while web request
function start(request,response) {
    console.log("Request handler 'start' was called.");

    // add query type here!!
    var query    = url.parse(request.url).query;
    console.log("Query is    " + query);
    //var id       = querystring.parse(query)["id"];
    //var type     = querystring.parse(query)["type"];
    //var argu     = querystring.parse(query)["argu"];
    //console.log("Request for id=" + id + " type=" + type + " argu=" + argu);

    console.log("Start to echo...");
    //const echo_fifo = exec_sync("printf " + type + " > myfifo");

    console.log("Start to cat...");
    //const cat_fifo = spawn_sync('cat', ['myfifo']);
    //console.log(cat_fifo.stdout.toString());

    // read c++ result
    var profit_string = fs.readFileSync('profit.rpt', 'utf8');
    var profit_array  = profit_string.split(", ");
    var profit_string = "[" + profit_string + "]";

    var labels_string = "[\"2006\", ";
    for(i = 0; i < profit_array.length - 2; i++){
        labels_string = labels_string + "\" \", ";
    }
    labels_string = labels_string + "\"2016\"]";
    console.log(labels_string);
    //var fifty_array = fs.readFileSync('fifty.rpt', 'utf8');
    //var profit_array  = profit_string.split(" ");

    exec("ls -lah", function (error, stdout, stderr) {
        //var body = '<html>'+
        //'<head>'+
        //'<meta http-equiv="Content-Type" content="text/html; '+
        //'charset=UTF-8" />'+
        //'</head>'+
        //'<body>'+
        //'<form action="/upload" method="post">'+
        //'<textarea name="text" rows="20" cols="60"></textarea>'+
        //'<input type="submit" value="Submit text" />'+
        //'</form>'+
        //'</body>'+
        //'</html>';

        //html_file = fs.readFileSync('d3.html');
        //response.writeHeader(200, {"Content-Type": "text/html"});
        //response.write(html_file);
        //response.end();

        var body = '<!DOCTYPE html>'+
        '<html>'+
        '<head>'+
        '<title></title>'+
        '<meta charset="utf-8" />'+
        '<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/1.0.2/Chart.min.js"></script>'+
        '<style>'+
        '</style>'+
        '</head>'+
        '<body>'+
        '<canvas id="myChart"></canvas>'+
        '<script>'+
        'var data = {'+
            'labels: ' + labels_string + ','+
            'datasets: ['+
            '{'+
                'label: "My First dataset",'+
                'fillColor: "rgba(220,220,220,0.2)",'+
                'strokeColor: "rgba(220,220,220,1)",'+
                'pointColor: "rgba(220,220,220,1)",'+
                'pointStrokeColor: "#fff",'+
                'pointHighlightFill: "#fff",'+
                'pointHighlightStroke: "rgba(220,220,220,1)",'+
                'data: ' + profit_string +
            '}'+
            ']'+
        '};'+
        'var ctx = document.getElementById("myChart").getContext("2d");'+
        'var myNewChart = new Chart(ctx).Line(data);'+
        '</script>'+
        '</body>'+
        '</html>';

        response.writeHead(200, {"Content-Type": "text/html"});
        response.write(body);
        response.end();
        //response.writeHead(200, {"Content-Type": "text/plain"});
        //response.write(stdout);
        //response.end();
    });
}

function upload(request,response) {
    console.log("Request handler 'upload' was called.");
    response.writeHead(200, {"Content-Type": "text/plain"});
    response.write("Hello Upload");
    response.end();
}

exports.start = start;
exports.upload = upload;
