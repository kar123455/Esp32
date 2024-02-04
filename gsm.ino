void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
retry();
}
void retry (){
  readandsend();
  if(sim.find(200)){
    Serial.println("gotit")
    delay(1000);

  }
  else{
    for(int i=0;i<2;i++){
      retry();
    }
    delay(1000);
    ESP.restart();
  }
}
