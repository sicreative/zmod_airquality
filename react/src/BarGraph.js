import React from 'react';
import { letterFrequency } from '@visx/mock-data';
import { Group } from '@visx/group';
import { Bar } from '@visx/shape';
import { scaleLinear, scaleBand } from '@visx/scale';
import { Text } from '@visx/text'



const sensor_url = '/sensor'




// We'll use some mock data from `@visx/mock-data` for this.
const data = letterFrequency;

// Define the graph dimensions and margins
const width = 500;
const height = 500;
const margin = { top: 20, bottom: 20, left: 20, right: 20 };

// Then we'll create some bounds
const xMax = width - margin.left - margin.right;
const yMax = height - margin.top - margin.bottom;

// We'll make some helpers to get at the data we want
const x = d => d.letter;
const y = d => +d.frequency * 100;

// And then scale the graph by our data
const xScale = scaleBand({
  range: [0, xMax],
  round: true,
  domain: data.map(x),
  padding: 0.4,
});
const yScale = scaleLinear({
  range: [yMax, 0],
  round: true,
  domain: [0, Math.max(...data.map(y))],
});

// Compose together the scale and accessor functions to get point functions
const compose = (scale, accessor) => data => scale(accessor(data));
const xPoint = compose(xScale, x);
const yPoint = compose(yScale, y);




// Finally we'll embed it all in an SVG
class BarGraph extends React.Component {
 
constructor(props){
    super(props);
    this.state = {
         sensors : []
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

	   this.setState({
	    sensors: result

	  });
		       console.log(this.state.sensors);

	},
        (error) => {
		console.log("error load sensor json")
	}
    )

 }




render(){
         const {sensors} = this.state;


  return (
 
    
     <svg>
    <Text dx={margin.left} dy={margin.top}
    verticalAnchor="start" fontFamily="monospace" 
    style={{ fontSize: '2em'}} textAnchor="start">{sensors['iaq']}</Text>
  </svg>
/*
    <svg width={width} height={height}>
       {data.map((d, i) => {
        const barHeight = yMax - yPoint(d);
        return (
          <Group key={`bar-${i}`}>
            <Bar
              x={xPoint(d)}
              y={yMax - barHeight}
              height={barHeight}
              width={xScale.bandwidth()}
              fill="#fc2e1c"
            />
          </Group>
        );
      })}
    </svg>*/
   )  	
 }
}

export default BarGraph;

// ... somewhere else, render it ...
// <BarGraph />
