import java.awt.*;

public class Triangle {
  private double xBotLeft;
  private double yBotLeft;
  private double width;
  private double height;
  private Color color;

  public Triangle(double x, double y, double wid, double ht) {
    xBotLeft = x;
    yBotLeft = y;
    width = wid;
    height = ht;
  }

  public double calculatePerimeter() {
    double a = Math.pow((width/2),2);
    double b = Math.pow(height,2);
    double sideLen = Math.sqrt(a + b);
    return 2 * sideLen + width;
  }

  public double calculateArea() {
    return 0.5 * width * height;
  }

  public void setColor(Color col) {
    color = col;
  }

  public void setPos(double x, double y) {
    xBotLeft = x;
    yBotLeft = y;
  }

  public void setWidth(double wid){
    width = wid;
  }

  public void setHeight(double ht) {
    height = ht;
  }

  public Color getColor() {
    return color;
  }

  public double getXPos() {
    return xBotLeft;
  }

  public double getYPos() {
    return yBotLeft;
  }

  public double getWidth() {
    return width;
  }

  public double getHeight() {
    return height;
  }
}
