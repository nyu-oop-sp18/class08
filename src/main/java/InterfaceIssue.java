interface I {
  void m();
}

interface I1 extends I {
  default void m() { System.out.println("I1"); }
}

interface I2 extends I {
  // can't inherit default implementation for the same method from two interfaces
  //default void m() { System.out.println("I2"); }
}

public class InterfaceIssue implements I1, I2 {
}
