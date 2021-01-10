import React, { useMemo, useCallback } from 'react';
import { LinePath, AreaClosed, Line, Bar } from '@visx/shape';
import { curveMonotoneX } from '@visx/curve';
import { GridRows, GridColumns } from '@visx/grid';
import { scaleTime, scaleLinear } from '@visx/scale';
import { withTooltip, Tooltip, TooltipWithBounds, defaultStyles } from '@visx/tooltip';
import { WithTooltipProvidedProps } from '@visx/tooltip/lib/enhancers/withTooltip';
import { localPoint } from '@visx/event';
import { LinearGradient } from '@visx/gradient';
import { timeFormat } from 'd3-time-format';

import './SensorChart.css'


export const background = '#5f7394';
export const background2 = '#28637d';
export const accentColor = '#332233';
export const accentColorDark = '#112211';
const tooltipStyles = {
  ...defaultStyles,
  backgroundColor: '#54772a' ,
  border: '0px',
  color: '#EEEEEE',
};




// util
const formatDate = timeFormat("%X %x");



type Sensors = {[key: string]: any};
type ChartConfig = {[key: string]: any};


type TooltipData = Sensors;

export type Props = {
  data: Sensors[];
  width: number;
  height: number;
  margin?: { top: number; right: number; bottom: number; left: number };
  chartconfig?: ChartConfig;
};





export default withTooltip<Props, TooltipData>(
  ({
    data,
    chartconfig,
    width,
    height,
    margin = { top: 0, right: 0, bottom: 0, left: 0 },
    showTooltip,
    hideTooltip,
    tooltipData,
    tooltipTop = 0,
    tooltipLeft = 0,
  }:  Props & WithTooltipProvidedProps<TooltipData>) => {
  console.log(data);
   
    if (width < 10) return null;

    // bounds
    const innerWidth = width - margin.left - margin.right;
    const innerHeight = height - margin.top - margin.bottom;

    // scales
    const dateScale = useMemo(
      () =>{
         const hour = 60*60*1000;
         const quadday = hour*6;
         const day = quadday*4;
      
         let datedomain = [Date.now()-hour, Date.now()];
         
         if (data.length>1){
            let dataTimeRange = data[data.length-1]['date']-data[0]['date'];
            //max show 4 days
            if (dataTimeRange>day)
              datedomain = [Date.now()-(day*4), Date.now()];
            else if (dataTimeRange>quadday)
              datedomain = [Date.now()-(day), Date.now()];
            else if (dataTimeRange>hour)  
              datedomain = [Date.now()-(quadday), Date.now()];
           
                 
         }
          
        return scaleTime({
          range: [margin.left, innerWidth + margin.left],
         
        
          domain: datedomain,
        
            
        })
        
        },
         // eslint-disable-next-line 
      [innerWidth, margin.left,data.length],
    );
    const iaqValueScale = useMemo(
      () =>
        scaleLinear({
          range: [innerHeight + margin.top, margin.top],
          domain: [0, 7],
          nice: true,
        }),
      [margin.top, innerHeight],
    );
    
     const eco2ValueScale = useMemo(
      () =>
        scaleLinear({
          range: [innerHeight + margin.top, margin.top],
          domain: [0, 2000],
          nice: true,
        }),
      [margin.top, innerHeight],
    );
    
     const tempValueScale = useMemo(
      () =>
        scaleLinear({
          range: [innerHeight + margin.top, margin.top],
          domain: [-10, 50],
          nice: true,
        }),
      [margin.top, innerHeight],
    );
    
    
    const humidValueScale = useMemo(
      () =>
        scaleLinear({
          range: [innerHeight + margin.top, margin.top],
          domain: [0, 100],
          nice: true,
        }),
      [margin.top, innerHeight],
    );

    // tooltip handler
    const handleTooltip = useCallback(
      (data: Sensors[],event: React.TouchEvent<SVGRectElement> | React.MouseEvent<SVGRectElement>) => {
        if (data.length<1) return;
        
        const { x } = localPoint(event) || { x: 0 };
        const date = dateScale.invert(x).valueOf();
        
        // find approximate index from Date
        let index = (date-data[0]['date'])/3;
        
        // check index within range, negative index mean before launch and no data and simple return
        if (index>=data.length)
          index=data.length-1;
        else if (index<0)
           return;
        
        //precisely check the Date and shift the index if necessuary
        
        while(index>=0 && index<data.length){
        
          if (data[index]['date']>date){
              --index;
              if (!index || data[index]['date']<=date)
                break;
          }else 
              ++index;
        }
        
       
        
        showTooltip({
          tooltipData: data[index],
          tooltipLeft: x,
          tooltipTop: iaqValueScale(data[index]['iaq']),
        });
      },
      [showTooltip, iaqValueScale, dateScale],
    );
    
    
       
         
 
    let sensorsList : [string,any,string][] = [['eco2',eco2ValueScale,'#330019'],['temperature',tempValueScale,'#FF0000'],['humidity',humidValueScale,'#3399FF']]; 
     
    let sensorsItems = sensorsList.map( (sensor) =>  {
    
        return <LinePath visibility={(chartconfig && (sensor[0] in chartconfig) && !chartconfig[sensor[0]])?"hidden":"visible"}
            data={data}
            x={d => dateScale(d['date']) ?? 0}
            y={d => sensor[1](d[sensor[0]]) ?? 0}
           
            strokeWidth={5}
            
            stroke={sensor[2]}
            strokeOpacity="75%"
          
            curve={curveMonotoneX}
            />
        
    });

    return (
      <div>
        <svg className='Sensorchart' width={width} height={height}>
      

          <rect
            x={0}
            y={0}
            width={width}
            height={height}
            fill="url(#area-background-gradient)"
            
            rx={5}
          />
          
          <LinearGradient id="area-background-gradient" from={background} to={background2} />
       
    
          
          <linearGradient id="iaq-gradient" x1="0" y1="10%" x2="0" y2="90%">
            <stop offset="0%" stopColor="red" stopOpacity="1" />
            <stop offset="50%" stopColor="yellow" stopOpacity="1" />
            <stop offset="100%" stopColor="green" stopOpacity="1" />
          </linearGradient>
          
       
          
          <GridRows
            left={margin.left}
            scale={iaqValueScale}
            width={innerWidth}
            strokeDasharray="1,3"
            stroke={accentColor}
            strokeOpacity={0}
            pointerEvents="none"
          />
          <GridColumns
            top={margin.top}
            scale={dateScale}
            height={innerHeight}
            strokeDasharray="1,3"
            stroke={accentColor}
            strokeOpacity={0.2}
            pointerEvents="none"
          />
        
          <mask id="areaclosedmask" >
          <AreaClosed
            data={data}
            x={d => dateScale(d['date']) ?? 0}
            y={d => iaqValueScale(d['iaq']) ?? 0}
            yScale={iaqValueScale}
            strokeWidth={1}
            stroke="white"
            fill="white"

            curve={curveMonotoneX}
          />
          </mask>
          
          <rect visibility={(chartconfig && ('iaq' in chartconfig) && !chartconfig['iaq'])?"hidden":"visible"}
            x={0}
            y={0}
            width={width}
            height={height}
            fill="url(#iaq-gradient)"
            mask="url(#areaclosedmask)"
            rx={5}
          /> 
       
          {sensorsItems}
          
          
          <Bar
            x={margin.left}
            y={margin.top}
            width={innerWidth}
            height={innerHeight}
            fill="transparent"
            rx={14}
            onTouchStart={(event) => handleTooltip(data,event)}
            onTouchMove={(event) => handleTooltip(data,event)}
            onMouseMove={(event) => handleTooltip(data,event)}
            onMouseLeave={() => hideTooltip()}
          />
          {tooltipData && (
            <g>
              <Line
                from={{ x: tooltipLeft, y: margin.top }}
                to={{ x: tooltipLeft, y: innerHeight + margin.top }}
                stroke={accentColorDark}
                strokeWidth={2}
                pointerEvents="none"
                strokeDasharray="5,2"
              />
              <circle
                cx={tooltipLeft}
                cy={tooltipTop + 1}
                r={4}
                fill="black"
                fillOpacity={0.1}
                stroke="black"
                strokeOpacity={0.1}
                strokeWidth={2}
                pointerEvents="none"
              />
              <circle
                cx={tooltipLeft}
                cy={tooltipTop}
                r={4}
                fill={accentColorDark}
                stroke="white"
                strokeWidth={2}
                pointerEvents="none"
              />
            </g>
          )}
        </svg>
        {tooltipData && (
          <div>
            <TooltipWithBounds
              key={Math.random()}
              top={tooltipTop - 12}
              left={tooltipLeft + 12}
              style={tooltipStyles}
            >
            <div>
                
                <div>iaq: {tooltipData['iaq']}</div>
                <div>eco2: {tooltipData['eco2']} ppm</div>
                <div>temp: {tooltipData['temperature']} C</div>
                <div>humid: {tooltipData['humidity']} %</div>
                
             </div>
           
             
            </TooltipWithBounds>
            <Tooltip
              top={innerHeight + margin.top - 14}
              left={tooltipLeft}
              style={{
                ...defaultStyles,
                minWidth: 80,
                textAlign: 'center',
                transform: 'translateX(-100%)',
              }}
            >
              {formatDate(tooltipData['date'])}
            </Tooltip>
          </div>
        )}
      </div>
    );
  },
);
