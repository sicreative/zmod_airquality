const Airquality = require("./lib/binding.js");
const instance = new Airquality("zmod");

var sensor_json = "";




function load_airquailty()
{
    
    
    console.log("Start ZMOD Sesnor Node.js NAPI addon");

    instance.startsensormeasurement(sensorResult);

 
   
    console.log("Zmod add-on load OK!");
}





async function sleep(milliseconds) {
  const date = Date.now();
  let currentDate = null;
  do {
    currentDate = Date.now();
  } while (currentDate - date < milliseconds);
}

function next(){
  instance.startsensormeasurement(sensorResult);
}


function sensorResult (err,result) {
    //console.log(result);
    sensor_json = result;
  //  sleep(1990);
    
  //  instance.startsensormeasurement(sensorResult);
  
    //let nextReadTime = Date.now() + 1990;
    
   // while(Date.now()< nextReadTime);
    
    setTimeout(next,1990);
    
    console.log(result);
  
   // instance.startsensormeasurement(sensorResult);
    
    
    
    
    
    return;
}

function clearup(){
  instance.clearup();
  process.exit();
}


var http = require('http');


load_airquailty();

http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end(sensor_json);
  
}).listen(8182, 'localhost');
//8182


process.on('exit', clearup);

process.on('SIGINT', clearup);

process.on('SIGUSR1', clearup);
process.on('SIGUSR2', clearup);

console.log('Server running at http://localhost:8080/');



