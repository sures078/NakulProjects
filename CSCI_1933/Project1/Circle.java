import java.awt.*;

public class Circle {
  private double xPos; //x position of center
  private double yPos; //y position of center
  private double radius;
  private Color color;

  public Circle(double x, double y, double rad) {
    xPos = x;
    yPos = y;
    radius = rad;
  }

  public double calculatePerimeter() {
    return 2 * radius * 3.14159;
  }

  public double calculateArea() {
    return 3.14159 * Math.pow(radius, 2);
  }

  public void setColor(Color col) {
    color = col;
  }

  public void setPos(double x, double y) {
    xPos = x;
    yPos = y;
  }

  public void setRadius(double rad) {
    radius = rad;
  }

  public Color getColor() {
    return color;
  }

  public double getXPos() {
    return xPos;
  }

  public double getYPos() {
    return yPos;
  }

  public double getRadius() {
    return radius;
  }
}
