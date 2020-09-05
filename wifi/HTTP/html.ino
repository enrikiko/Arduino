String html = "<div class='container'>  <div class='row'>    <div class='col-sm'>      One of three columns    </div>    <div class='col-sm'>      One of three columns    </div>    <div class='col-sm'>      One of three columns    </div>  </div>   </div>";

void handleRoot() {
blind();
server.send(200, "text/html", "<html><h1>Wellcome to wemos</h1><h1><a href=\"/enrique\">Click here to go to enrique.</a></h1><h1><a href=\"/404\">Click here to go to 404.</a></h1></html>");
Serial.println("HTTP request! Root");
first = "Basic";
second = "Things";
http();
}

void handleException() {
blind();
server.send(404, "text/html", "<html><h1>Error 404, Page not found</h1><h1><a href=\"/enrique\">Click here to go to enrique.</a></h1><h1><a href=\"/\">Click here to go to home.</a></h1></html>");
Serial.println("HTTP request! Exception");
first = "null";
second = "null";
http();
}

void handleEnrique() {
blind();
server.send(200, "text/html", "<html><h1>Enrique</h1><h1><a href=\"/\">Click here to go to home.</a></h1><h1><a href=\"/404\">Click here to go to 404.</a></h1></html>");
Serial.println("HTTP request! Enrique");
first = "Enrique";
second = "Ramos";
http();
}

void handleHtml() {
blind();
server.send(200, "text/html", html);
Serial.println("HTTP request! Html");
first = "HTML";
second = "send";
http();
}
