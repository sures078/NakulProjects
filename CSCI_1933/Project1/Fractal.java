//Nakul Suresh, Student ID: 5497189
//Hunter Warner, Student ID: 5337997

import java.util.Scanner;
import java.awt.*;

public class Fractal {

  //DRAWS CIRCULAR FRACTAL AND RETURNS TOTAL AREA OF ALL CIRCLES
  public static double drawCircle(Canvas drawing, Color[] colors) {
    double radius = 400;
    double x = 400;
    double y = 400;
    double deltaX = 0;
    double totalArea = 0;
    for(int i = 0; i < 10; i++) {
      for(int j = 0; j < Math.pow(2, i); j++) {
        Circle c = new Circle(x, y, radius);
        c.setColor(colors[i%3]);
        drawing.drawShape(c);
        totalArea += c.calculateArea();
        x += deltaX;
      }
      x = radius/2;         //new x
      radius /= 2;          //new radius
      deltaX = radius * 2;  //new deltaX
    }
    return totalArea;
  }

  //DRAWS TRIANGULAR FRACTAL AND RETURNS TOTAL AREA OF ALL TRIANGLES
  public static double drawTriangle(Canvas drawing, int counter, Triangle previous) {
    drawing.drawShape(previous);
    double area;

    if(counter == 1) { //base case
      return 0;
    }

    //Left Triangle
    double x = previous.getXPos() - 0.5*previous.getWidth();
    double y = previous.getYPos();
    double width = 0.5 * previous.getWidth();
    double height = 0.5 * previous.getHeight();
    Triangle left = new Triangle(x, y, width, height);
    left.setColor(Color.BLUE);
    area = left.calculateArea() + drawTriangle(drawing, counter - 1, left);

    //Right Triangle
    x = previous.getXPos() + previous.getWidth();
    Triangle right = new Triangle(x, y, width, height);
    right.setColor(Color.RED);
    area += right.calculateArea() + drawTriangle(drawing, counter - 1, right);

    //Top triangle
    x = previous.getXPos() + 0.5*previous.getWidth() - 0.5*(0.5*previous.getWidth());
    y = previous.getYPos() - previous.getHeight();
    Triangle top = new Triangle(x, y, width, height);
    top.setColor(Color.GREEN);
    area += top.calculateArea() + drawTriangle(drawing, counter - 1, top);

    return area;
  }



    //DRAWS RECTANGULAR FRACTAL AND RETURNS TOTAL AREA OF ALL RECTANGLES
    public static double drawRectangle(Canvas drawing, int counter, Rectangle previous) {
    drawing.drawShape(previous);
    double area;

    if(counter == 1) { //base case
      return 0;
    }

    //Bottom Left Rectangle
    double x = previous.getXPos() - 0.5*previous.getWidth();
    double y = previous.getYPos() + previous.getHeight();
    double width = 0.5 * previous.getWidth();
    double height = 0.5 * previous.getHeight();
    Rectangle botLeft = new Rectangle(x, y, width, height);
    botLeft.setColor(Color.RED);
    area = botLeft.calculateArea() + drawRectangle(drawing, counter - 1, botLeft);

    //Bottom Right Rectangle
    x = previous.getXPos() + previous.getWidth();
    Rectangle botRight = new Rectangle(x, y, width, height);
    botRight.setColor(Color.BLUE);
    area += botRight.calculateArea() + drawRectangle(drawing, counter - 1, botRight);

    //Top Left Rectangle
    x = previous.getXPos() - 0.5*previous.getWidth();
    y = previous.getYPos() - 0.5*previous.getHeight();
    Rectangle topLeft = new Rectangle(x, y, width, height);
    topLeft.setColor(Color.GREEN);
    area += topLeft.calculateArea() + drawRectangle(drawing, counter - 1, topLeft);

    //Top Right Rectangle
    x = previous.getXPos() + previous.getWidth();
    Rectangle topRight = new Rectangle(x, y, width, height);
    topRight.setColor(Color.YELLOW);
    area += topRight.calculateArea() + drawRectangle(drawing, counter - 1, topRight);

    return area;
  }


  //EXPAND CANVAS WINDOW TO SEE FULL FRACTAL IMAGE
  public static void main(String[] args){
    Canvas drawing = new Canvas(800, 800);
    Scanner keyboard = new Scanner(System.in);
    System.out.println("Which shape would you like? Circle, Triangle or Rectangle?");

    String input = keyboard.next();
    while(!input.equalsIgnoreCase("triangle") && !input.equalsIgnoreCase("rectangle") && !input.equalsIgnoreCase("circle")) {
      System.out.println("Try Again");
      input = keyboard.next();
    }

    Color[] colors = {Color.RED, Color.BLUE, Color.GREEN};

    if (input.equalsIgnoreCase("circle")) {
      double totalArea = drawCircle(drawing, colors);
      System.out.println("The total area is " + totalArea);
    }

    else if (input.equalsIgnoreCase("triangle")) {
      Triangle first = new Triangle(250,550,250,250);
      first.setColor(Color.RED);
      double totalArea = drawTriangle(drawing, 10, first) + first.calculateArea();
      System.out.println("The total area is " + totalArea);
    }

    else if (input.equalsIgnoreCase("rectangle")){
      Rectangle first = new Rectangle(300,450,200,100);
      first.setColor(Color.RED);
      double totalArea = drawRectangle(drawing, 10, first) + first.calculateArea();
      System.out.println("The total area is " + totalArea);
    }
  }
}
