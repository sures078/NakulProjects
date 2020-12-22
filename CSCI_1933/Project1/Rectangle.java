import java.awt.*;

public class Rectangle {
  private double xTopLeft;
  private double yTopLeft;
  private double width;
  private double height;
  private Color color;

  public Rectangle(double x, double y, double wid, double ht) {
    xTopLeft = x;
    yTopLeft = y;
    width = wid;
    height = ht;
  }

  public double calculatePerimeter() {
    return (2 * width) + (2 * height);
  }

  public double calculateArea() {
    return width * height;
  }

  public void setColor(Color col) {
    color = col;
  }

  public void setPos(double x, double y) {
    xTopLeft = x;
    yTopLeft = y;
  }

  public void setHeight(double ht) {
    height = ht;
  }

  public void setWidth(double wid) {
    width = wid;
  }

  public Color getColor() {
    return color;
  }

  public double getXPos() {
    return xTopLeft;
  }

  public double getYPos() {
    return yTopLeft;
  }

  public double getWidth() {
    return width;
  }

  public double getHeight() {
    return height;
  }
}
