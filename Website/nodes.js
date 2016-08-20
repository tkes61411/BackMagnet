nodes = [];
categorys = [0,1,2,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2];
person = -1
svg = null
function init()
{

  var width = window.innerWidth - 400,
      height = 700;
  var fixMagnet = ['選股','進場','退場']
  var stocksSelection = ['一支股票','全部股票','前十獲利股'];
  var stocksEnter = ['超越5日平均','超越10日平均','超越20日平均'
                    ,'日KD黃金交叉','週KD黃金交叉','月KD黃金交叉'
                    ,'布林通道下緣'];
  var stocksExist = ['低於5日平均','低於10日平均','低於20日平均'
                    ,'日KD死亡交叉','週KD死亡交叉','月KD死亡交叉'
                    ,'布林通道上緣'];
  // create nodes
  text = fixMagnet.concat(stocksSelection)
                  .concat(stocksEnter)
                  .concat(stocksExist);
  

  //assert(text.length == categorys.length);

  
  mainCirclePosY = []
  for (var i  = 0 ; i < text.length ; i += 1)
  {
    var x,y,fixed,radius;
    switch(i)
    {
      case 0:
        x = width/2;
        y = 60 + 200;
        fixed = true;
        radius = 60;
        break;
      case 1:
      case 2:
        x = i  * (width / 3);
        y = 60/2+10+500;
        fixed = true;
        radius = 60;
        break;
      case 3:
      case 4:
      case 5:
        x = (i * (width / 8));
        y = (Math.ceil( (i-1)/6 ))* 65;
        fixed = true;
        radius = 30;
        break;
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
        x = ((i-2)%2+1) * (width / 7);
        y = (Math.ceil( (i-1)/2 ))* 65 - 50;
        fixed = true;
        radius = 30;
        break;
      default:
        x = ((i-2)%2+5) * (width / 7);
        y = (Math.ceil( (i-8)/2 ))* 65 - 50;
        fixed = true;
        radius = 30;
    }
    console.log(categorys[i])
    nodes.push({
      type: categorys[i],
      radius: radius,
      fixed: fixed,
      initx: x,
      inity: y,
      x: x,
      y: y,
      title:text[i],
      level: 100
    });
  };
  console.log(nodes)

  var color = ['FFE66F','70E4EF','ADF6B1'];


  var force = d3.layout.force()
      .nodes(nodes)
      .size([width, height])
      .gravity(0)
      .charge(0)
      .on("tick", ticked)
      .start();

  svg = d3.select("#nodes").append("svg")
      .attr("width", width)
      .attr("height", height);
      //.on("mousemove", mousemoved);

  svg.append("rect")
      .attr("width", width)
      .attr("height", height)
      .attr("fill","white");

  svg.append("ellipse")
    .attr("cx",nodes[0].x)
    .attr("cy",nodes[0].y)
    .attr("rx",100)
    .attr("ry",100)
    .attr("style","fill:none;stroke:gray;stroke-width:2")
    .attr("stroke-dasharray","5,10")

  svg.append("ellipse")
    .attr("cx",nodes[0].x)
    .attr("cy",nodes[0].y)
    .attr("rx",150)
    .attr("ry",150)
    .attr("style","fill:none;stroke:gray;stroke-width:2")
    .attr("stroke-dasharray","5,10")

  svg.append("ellipse")
    .attr("cx",nodes[1].x)
    .attr("cy",nodes[1].y)
    .attr("rx",100)
    .attr("ry",100)
    .attr("style","fill:none;stroke:gray;stroke-width:2")
    .attr("stroke-dasharray","5,10")

  svg.append("ellipse")
    .attr("cx",nodes[1].x)
    .attr("cy",nodes[1].y)
    .attr("rx",150)
    .attr("ry",150)
    .attr("style","fill:none;stroke:gray;stroke-width:2")
    .attr("stroke-dasharray","5,10")

  svg.append("ellipse")
    .attr("cx",nodes[2].x)
    .attr("cy",nodes[2].y)
    .attr("rx",100)
    .attr("ry",100)
    .attr("style","fill:none;stroke:gray;stroke-width:2")
    .attr("stroke-dasharray","5,10")

  svg.append("ellipse")
    .attr("cx",nodes[2].x)
    .attr("cy",nodes[2].y)
    .attr("rx",150)
    .attr("ry",150)
    .attr("style","fill:none;stroke:gray;stroke-width:2")
    .attr("stroke-dasharray","5,10")

  move = false;


  var node_drag = d3.behavior.drag()
      .on("dragstart", dragstart)
      .on("drag", dragmove)
      .on("dragend", dragend);

  function dragstart(d, i) {
    move = false;
    console.log('start ',move)
    force.stop() // stops the force auto positioning before you start dragging
  }

  function dragmove(d, i) {
      console.log('move ',move)
      if (d.px != d3.event.dx || d.py != d3.event.dy)
        move = true;
      d.px += d3.event.dx;
      d.py += d3.event.dy;
      d.x += d3.event.dx;
      d.y += d3.event.dy; 
      ticked(); // this is the key to make it work together with updating both px,py,x,y on d !
  }

  function dragend(d, i) {
      ticked();
      if (move)
      {
        if (d.fixed == true)
        {
          d.x = d.px = d.initx;
          d.y = d.py = d.inity;
        }
        else
        {
          delx = d.x - nodes[d.type].x;
          dely = d.y - nodes[d.type].y;
          r = Math.sqrt(Math.pow(delx,2)+Math.pow(dely,2));
          if (r > d.level)
            d.level += 50;
          else
            d.level -= 50;
          if (d.level < 0)
            d.level = 100;
        }
      }
      force.resume();
  }


  svg.selectAll("circle")
      .data(nodes)
      .enter().append("circle")
      .attr("r", function(d) { return d.radius; })
      .style("fill", function(d) {return color[d.type]; })
      .attr("stroke-width",0)
      .on("click",function(d,idx){
          if (idx < 3)
            return;
          if (move)
            return;
          if (d.fixed == true)
          {
            d.fixed = false;
            if (idx == 3)
            {
              person = prompt("Please enter your favorate stock", "my stock");
    
              if (person != null) {
                  console.log(person)
                  d.title = "只買" + person;
                  //text[0][3].textContent = "只買"+person;
              }
              else
                d.fixed = true
            }
            //console.log(hi);
          }
          else
          {
            d.fixed = true;
            d.x = d.px = d.initx;
            d.y = d.py = d.inity;
            if (idx == 3)
              d.title = "一支股票";
          }
          force.resume();
        })
      .call(node_drag);
      //.on("")

  text = svg.selectAll("text")
      .data(nodes)
      .enter().append("text")

  var textLabels = text.append("tspan")
                 .attr("text-anchor", "middle")
                 .attr("x", function(d) { return d.x; })
                 .attr("y", function(d,idx) { return d.y; })
                 .text( function (d) { return d.title; })
                 .attr("font-size",function(d,idx){if(idx < 3)return 25;else return 15;});

  force.start();

  function ticked() {
    var q = d3.geom.quadtree(nodes),
        k = 0.04,
        i = 0,
        n = nodes.length,
        o;

    while (++i < n) {
      o = nodes[i];
      if (o.fixed) continue;
      c = nodes[o.type];
      delx = c.x - o.x;
      dely = c.y - o.y;
      r = Math.sqrt(Math.pow(delx,2)+Math.pow(dely,2));
      rreal = o.level;
      trueDelx = rreal/r * delx;
      trueDely = rreal/r * dely;
      truex = c.x - trueDelx;
      truey = c.y - trueDely;
      o.x += (truex - o.x) * k;
      o.y += (truey - o.y) * k;
      q.visit(collide(o));
    }

    svg.selectAll("circle")
        .attr("cx", function(d) { return d.x; })
        .attr("cy", function(d) { return d.y; });

    svg.selectAll("tspan")
        .text(function(d){return d.title})
        .attr("text-anchor", "middle")
        .attr("x", function(d) { return d.x; })
        .attr("y", function(d) { return d.y; });
  }

  function collide(node) {
    var r = node.radius + 106,
        nx1 = node.x - r,
        nx2 = node.x + r,
        ny1 = node.y - r,
        ny2 = node.y + r;
    return function(quad, x1, y1, x2, y2) {
      if (quad.point && (quad.point !== node)) {
        var x = node.x - quad.point.x,
            y = node.y - quad.point.y,
            l = Math.sqrt(x * x + y * y),
            r = node.radius + quad.point.radius;
        if (l < r) {
          l = (l - r) / l * .5;
          node.px += x * l;
          node.py += y * l;
        }
      }
      return x1 > nx2
          || x2 < nx1
          || y1 > ny2
          || y2 < ny1;
    };
  }
}
