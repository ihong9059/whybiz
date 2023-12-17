
const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" class="js-focus-visible">

<title>Web Page Update Demo</title>

  <style>
    table {
      position: relative;
      width:100%;
      border-spacing: 0px;
    }
    tr {
      border: 1px solid white;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
    }
    th {
      height: 20px;
      padding: 3px 15px;
      background-color: #343a40;
      color: #FFFFFF !important;
      }
    td {
      height: 20px;
       padding: 3px 15px;
    }
    .tabledata {
      font-size: 20px;
      position: relative;
      padding-left: 5px;
      padding-top: 5px;
      height:   25px;
      border-radius: 5px;
      color: #FFFFFF;
      line-height: 20px;
      transition: all 200ms ease-in-out;
      background-color: #00AA00;
    }
    .fanrpmslider {
      width: 30%;
      height: 55px;
      outline: none;
      height: 25px;
    }
    .bodytext {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      text-align: left;
      font-weight: light;
      border-radius: 5px;
      display:inline;
    }
    .navbar {
      width: 100%;
      height: 50px;
      margin: 0;
      padding: 10px 0px;
      background-color: #FFF;
      color: #000000;
      border-bottom: 5px solid #293578;
    }
    .fixed-top {
      position: fixed;
      top: 0;
      right: 0;
      left: 0;
      z-index: 1030;
    }
    .navtitle {
      float: left;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 30px;
      font-weight: bold;
      line-height: 50px;
      padding-left: 20px;
    }
   .navheading {
     position: fixed;
     left: 20%;
     height: 50px;
     font-family: "Verdana", "Arial", sans-serif;
     font-size: 20px;
     font-weight: bold;
     line-height: 20px;
     padding-right: 20px;
   }
   .navdata {
      justify-content: flex-end;
      position: fixed;
      left: 30%;
      height: 50px;
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      font-weight: bold;
      line-height: 20px;
      padding-right: 20px;
   }
    .whybiztext {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      text-align: left;
      font-weight: bold;
      border-radius: 5px;
      display:inline;
    }
    .category {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: bold;
      font-size: 20px;
      line-height: 30px;
      padding: 10px 10px 0px 10px;
      color: #000000;
    }
    .heading {
      font-family: "Verdana", "Arial", sans-serif;
      font-weight: normal;
      font-size: 20px;
      text-align: left;
    }
  
    .btn {
      background-color: #444444;
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
    }
    .foot {
      font-family: "Verdana", "Arial", sans-serif;
      font-size: 20px;
      position: fixed;
      left: 10%;
      height:   30px;
      text-align: center;   
      color: #AAAAAA;
      line-height: 20px;
    }
    .container {
      max-width: 1800px;
      margin: 0 auto;
    }
    table tr:first-child th:first-child {
      border-top-left-radius: 5px;
    }
    table tr:first-child th:last-child {
      border-top-right-radius: 5px;
    }
    table tr:last-child td:first-child {
      border-bottom-left-radius: 5px;
    }
    table tr:last-child td:last-child {
      border-bottom-right-radius: 5px;
    }

    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 34px;
    }

    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      -webkit-transition: .4s;
      transition: .4s;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      -webkit-transition: .4s;
      transition: .4s;
    }

    input:checked + .slider {
      background-color: #2196F3;
    }

    input:focus + .slider {
      box-shadow: 0 0 1px #2196F3;
    }

    input:checked + .slider:before {
      -webkit-transform: translateX(26px);
      -ms-transform: translateX(26px);
      transform: translateX(26px);
    }

    .slider.round {
      border-radius: 34px;
    }

    .slider.round:before {
      border-radius: 50%;
    }
  </style>

  <body style="background-color: #efefef" onload="process()">
  
    <header>
      <div class="navbar fixed-top">
          <div class="container">
            <div class="navtitle">Whybiz</div>
            <div class="navdata" id = "date">mm/dd/yyyy</div>
            <div class="navheading">DATE</div><br>
            <div class="navdata" id = "time">00:00:00</div>
            <div class="navheading">TIME</div>
            
          </div>
      </div>
    </header>
//test0 
    <main class="container" style="margin-top:50px">

      <div class="whybiztext" id = "version"></div>
      <br>
      <div class="whybiztext" id = "lora_rssi"></div>
      <br>
      <div class="category">Analog & Switch</div>
      <div style="border-radius: 10px !important;">
      <table style="width:50%">
      <colgroup>
        <col span="1" style="background-color:rgb(230,230,230); width: 10%; color:#000000 ;">
        <col span="1" style="background-color:rgb(200,200,200); width: 10%; color:#000000 ;">
        <col span="1" style="background-color:rgb(230,230,230); width: 10%; color:#000000 ;">
        <col span="1" style="background-color:rgb(200,200,200); width: 10%; color:#000000 ;">
      </colgroup>
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <col span="2"style="background-color:rgb(0,0,0); color:#FFFFFF">
      <tr>
        <th colspan="1"><div class="heading">Pin</div></th>
        <th colspan="1"><div class="heading">Bits</div></th>
        <th colspan="1"><div class="heading">Pin</div></th>
        <th colspan="1"><div class="heading">Bits</div></th>
      </tr>
      <tr>
        <td><div class="bodytext">ADC 34</div></td>
        <td><div class="tabledata" id = "b0"></div></td>
        <td><div class="bodytext">ADC 35</div></td>
        <td><div class="tabledata" id = "b1"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">SW 1</div></td>
        <td><div class="tabledata" id = "sw1"></div></td>
        <td><div class="bodytext">SW 2</div></td>
        <td><div class="tabledata" id = "sw2"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">SW 3</div></td>
        <td><div class="tabledata" id = "sw3"></div></td>
        <td><div class="bodytext">SW 4</div></td>
        <td><div class="tabledata" id = "sw4"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">SW 5</div></td>
        <td><div class="tabledata" id = "sw5"></div></td>
        <td><div class="bodytext">SW 6</div></td>
        <td><div class="tabledata" id = "sw6"></div></td>
      </tr>
      <tr>
        <td><div class="bodytext">SW 7</div></td>
        <td><div class="tabledata" id = "sw7"></div></td>
        <td><div class="bodytext">SW 8</div></td>
        <td><div class="tabledata" id = "sw8"></div></td>
      </tr>

      </table>
    </div>
  
    <div class="category">Relays</div>
  
    <div class="bodytext">0 ~ 3</div>
    <button type="button" class = "btn" id = "btn0" onclick="ButtonPress0()"></button>
    <button type="button" class = "btn" id = "btn1" onclick="ButtonPress1()"></button>
    <button type="button" class = "btn" id = "btn2" onclick="ButtonPress2()"></button>
    <button type="button" class = "btn" id = "btn3" onclick="ButtonPress3()"></button>
    </div>
    <br>
    <div class="bodytext">4 ~ 7</div>
    <button type="button" class = "btn" id = "btn4" onclick="ButtonPress4()"></button>
    <button type="button" class = "btn" id = "btn5" onclick="ButtonPress5()"></button>
    <button type="button" class = "btn" id = "btn6" onclick="ButtonPress6()"></button>
    <button type="button" class = "btn" id = "btn7" onclick="ButtonPress7()"></button>
    </div>

    <br>
    <select class="category" id="select_value" onchange="ChangeValue()">
      <option value="1">select1</option>
      <option value="2">select2</option>
      <option value="3">select3</option>
      <option value="4">select4</option>
    </select>

    <div style="border-radius: 10px !important;">
      <table style="width:50%">
      <colgroup>
        <col span="1" style="background-color:rgb(230,230,230); width: 10%; color:#000000 ;">
        <col span="1" style="background-color:rgb(200,200,200); width: 10%; color:#000000 ;">
      </colgroup>
      <tr>
        <th colspan="1"><label class="switch"><input type="checkbox" id="act1" onchange="Act1()">
        <span class="slider round"></span></label><label for="">act1</label></th>
        <th colspan="1"><label class="switch"><input type="checkbox" id="act2" onchange="Act2()">
        <span class="slider round"></span></label><label for="">act2</label></th>
      </tr>

      </table>
    </div>

  </main>

  <footer div class="foot" id = "temp" >Whybiz Test Web by UTTEC</div></footer>
  
  </body>


  <script type = "text/javascript">
  
    var xmlHttp=createXmlHttpObject();

    function createXmlHttpObject(){
      if(window.XMLHttpRequest){
        xmlHttp=new XMLHttpRequest();
      }
      else{
        xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      }
      return xmlHttp;
    }

    function ButtonPress0() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_0", false);
      xhttp.send();
    }

    function ButtonPress1() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_1", false);
      xhttp.send(); 
    }
    
    function ButtonPress2() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_2", false);
      xhttp.send(); 
    }
    
    function ButtonPress3() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_3", false);
      xhttp.send(); 
    }
    
    function ButtonPress4() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_4", false);
      xhttp.send(); 
    }
    
    function ButtonPress5() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_5", false);
      xhttp.send(); 
    }
    
    function ButtonPress6() {
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("PUT", "BUTTON_6", false);
      xhttp.send(); 
    }

    function ButtonPress7() {
      // var xmlResponse=xmlHttp.responseXML;
      // xmldoc = xmlResponse.getElementsByTagName("LED7");
      // message = xmldoc[0].firstChild.nodeValue;

      var xhttp = new XMLHttpRequest(); 
      // xhttp.open("PUT", "BUTTON_7?VALUE="+message, false);
      xhttp.open("PUT", "BUTTON_7", false);
      xhttp.send(); 
    }
    
    function ChangeValue(){
      var xhttp = new XMLHttpRequest(); 
      var value_str = document.getElementById('select_value');
      xhttp.open("PUT", "UPDATE_SELECT?VALUE="+value_str.options[value_str.selectedIndex].value, false);
      xhttp.send(); 
    }

    function Act1(){
      var xhttp = new XMLHttpRequest(); 
      var value_str = document.getElementById('act1');
      var result = 0;
      if(value_str.checked){result = 1;}
      xhttp.open("PUT", "UPDATE_TOGGLE?VALUE="+result, false);
      xhttp.send(); 
    }
    function Act2(){
      var xhttp = new XMLHttpRequest(); 
      var value_str = document.getElementById('act2');
      var result = 0;
      if(value_str.checked){result = 1;}
      xhttp.open("PUT", "UPDATE_TOGGLE?VALUE="+result, false);
      xhttp.send(); 
    }
    // function to handle the response from the ESP
    function response(){
      var message;
      var barwidth;
      var currentsensor;
      var xmlResponse;
      var xmldoc;
      var dt = new Date();
      var color = "#e8e8e8";
     
      // get the xml stream
      xmlResponse=xmlHttp.responseXML;
  
      document.getElementById("time").innerHTML = dt.toLocaleTimeString();
      document.getElementById("date").innerHTML = dt.toLocaleDateString();
// test1
      // Version
      xmldoc = xmlResponse.getElementsByTagName("VERSION"); //VERSION Display
      message = xmldoc[0].firstChild.nodeValue;

      document.getElementById("version").innerHTML="Version: " + message;


      // LORA_RSSI
      xmldoc = xmlResponse.getElementsByTagName("LORA_RSSI"); //LORA_RSSI Display
      message = xmldoc[0].firstChild.nodeValue;

      document.getElementById("lora_rssi").innerHTML="LORA_RSSI: " + message;
      // document.getElementById("b0").style.width=(barwidth+"%");


      // A0
      xmldoc = xmlResponse.getElementsByTagName("B0"); //bits for A0
      message = xmldoc[0].firstChild.nodeValue;
      
      if (message > 2048){
        color = "#aa0000";
      }
      else {
        color = "#0000aa";
      }
      
      barwidth = message / 40.95;
      document.getElementById("b0").innerHTML=message;
      document.getElementById("b0").style.width=(barwidth+"%");
        
      // A1
      xmldoc = xmlResponse.getElementsByTagName("B1");
      message = xmldoc[0].firstChild.nodeValue;
      if (message > 2048){
        color = "#aa0000";
      }
      else {
        color = "#0000aa";
      }
      document.getElementById("b1").innerHTML=message;
      width = message / 40.95;
      document.getElementById("b1").style.width=(width+"%");
      document.getElementById("b1").style.backgroundColor=color;

//------------------------------------------------------------------------------- 
      xmldoc = xmlResponse.getElementsByTagName("LED");
      message = xmldoc[0].firstChild.nodeValue;
      if (message == 0){document.getElementById("btn0").innerHTML="Relay0 ON";}
      else{document.getElementById("btn0").innerHTML="Relay0 OFF";}         

      xmldoc = xmlResponse.getElementsByTagName("LED1");
      message = xmldoc[0].firstChild.nodeValue;
      if (message == 0){document.getElementById("btn1").innerHTML="Relay1 ON";}
      else{document.getElementById("btn1").innerHTML="Relay1 OFF";}         

      xmldoc = xmlResponse.getElementsByTagName("LED2");
      message = xmldoc[0].firstChild.nodeValue;
      if (message == 0){document.getElementById("btn2").innerHTML="Relay2 ON";}
      else{document.getElementById("btn2").innerHTML="Relay2 OFF";}         

      xmldoc = xmlResponse.getElementsByTagName("LED3");
      message = xmldoc[0].firstChild.nodeValue;
      if (message == 0){document.getElementById("btn3").innerHTML="Relay3 ON";}
      else{document.getElementById("btn3").innerHTML="Relay3 OFF";}         

      xmldoc = xmlResponse.getElementsByTagName("LED4");
      message = xmldoc[0].firstChild.nodeValue;
      if (message == 0){document.getElementById("btn4").innerHTML="Relay4 ON";}
      else{document.getElementById("btn4").innerHTML="Relay4 OFF";}         

      xmldoc = xmlResponse.getElementsByTagName("LED5");
      message = xmldoc[0].firstChild.nodeValue;
      if (message == 0){document.getElementById("btn5").innerHTML="Relay5 ON";}
      else{document.getElementById("btn5").innerHTML="Relay5 OFF";}         

      xmldoc = xmlResponse.getElementsByTagName("LED6");
      message = xmldoc[0].firstChild.nodeValue;
      if (message == 0){document.getElementById("btn6").innerHTML="Relay6 ON";}
      else{document.getElementById("btn6").innerHTML="Relay6 OFF";}         

      xmldoc = xmlResponse.getElementsByTagName("LED7");
      message = xmldoc[0].firstChild.nodeValue;
      if (message == 0){document.getElementById("btn7").innerHTML="Relay7 ON";}
      else{document.getElementById("btn7").innerHTML="Relay7 OFF";}         

// ------------------------------------------------------------------------
      xmldoc = xmlResponse.getElementsByTagName("sw1");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("sw1").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("sw1").innerHTML="OFF";}
      else {document.getElementById("sw1").innerHTML="ON";}

      xmldoc = xmlResponse.getElementsByTagName("sw2");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("sw2").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("sw2").innerHTML="OFF";}
      else {document.getElementById("sw2").innerHTML="ON";}

      xmldoc = xmlResponse.getElementsByTagName("sw3");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("sw3").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("sw3").innerHTML="OFF";}
      else {document.getElementById("sw3").innerHTML="ON";}

      xmldoc = xmlResponse.getElementsByTagName("sw4");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("sw4").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("sw4").innerHTML="OFF";}
      else {document.getElementById("sw4").innerHTML="ON";}

      xmldoc = xmlResponse.getElementsByTagName("sw5");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("sw5").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("sw5").innerHTML="OFF";}
      else {document.getElementById("sw5").innerHTML="ON";}

      xmldoc = xmlResponse.getElementsByTagName("sw6");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("sw6").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("sw6").innerHTML="OFF";}
      else {document.getElementById("sw6").innerHTML="ON";}

      xmldoc = xmlResponse.getElementsByTagName("sw7");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("sw7").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("sw7").innerHTML="OFF";}
      else {document.getElementById("sw7").innerHTML="ON";}

      xmldoc = xmlResponse.getElementsByTagName("sw8");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("sw8").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("sw8").innerHTML="OFF";}
      else {document.getElementById("sw8").innerHTML="ON";}

      xmldoc = xmlResponse.getElementsByTagName("act1");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("act1").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("act1").checked = 1;}
      else {document.getElementById("act1").checked = 0;}

      xmldoc = xmlResponse.getElementsByTagName("act2");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("act2").style.backgroundColor="rgb(200,200,200)";
      if (message == 0){document.getElementById("act2").checked = 1;}
      else {document.getElementById("act2").checked = 0;}
    }  

    function process(){     
      if(xmlHttp.readyState==0 || xmlHttp.readyState==4) {
        xmlHttp.open("PUT","xml",true);
        xmlHttp.onreadystatechange=response;
        xmlHttp.send(null);
      }       
      setTimeout("process()",200);
    }  

  </script>

</html>



)=====";
