public class CastTwo
{
 public static void main(String[] args)
 {
	 byte byte1;
	 short short1;
	 char char1 = '\ud55c';
	 int int1;
	 float float1 = 3.6f;
	 
	 byte1 = (byte) char1;
	 System.out.print("char1 = "+char1+"=>\t byte1 = "+byte1);
	 System.out.println("=> \t(char)"+byte1 +"="+(char)byte1);
	 
	 short1 = (short) char1;
	 System.out.print("char1 = "+char1+"=>\t short1 = " + short1);
	 System.out.println("=> \t(char)"+short1 +" = "+(char)short1);
	 
	 int1 = char1;
	 System.out.print("char1 = "+char1+"=>\t int1 = "+int1);
	 System.out.println("=> \t(char)"+int1+" = "+(char)int1);
	 
	 int1 = (int) float1;
	 System.out.print("float1 = "+float1 +" => \t int1 =" +int1);
	 System.out.println("=> \t(float)"+int1 +" = "+(float) int1);
	 
	 short1 = -134;
	 byte1 = (byte) short1;
	 System.out.println("short1 = "+short1 +"=> \t byte1 = "+byte1);
 }
}