public class StudentScores
{
	private final int MAX_STUDENTS = 100;
	private Student[] s;
	private int numStudents;
	
	public StudentScores()
	{
		s = new Student[MAX_STUDENTS];
		numStudents = 0;
	}
	
	public void add(String name, int score)
	{
		if (numStudents >= MAX_STUDENTS)
			return; // not enough space to add new student score
		s[numStudents] = new Student(name, score);
		numStudents++;
	}
	
	public Student getHighest()
	{
		if (numStudents == 0)
			return null;
		
		int highest = 0;
		
		for (int i = 1; i < numStudents; i++)
			if (s[i].getScore() > s[highest].getScore())
		highest = i;
		
		return s[highest];
	}

	public Student getLowest()
	{
		if (numStudents == 0)
			return null;
		
		int lowest = 0;
		
		for (int i = 1; i < numStudents; i++)
			if (s[i].getScore() < s[lowest].getScore())
					lowest = i;
		
		return s[lowest];
	}
}
