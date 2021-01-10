const addon = require('../build/Release/airquality-native');

function Airquality(name) {
   
    
    this.startsensormeasurement = function(callback){
        return _addonInstance.startsensormeasurement(callback);
    }
    
    this.clearup = function(){
        return _addonInstance.clearup();
    }
    
    this.geths300x = function(str){
        return _addonInstance.geths300x(str);
    }



    var _addonInstance = new addon.Airquality(name);
}

module.exports = Airquality;
