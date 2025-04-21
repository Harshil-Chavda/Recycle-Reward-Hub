
void handleRoot() {
  server.send(200, "text/plain", "Hello, world! ");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

void handlePost() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    Serial.println("Received POST data:");
    Serial.println(body);
    server.send(200, "application/json", "{\"status\":\"OK\"}");
  } else {
    server.send(400, "application/json", "{\"status\":\"Failed\"}");
  }
}
