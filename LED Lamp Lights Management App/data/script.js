var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
// Init web socket when the page loads
window.addEventListener('load', onload);

document.addEventListener("DOMContentLoaded", ready);


var sliders = document.getElementsByClassName("slider");
var time = document.getElementsByClassName("time");
var redValue, greenValue, blueValue, brightnessValue, whiteValue;
var startHourValue, endHourValue, startMinValue, endMinValue;
var myJSON;

function ready() {

    document.getElementById('send').addEventListener('click', sendNewLedProfile);

    for (var i = 0; i < sliders.length; i++) {
        sliders[i].addEventListener('mouseup', function () {
            switch (this.id) {
                case 'Red': redValue = parseInt(this.value); console.log(this.id + ": " + redValue); break;
                case 'Green': greenValue = parseInt(this.value); console.log(this.id + ": " + greenValue); break;
                case 'Blue': blueValue = parseInt(this.value); console.log(this.id + ": " + blueValue); break;
                case 'Brightness': brightnessValue = parseInt(this.value); console.log(this.id + ": " + brightnessValue); break;
                case 'White': whiteValue = parseInt(this.value); console.log(this.id + ": " + whiteValue); break;
            }
        });
    }

    for (var i = 0; i < time.length; i++) {
        time[i].addEventListener('change', function () {
            switch (this.id) {
                case 'startHour': startHourValue = parseInt(this.value); console.log(this.id + ": " + startHourValue); break;
                case 'endHour': endHourValue = parseInt(this.value); console.log(this.id + ": " + endHourValue); break;
                case 'startMin': startMinValue = parseInt(this.value); console.log(this.id + ": " + startMinValue); break;
                case 'endMin': endMinValue = parseInt(this.value); console.log(this.id + ": " + endMinValue); break;
            }
        });
    }
}

// function updateTime() {
//     startHourValue = parseInt(document.getElementById("startHour").value);
//     endHourValue = parseInt(document.getElementById("endHour").value);
//     startMinValue = parseInt(document.getElementById("startMin").value);
//     endMinValue = parseInt(document.getElementById("endMin").value);

//     console.log(startHourValue);
//     console.log(startMinValue);
//     console.log(endHourValue); 
//     console.log(endMinValue); 
// }

function onload(event) {
    initWebSocket();
}


function readColors() {
    redValue = parseInt(document.getElementById("Red").value);
    greenValue = parseInt(document.getElementById("Green").value);
    blueValue = parseInt(document.getElementById("Blue").value);
}

function readTime() {
    startHourValue = parseInt(document.getElementById("startHour").value);
    endHourValue = parseInt(document.getElementById("endHour").value);
    startMinValue = parseInt(document.getElementById("startMin").value);
    endMinValue = parseInt(document.getElementById("endMin").value);
}

function readBrightnessAndWhite() {
    brightnessValue = parseInt(document.getElementById("Brightness").value);
    whiteValue = parseInt(document.getElementById("White").value);
}

function sendNewLedProfile() {
    // const ledProfile = {
    //     red: redValue,
    //     green: greenValue,
    //     blue: blueValue
    // };

    //myJSON = JSON.stringify(ledProfile);

    let ledProfile = '{ "Type": "setLedProfile", "Colors": { "Red": ' + redValue + ', "Green": '+ greenValue +', "Blue": '+ blueValue +' }, "Brightness": '+ brightnessValue +', "White": '+ whiteValue +', "Time": { "Hours": { "Start": ' + startHourValue + ', "End": ' + endHourValue + ' }, "Minutes": { "Start": ' + startMinValue + ', "End": ' + endMinValue + ' }}}';

    console.log(ledProfile);

    websocket.send(ledProfile);
}

function getLedProfile() {
    websocket.send("{\"Type\":\"getLedProfile\"}");
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

// When websocket is established, call the getLedProfile() function
function onOpen(event) {
    console.log('Connection opened');
    getLedProfile();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

// Function that receives the message from the ESP32 with the readings
function onMessage(event) {
    console.log(event.data);
    var myObj = JSON.parse(event.data);
    var keys = Object.keys(myObj);
    
    document.getElementById("Red").value = myObj.Colors["Red"];
    document.getElementById("Green").value = myObj.Colors["Green"];
    document.getElementById("Blue").value = myObj.Colors["Blue"];
    
    document.getElementById("Brightness").value = myObj["Brightness"];
    document.getElementById("White").value = myObj["White"];
    
    document.getElementById("startHour").value = myObj.Time.Hours["Start"];
    document.getElementById("endHour").value = myObj.Time.Hours["End"];
    document.getElementById("startMin").value = myObj.Time.Minutes["Start"];
    document.getElementById("endMin").value = myObj.Time.Minutes["End"];

    // for (var i = 0; i < keys.length; i++){
    //     var key = keys[i];
    //     console.log(key);
    //     document.getElementById("Red").value = myObj.Colors[key];
    //     console.log(Colors[key]);
        
    //     console.log(myObj.Colors[key]);
    // }
    
    readColors();
    readTime();
    readBrightnessAndWhite();
}