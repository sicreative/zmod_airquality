import React from 'react';
import  './SensorItem.css';

type props = {
		
		name?: string;
		id?: string;
		value?: number;
		chartcolor?: string;
		namecolor?: string;
		checkedCallback?: (id: string,enable:Boolean) => void;
		
	  };

class SensorItem extends React.Component<props,any> {
	
   
    
    constructor(props:any){
		super(props);
		 this.state = {
			checked : true,
		 };
		 
		 
		
    }
    
  
    
    toggleChecked= () => {
    
		let value = !this.state.checked;
    
		this.setState({
			checked : value
			});
			
			
			
			
		if (this.props.checkedCallback  && this.props.id ){
			
				this.props.checkedCallback(this.props.id,value);
		}
	   		
    }
	  
	
	render(){
		return (
		<div className = "Sensorboard">
			<div className = "Sensorname" style = {{ backgroundColor: this.props.namecolor }}>{this.props.name} 
				<div className="toggle-container" onClick={this.toggleChecked}>
				<div className={`toggle-button ${this.state.checked ? "" : "disabled"}`} style={this.state.checked ? {backgroundColor: this.props.chartcolor}:{}}>
					
					</div>
			</div>
				
			
			</div>
			
		
			<div className = "Sensorvalue">{this.props.value ?? '-'}</div>
		
			
			
			
		</div>
		);
	}
}


export default SensorItem;
