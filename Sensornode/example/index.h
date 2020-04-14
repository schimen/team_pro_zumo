const char mainPage[] PROGMEM = R"=====(
    <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Sensor Data</title>
    
    <style>
        .flex-Container{
            display: flex;
        }
        .sensor-Card{
            font-family: sans-serif;
            max-width: 30vw;
            min-width: 25vw;
            min-height: 50vh;
            background: #d0fdf0;
            padding: 1vh;
            padding-left: 3vh;
            margin: 1vh;
            border-radius: 25px;
            box-sizing: border-box;
            box-shadow: .5vh .5vh rgba(0, 0, 0, 0.295);
        }
        .sensor-Data{
            text-align: right;
            width: 50px !important;
            display:  inline-block;
            padding: 0;
            margin: 0;
        }
        .blink{
            border-radius: 50%;
        }
        h1{
            font-family: monospace;
            font-size: 1.8em;
        }
        table{
            font-family: sans-serif;
            width: 100%;
        }
    </style>
</head>
<body>
    <div class ="flex-Container">
        <div class="sensor-Card">
            <h1>Sensor 1</h1>
            <table>
                <tr>
                    <td>Value: </td>
                    <td id="sens1Val">0</td>
                </tr>
                <tr>
                    <td>Min. value: </td>
                    <td id="sens1Min">0</td>
                </tr>
                <tr>
                    <td>Max. value: </td>
                    <td id="sens1Max">0</td>
                </tr>
                <tr>
                    <td>Average pr. 10 readings: </td> <!--Ask Simen how man avg-->
                    <td id="sens1Avg">0</td>
                </tr>
            </table>
            <div class="blink" id="sens1Blink"></div>
        </div>
        <div class="sensor-Card">
            <h1>Sensor 2</h1>
        </div>
        <div class="sensor-Card">
            <h1>Sensor 3</h1>
        </div>
    </div>
</body>
</html>

<script>
 
setInterval(function() {

  // Call a function repetatively with 2 Second interval
  getData();
}, 2000); //2000mSeconds update rate
 
function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("sens1Val").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "sens1Val", true);
  xhttp.send();
  
}
</script>

)=====";
