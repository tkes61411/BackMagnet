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
    const echo_fifo = exec_sync("printf " + query + " > Cpackage/myfifo");

    console.log("Start to cat...");
    const cat_fifo = spawn_sync('cat', ['Cpackage/myfifo2']);
    console.log(cat_fifo.stdout.toString());

    // read c++ result
    var profit_string = fs.readFileSync('Cpackage/profit.rpt', 'utf8');
    var profit_array  = profit_string.split(", ");
    var profit_yaxis = "[";
    for(i = 0; i < profit_array.length - 1; i++){
        profit_yaxis = profit_yaxis + "{x: " + i + ", y: " + profit_array[i] + "}, ";
    }
    profit_yaxis = profit_yaxis + "{x: " + i + ", y: " + profit_array[profit_array.length-1] + "}]";

    var total_string = fs.readFileSync('Cpackage/total.rpt', 'utf8');
    var total_array  = total_string.split(", ");
    var total_yaxis = "[";
    var diff_yaxis = "[";
    var temp = total_array[0];
    for(i = 0; i < total_array.length - 1; i++){
        total_array[i] = total_array[i]/temp;
        total_yaxis = total_yaxis + "{x: " + i + ", y: " + total_array[i] + "}, ";
        diff_yaxis = diff_yaxis + "{x: " + i + ", y: " + (profit_array[i]-total_array[i]) + "}, ";
    }
    total_yaxis = total_yaxis + "{x: " + i + ", y: " + total_array[profit_array.length-1]/temp + "}]";
    diff_yaxis = diff_yaxis + "{x: " + i + ", y: " + (profit_array[total_array.length-1]-total_array[total_array.length-1]) + "}]";
    //console.log(diff_yaxis);
    //var labels_string = "[\"2006\", ";
    //for(i = 0; i < profit_array.length - 2; i++){
    //    labels_string = labels_string + "\" \", ";
    //}
    //labels_string = labels_string + "\"2016\"]";
    //console.log(labels_string);
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
        
        var body = '<!DOCTYPE HTML>'+
		'<html>'+
		'<head>'+
		'<script type="text/javascript">'+
		'window.onload = function () {'+
		'var chart = new CanvasJS.Chart("chartContainer",'+
		'{'+
		'title:{'+
		'text: "回測結果",'+
		'fontSize: 30'+
		'},'+
		'animationEnabled: true,'+
        'animationDuration: 600,'+
		'axisX:{'+
        'title: "Timeline(Days)",'+
		'gridColor: "Silver",'+
		'tickColor: "silver",'+
        'interval:2600,'+
		//'valueFormatString: "DD/MMM"'+
		'},'+
		'toolTip:{'+
		'shared:true'+
		'},'+
		'theme: "theme2",'+
		'axisY: {'+
        'title: "Gain(Normalized)",'+
		'gridColor: "Silver",'+
		'tickColor: "silver",'+
        //'maximum: 2,'+
        //'minimum: -2,'+
        'interval: 0.5,'+
		'},'+
		'legend:{'+
        'fontSize: 22,'+
		'verticalAlign: "center",'+
		'horizontalAlign: "right"'+
		'},'+
		'data: ['+
		'{'+
		'type: "line",'+
		'showInLegend: true,'+
		'lineThickness: 3,'+
		'name: "Target Gain",'+
		//'markerType: "square",'+
		'color: "#FF8F1F",'+
		'dataPoints: '+ profit_yaxis +
		'},'+
		'{        '+
		'type: "line",'+
		'showInLegend: true,'+
		'name: "Composite Index",'+
		'color: "#919191",'+
		'lineThickness: 1,'+
		'dataPoints: '+ total_yaxis +
		'},'+
		'{        '+
		'type: "line",'+
		'showInLegend: true,'+
        'visible: false,'+
		'name: "Diff against Composite Index",'+
		'color: "#FF0000",'+
		'lineThickness: 2,'+
		'dataPoints: '+ diff_yaxis +
		'}'+
		'],'+
		'legend:{'+
		'cursor:"pointer",'+
		'itemclick:function(e){'+
		'if (typeof(e.dataSeries.visible) === "undefined" || e.dataSeries.visible) {'+
		'e.dataSeries.visible = false;'+
		'}'+
		'else{'+
		'e.dataSeries.visible = true;'+
		'}'+
		'chart.render();'+
		'}'+
		'}'+
		'});'+
		'chart.render();'+
		'}'+
		'</script>'+
		'<script type="text/javascript" src="http://canvasjs.com/assets/script/canvasjs.min.js"></script>'+
		'</head>'+
		'<body>'+
		'<div id="chartContainer" style="height: 300px; width: 100%;">'+
		'</div>'+
		'</body>'+
		'</html>';



        //var body = '<!DOCTYPE html>'+
        //'<html>'+
        //'<head>'+
        //'<title></title>'+
        //'<meta charset="utf-8" />'+
        //'<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/1.0.1/Chart.js"></script>'+
        //'<style>'+
        //'</style>'+
        //'</head>'+
        //'<body>'+
        //'<canvas id="myChart"></canvas>'+
        //'<script>'+
        //'var data = {'+
        //    'labels: ' + labels_string + ','+
        //        'datasets: ['+
        //        '{'+
        //        'label: "My First dataset",'+
        //        'fill: true,'+
        //        'lineTension: 0.1,'+
        //        'backgroundColor: "rgba(75,192,192,0.4)",'+
        //        'borderColor: "rgba(192,75,0,1)",'+
        //        'borderCapStyle: \'butt\','+
        //        'borderDash: [],'+
        //        'borderDashOffset: 0.0,'+
        //        'borderJoinStyle: \'miter\','+
        //        'pointBorderColor: "rgba(75,192,192,1)",'+
        //        'pointBackgroundColor: "#fff",'+
        //        'pointBorderWidth: 0,'+
        //        'pointHoverRadius: 0,'+
        //        'pointHoverBorderColor: "rgba(220,220,220,1)",'+
        //        'pointHoverBorderWidth: 0,'+
        //        'pointRadius: 0,'+
        //        'pointHitRadius: 0,'+
        //        'spanGaps: false,'+
        //        'radius: 0,'+
        //        //'label: "My First dataset",'+
        //        //'fillColor: "rgba(220,220,220,0.2)",'+
        //        //'strokeColor: "rgba(220,220,220,1)",'+
        //        //'pointColor: "rgba(220,220,220,1)",'+
        //        //'pointStrokeColor: "#fff",'+
        //        //'pointHighlightFill: "#fff",'+
        //        //'pointHighlightStroke: "rgba(220,220,220,1)",'+
        //        'data: ' + profit_string + ',' +
        //    '}'+
        //    ']'+
        //'};'+
        //'var ctx = document.getElementById("myChart").getContext("2d");'+
        //'ctx.canvas.width = 1200;'+
        //'ctx.canvas.height = 600;'+
        //'var myNewChart = new Chart(ctx).Line(data);'+
        //'myNewChart.reDraw();'+
        //'</script>'+
        //'</body>'+
        //'</html>';

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
