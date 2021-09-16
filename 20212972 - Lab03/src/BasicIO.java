import java.util.*;

// An exploration of basic input and output.
class BasicIO {

   // Reads and processes string input.
   public static void main(String[] args) {

      Scanner stdin = new Scanner(System.in);

      // get first input
      System.out.print("Enter your name: ");
      String name = stdin.nextLine();
      System.out.print("Enter your age: ");
      int years = stdin.nextInt();
      System.out.print("Enter your height: ");
      int height = stdin.nextInt();
      
      // display output on console
      System.out.println("your name is " + name + " and your age is " + years);
      System.out.println("you leave " + years*365 + "days");
      System.out.printf("2015년 9월 06일 현재 %s(%d)의 키는 %d cm 입니다.", name, years, height);
      stdin.close();
   }  // method main

}  // class BasicIO
