import java.awt.*;
import processing.serial.*;
import java.awt.event.InputEvent;
import java.awt.geom.GeneralPath;
import javax.swing.*;
import java.awt.event.*;

Robot robot;
Serial port;

void setup(){
  
  try{
    robot = new Robot();
  }catch(AWTException e){
  }
  println(Serial.list());

}
int count = 0;

void draw(){
  //scale(-1, 1);
  while(port.available() > 0){
    delay(100);
    String s = port.readString();
    //println(s);
    int[] mouseCo = new int[2];
    String[] tokens = s.split("/");
    int buttonPressed=0;
    int[] leftRight=new int[2];
    //int i = 0;
    for(int i = 0; i < 2; i++){
      mouseCo[i] = Integer.valueOf(tokens[i].toString().trim());
      print(mouseCo[i]);
      //i++;
    }
    if(tokens.length > 2){
        buttonPressed=1;
        leftRight[0]=Integer.valueOf(tokens[2].toString().trim());
        leftRight[1]=Integer.valueOf(tokens[3].toString().trim());
        //leftRight[2]=Integer.valueOf(tokens[4].toString().trim());
        print(leftRight[0]);
        print(leftRight[1]);
        //print(leftRight[2]);
        if(leftRight[0] == 1 && leftRight[1] == 0){
           robot.mousePress(InputEvent.BUTTON1_DOWN_MASK);
           robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
        }  
        else if(leftRight[0] == 0 && leftRight[1] == 1){
           robot.mousePress(InputEvent.BUTTON3_DOWN_MASK);
           robot.mouseRelease(InputEvent.BUTTON3_DOWN_MASK);
        }
        
    }
      
    
    if(buttonPressed == 0){
      Point p = MouseInfo.getPointerInfo().getLocation();
      robot.mouseMove(p.x + mouseCo[0], p.y + mouseCo[1]);
    }
    
    println();
    //println(x +" "+ y);
    
  }
}


//class MouseMoveOnScreen{
//  //Robot rb;
//  GeneralPath gp;
//  int prevX;
//  int prevY;
//  MouseMoveOnScreen() throws AWTException{
//    //rb = new Robot();
//    //rb = robot;
//    gp = new GeneralPath();
//    Point p = MouseInfo.getPointerInfo().getLocation();
//    gp.moveTo(p.x, p.y);
//    //println("PREV: ", p.x, p.y);
//    prevX = p.x;
//    prevY = p.y;
//    //drawLatestMouseMovement();
//    ActionListener al = new ActionListener() {

//        Point lastPoint = new Point(prevX, prevY);

//        @Override
//        public void actionPerformed(ActionEvent e) {
//            Point p = MouseInfo.getPointerInfo().getLocation();
//            //println("CURR: ", p.x, p.y);
//            if (!p.equals(lastPoint)) {
//                gp.lineTo(p.x, p.y);
//                if(lastPoint.y < p.y){
//                  println("DOWN");
//                  robot.mouseWheel(p.y - lastPoint.y);
//                }
//                else if(lastPoint.y > p.y){
//                  println("UP");
//                  robot.mouseWheel(-(lastPoint.y - p.y));
//                }
//                //drawLatestMouseMovement();
//            }
//            lastPoint = p;
//        }
//    };
//    Timer timer = new Timer(100, al);
//    timer.setRepeats(false);
//    timer.start(); 
//  }
//}
