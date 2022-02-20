package kr.ac.kookmin.udclasses;

import java.util.Properties;

public class DataInitializer {
	public DVD retrieveDvd(Properties properties, int dvdIndex) {
		// set up the keys to read from the properties file
		String titleKey = new StringBuffer().append("dvd").
				append(dvdIndex).append(".title").toString();
		System.out.print(titleKey + " = ");
		String categoryKey = new StringBuffer().append("dvd").
				append(dvdIndex).append(".category").toString();
		String costKey = new StringBuffer().append("dvd").
				append(dvdIndex).append(".cost").toString();
		String directorKey = new StringBuffer().append("dvd").
				append(dvdIndex).append(".director").toString();
		String lengthKey = new StringBuffer().append("dvd").
				append(dvdIndex).append(".length").toString();
		// Create a new dvd object and set the fields
		DVD dvd = new DVD();
		dvd.setTitle(properties.getProperty(titleKey));
		System.out.println(dvd.getTitle());
		dvd.setCategory(properties.getProperty(categoryKey));
		dvd.setCost(Float.parseFloat(
				properties.getProperty(costKey)));
		dvd.setDirector(properties.getProperty(directorKey));
		dvd.setLength(Integer.parseInt(
				properties.getProperty(lengthKey)));
		return dvd;
		}
}
