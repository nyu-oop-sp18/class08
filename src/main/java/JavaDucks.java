public class JavaDucks {

  public interface Duck {
    void quack();
    
    default void push() {
      quack();
    }
  }
  
  public interface Flyable {
    void fly();
  }
  
  class Mallard implements Duck, Flyable {
    public void quack() {
      System.out.println("Quack!");
    }
  
    public void fly() { System.out.println("Heading south!"); }
    
    public void push() { quack(); fly(); }
  }

  class RubberDuck implements Duck {
    public void quack() {
      System.out.println("Squeak!");
    }
  }

}
