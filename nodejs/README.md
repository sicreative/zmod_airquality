# zmod_airquality Nodejs
### Demo of ZMOD AirQuality Kit Nodejs 


This as a part of Zmod Air Quality Raspberry as a dash board for elements14 Roadtest.

https://www.element14.com/community/roadTestReviews/3517/l/idt-zmod4410-indoor-air-quality-raspberry-pi-hat-review


You need to agree with Renesas software licences, approvided and download the firmware and relative code at
Renesas's sites, and copy the relative source code and lib to nodejs root folder.

### Install Node JS 

following nodejs to install Node.js
https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions

### Install Node-gyp
```
npm install -g node-gyp
```
https://github.com/nodejs/node-gyp

### Compile the native add-on 

Copy those file from Renesas package to root directory
![alt directory](pic/zmod_2.png)


***Renesas_ZMOD4410_IAQ_2nd_Gen_Example/ZMOD4410_Firmware/zmod4xxx_example/src/zmod4xxx_types.h***

***Renesas_ZMOD4410_IAQ_2nd_Gen_Example/ZMOD4410_Firmware/zmod4xxx_example/src/zmod4xxx.h***

***Renesas_ZMOD4410_IAQ_2nd_Gen_Example/ZMOD4410_Firmware/zmod4xxx_example/src/zmod4410_config_iaq2.h***

***Renesas_ZMOD4410_IAQ_2nd_Gen_Example/ZMOD4410_Firmware/zmod4xxx_example/src/zmod4xxx.c***

***ZMOD4410_Firmware/gas-algorithm-libraries/iaq_2nd_gen/Raspberry Pi/ARMv8-A/arm-linux-gnueabihf-gcc/lib_iaq_2nd_gen.a***

***ZMOD4410_Firmware/gas-algorithm-libraries/iaq_2nd_gen/Raspberry Pi/ARMv8-A/arm-linux-gnueabihf-gcc/lib_iaq_2nd_gen.h***

build native addon
```
node-gyp build
```

### Install PM2 
https://pm2.keymetrics.io/docs/usage/quick-start/
```
npm install pm2@latest -g
```
### Start the app
pm2 start airquality.js


## Install NGNIX web server
follow the guide from nginx 
https://www.nginx.com/resources/wiki/start/topics/tutorials/install/#official-debian-ubuntu-packages

modifity /etc/nginx/sites-available/default 
### Insert proxy server:
```
location /sensor {
    proxy_pass http://127.0.0.1:8182;
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection 'upgrade';
    proxy_set_header Host $host;
    proxy_cache off;
    proxy_cache_bypass 1
    add_header 'Cache-Control' 'no-store, no-cache, must-revalidate, proxy-revalidate, max-age=0';
            expires off;
            add_header    'Access-Control-Allow-Methods' 'GET, POST, OPTIONS' always;
            add_header    'Access-Control-Allow-Headers' 'Origin, X-Requested-With, Content-Type, Accept' always;

#    proxy_cache_bypass $http_upgrade;
}

```

### For use production version of React frontend
```
# root /var/www/html;

# React build folder
root /home/pi/airquality/react/build;


location / {
    # First attempt to serve request as file, then
    # as directory, then fall back to displaying a 404.
    try_files $uri $uri/ =404;
}

```

### For use development version of React frontend
```
location / {
        proxy_pass http://127.0.0.1:3000;
                proxy_http_version 1.1;
                proxy_set_header Upgrade $http_upgrade;
                proxy_set_header Connection 'upgrade';
                proxy_set_header Host $host;
}
```

