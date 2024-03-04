var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
// Init web socket when the page loads
window.addEventListener('load', onload);

document.addEventListener("DOMContentLoaded", ready);


var sliders = document.getElementsByClassName("slider");
var redValue, greenValue, blueValue;
var myJSON;

function ready() {

    document.getElementById('send').addEventListener('click', sendNewLedProfile);

    for (var i = 0; i < sliders.length; i++) {
        sliders[i].addEventListener('mouseup', function () {
            //alert(this.id + ": " + this.value);
            switch (this.id) {
                case 'Red': redValue = parseInt(this.value); console.log(this.id + ": " + redValue); break;
                case 'Green': greenValue = parseInt(this.value); console.log(this.id + ": " + greenValue); break;
                case 'Blue': blueValue = parseInt(this.value); console.log(this.id + ": " + blueValue); break;
            }
        });
    }
}

function onload(event) {
    initWebSocket();
}


function readColors() {
    redValue = parseInt(document.getElementById("Red").value);
    greenValue = parseInt(document.getElementById("Green").value);
    blueValue = parseInt(document.getElementById("Blue").value);
}

function sendNewLedProfile() {
    // const ledProfile = {
    //     red: redValue,
    //     green: greenValue,
    //     blue: blueValue
    // };

    //myJSON = JSON.stringify(ledProfile);

    let ledProfile = '{ "Type": "setLedProfile", "Colors": { "Red": ' + redValue + ', "Green": '+ greenValue +', "Blue": '+ blueValue +' }, "Brightness": 80, "Time": { "Hours": { "Start": 10, "End": 18 }, "Minutes": { "Start": 15, "End": 15 } } }';

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

    // for (var i = 0; i < keys.length; i++){
    //     var key = keys[i];
    //     console.log(key);
    //     document.getElementById("Red").value = myObj.Colors[key];
    //     console.log(Colors[key]);
        
    //     console.log(myObj.Colors[key]);
    // }
    
    readColors();
}