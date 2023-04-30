#ifndef _HOME_HTML_H
#define _HOME_HTML_H

#include <Arduino.h>

const char HOMEPAGE_HTML[] PROGMEM = R"=====(
<!DOCTYPE html><html><head>
<meta charset='utf-8' name="viewport" content="width=device-width, initial-scale=1">
<title>ECU-ESP8266</title>
<link rel="stylesheet" type="text/css" href="/mypico.css">
<style>
body {
  background-color: #f6f8f6;
}
.console {
  max-height: 75vh;
  overflow: auto;
  display: flex;
  flex-direction: column-reverse;
  background: black;
  font-family: Mono;
  color: white;
  padding: 2em;
}
.debug {
  color: #9f9f9f;
  font-size: 14px;
}
.info {
  color: #fefefe;
  font-size: 16px;
}
</style>

<script type="text/javascript">
  var webSocket = new WebSocket("ws://" + location.host + "/ws");
  webSocket.onopen = function(e) {
    console.log("Websocket: open");
  }
  webSocket.onclose = function(e) {
    console.log("Websocket: close");
  }
  
  webSocket.onmessage = function(e) {
    console.log("Websocket: message");
    console.log(e.data);

    var data = e.data;
    
    try {
      jData = JSON.parse(e.data);
      if (jData.type == 'log') {
        let logEntry = document.createElement("div");
        logEntry.className = jData.level;
        logEntry.prepend(jData.message);
        document.getElementById('console').prepend(logEntry);
      }
      else {
        let logEntry = document.createElement("div");
        logEntry.prepend(data);
        document.getElementById('console').prepend(logEntry);
      }
    } catch(e) {
      let logEntry = document.createElement("div");
      logEntry.prepend(data);
      document.getElementById('console').prepend(logEntry);
    }
  }
</script>
</head>

<body class="container">
<nav><ul>
<li><a href="/">Home</a></li>
<li><a href="/">Setting</a></li>
<li><a href="/coordinator" role="button">restart coordinator</a></li></ul>
</nav>

<div class="console" id="console">
</div>
</body>
<footer></footer>
</html>
)=====";

#endif