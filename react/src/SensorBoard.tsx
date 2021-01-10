import React from 'react';
import SensorItem from './SensorItem'
import SensorChart from './SensorChart' 
import './SensorBoard.css'




const sensor_url = '/sensor'




// Finally we'll embed it all in an SVG
class SensorBoard extends React.Component<any,any> {
timerID: any;
resizetimerID: any;
resizing: Boolean;

 
constructor(props:any){
    super(props);
    
    this.timerID = 0;
    this.resizetimerID = 0;
    this.resizing = false;
    
 
    
    this.resize = this.resize.bind(this);
    this.sensorItemChartSwitch = this.sensorItemChartSwitch.bind(this);
    
    this.state = {
         chartconfig : [],
         records : [],
         sensors : [],
	 sensorchartwidth : window.innerWidth - 250,
	 sensorchartheight : 480
  };


 }



componentDidMount(){
   this.timerID = setInterval(
	()=> this.getSensor(),
	3000
   );
}

componentWillUnmount(){
    clearInterval(this.timerID);
}

getSensor(){
    fetch(sensor_url)
	.then(res => res.json())
      	.then(
	 (result) => {
	 
	  result['date'] = Date.now();

	   this.setState({
	    
	    sensors: result
	    

	  });
	  
	  this.state.records.push(result);
	  console.log(this.state.sensors);
	  console.log(this.state.records.length)

	},
        (error) => {
		console.log("error load sensor json")
	}
    )

 }
 
delayresize(){
    this.setState({
	sensorchartwidth: window.innerWidth -250
	
	
	});
     this.resizing = false;	  
}

resize(){

    
	
      if (this.resizing)
	  clearTimeout(this.resizetimerID);
      this.resizetimerID = setTimeout(
	()=> this.delayresize(),
	20
	);
      this.resizing = true;
	
  }
  
sensorItemChartSwitch(id: string,enable:Boolean){
  
  this.setState((state:any) => {
    state.chartconfig[id] = enable;
    return {chartconfig: state.chartconfig};
    });
 
  
  
 
 
}  


  
  


render(){
  const {sensors,sensorchartwidth,sensorchartheight,records,chartconfig} = this.state;
  
  
  
  const iaq_red =  sensors['iaq']>3.5?0xFF:Math.round((sensors['iaq'] - 1.0) / 2.5*255.0);
  const iaq_green = sensors['iaq']>6.0?0:sensors['iaq']<3.5?0xFF:Math.round(255-(sensors['iaq'] - 3.5) / 2.5*255.0);
  
  
  const iaq_name_color = '#'+ (iaq_red<=0xF?'0':'') +iaq_red.toString(16) + (iaq_green<=0xF?'0':'') + iaq_green.toString(16) +'00';


  window.addEventListener('resize', ()=>{
      this.resize();
    });  
  
   document.title = 'ZMOD IAQ Demo'

  return (
  
    <div className="row">
    <div className="columnleftitem">
    <SensorItem name="💨 IAQ" id="iaq" checkedCallback={this.sensorItemChartSwitch} value={sensors['iaq']} chartcolor="green" namecolor={iaq_name_color}/>
    <SensorItem name="🏭 eCO2" id="eco2" checkedCallback={this.sensorItemChartSwitch} value={sensors['eco2']} chartcolor="#330019"/>
    <SensorItem name="🌡  Temp" id="temperature" checkedCallback={this.sensorItemChartSwitch} value={sensors['temperature']} chartcolor="#FF0000"/>
    <SensorItem name="💦 Humid" id="humidity" checkedCallback={this.sensorItemChartSwitch} value={sensors['humidity']} chartcolor="#3399FF"/>
    
    </div>
    
    <div id="rightchat" className="columnrightchat">
    
   
    
    <SensorChart chartconfig={chartconfig} data={records} width={sensorchartwidth} height={sensorchartheight} />
     
  
    </div>
    </div>
    
    

   );  	
 
}
}

export default SensorBoard;


