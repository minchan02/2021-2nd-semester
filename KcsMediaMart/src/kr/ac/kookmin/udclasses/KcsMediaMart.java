package kr.ac.kookmin.udclasses;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Properties;

public class KcsMediaMart {

	public static void main(String[] args) 
				throws FileNotFoundException {
			Properties prop = new Properties();
			FileInputStream fis =
					new FileInputStream("media.properties");
			try {
				prop.load(fis);
			}
			catch(IOException e) {
			}
			// Create a new object for getting property data
			DataInitializer data = new DataInitializer();
			
			// Create a new Order object
			Order anOrder = new Order();
		
			// add a number of dvds to the order
			int dvdsToBeAdded = 3;
			for (int dvdNumber=1; dvdNumber <= dvdsToBeAdded; dvdNumber++){
				// get the dvd data from the properties file
				DVD dvd = data.retrieveDvd(prop, dvdNumber);
				// add the dvd to the order
				anOrder.addDVD(dvd);
			}
			System.out.print("Total Cost is: ");
			System.out.println(anOrder.totalCost());
		}
	

}
