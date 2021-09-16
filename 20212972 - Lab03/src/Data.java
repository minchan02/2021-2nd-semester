// An exploration of primitive data types.
class Data {

   // Declares and uses variables of various types.
   public static void main(String[] args) {

      int num1 = 40, num2 = 65, x = 99;
      float num3 = 18.53f;
      char letter = 'x';
      num1 = 777;
      double num4 = num1;
      boolean flag = false;
      final int FIXED = 2020;
      // FIXED = 3030;
      // flag = num1;
      
      System.out.println("The value of num1: " + num1);
      System.out.println("The value of num2: " + num2);
      System.out.println("The value of num3: " + num3);
      System.out.println("The value of num4: " + num4);
      System.out.println("The value of flag: " + flag);
      System.out.println("The value of x: " + x);
      System.out.println("The value of fixed: " + FIXED);
      System.out.println("The letter x: " + letter);

   }  // method main

}  // class Data
